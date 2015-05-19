
/**
 * @author Alexander Preisinger
 * @date   2013-12-03
 */

#include <iostream>
#include <iomanip>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "Logging.h"

namespace c = std::chrono;

Logger::Logger(int fd, std::string const & section) :
    mFD(fd),
    mSection(section)
{
}

Logger::~Logger ()
{
}

void Logger::beginTimeMeasure()
{
    mTimePointStack.push(
        c::time_point_cast<c::microseconds>(c::high_resolution_clock::now()));
}

void Logger::endTimeMeasure(std::string const & msg)
{
    TimePoint mEndTime =
        c::time_point_cast<c::microseconds>(c::high_resolution_clock::now());
    if (!mTimePointStack.size()) {
        error("No Start-Time found for time measurement");
        return;
    }
    auto elapsed = mEndTime - mTimePointStack.top();
    mTimePointStack.pop();

    std::stringstream ss;
    ss << "[Elapsed Microseconds: " << std::setw(4) << elapsed.count() << "] " << msg;
    writeLine(ss.str());
}

void Logger::error(std::string const & msg)
{
    std::string error_message = "[EE] " + msg;
    std::cerr << error_message << std::endl;
    writeLine(error_message);
}

void Logger::warn(std::string const & msg)
{
    writeLine("[WW] " + msg);
}

void Logger::info(std::string const & msg)
{
    writeLine("[II] " + msg);
}

void Logger::writeLine(std::string const & msg)
{
	// do not use with matlab control loop
	// because realtime constraints not met...
    std::string to_write =  "[" + mSection + "]" + msg;
    int ret = 0;
    ret = write(mFD, to_write.c_str(), to_write.length());
    if (ret != EWOULDBLOCK && ret != (int)to_write.length())
        std::cerr << "Error writing log file" << std::endl;

    ret = write(mFD, "\n", 1);
    if (ret != EWOULDBLOCK && ret != 1)
        std::cerr << "Error writing log file" << std::endl;
}


Logging::Logging(std::string const & filename)
{
    mFD = ::open(filename.c_str(), O_CREAT | O_APPEND | O_NONBLOCK | O_RDWR, 0600);
    if (mFD == -1) {
        std::cerr << "Could not open file: " << filename << std::endl;
        mMappedMem = MAP_FAILED;
    }
    else {
        errno = 0;
        mMappedSize = 1024 * 1024 * 2;
        mMappedMem =  mmap(NULL, mMappedSize, PROT_WRITE, MAP_PRIVATE, mFD, 0);

        if (mMappedMem == MAP_FAILED)
            std::cerr << "Memory Mapping failed: " << strerror(errno) << std::endl;
    }
}

Logging::~Logging ()
{
    flush();

    if (mMappedMem != MAP_FAILED)
        munmap(mMappedMem, mMappedSize);

    if (mFD != -1)
        close(mFD);
}

void Logging::flush()
{
    if (mMappedMem != MAP_FAILED)
        msync(mMappedMem, mMappedSize, MS_SYNC);

#ifdef _GNU_SOURCE
    if (mFD != -1)
        syncfs(mFD);
#else
    sync();
#endif
}

Logger * Logging::createLogger(const std::string &section)
{
    return new Logger(mFD, section);
}
