
/**
 * @author Alexander Preisinger
 * @date   2013-12-03
 */

#ifndef LOGGING_H_W3JKC6U1
#define LOGGING_H_W3JKC6U1

#include <string>
#include <chrono>
#include <memory>
#include <fstream>
#include <sstream>
#include <stack>

class Logging;
class Logger;

typedef std::unique_ptr<Logger> UnqLogger;

// Logger classes can only be created by a Logging class
// All Logger classes created by the same Logging class share the same
// file descriptor
class Logger
{
public:
    ~Logger ();

    void error(std::string const & msg);
    void warn(std::string const & msg);
    void info(std::string const & msg);

    // Start measurring time. This function call puts the time on a stack and
    // the call can be nested
    void beginTimeMeasure();
    void endTimeMeasure(std::string const & msg);

private:
    friend class Logging;
    typedef std::chrono::time_point<std::chrono::high_resolution_clock,
                                    std::chrono::microseconds> TimePoint;

    // Disable CTor, Copy-CTor and Move-CTor
    Logger (int fd, std::string const & section);
    Logger ();
    Logger (Logger &);
    Logger (Logger const &);
    Logger (Logger &&);
    Logger (Logger const &&);

    void writeLine(std::string const & msg);

    int mFD;
    std::string mSection;                     // Prefix in the log file
    std::stack<TimePoint> mTimePointStack;    // Stack for the measurement times
};


class Logging
{
public:
    Logging(std::string const & filename);

    void flush();
    Logger * createLogger(std::string const & section);

    ~Logging();

private:
    int mFD; // file descriptor
    void *mMappedMem;
    size_t mMappedSize;
};


#endif /* end of include guard: LOGGING_H_W3JKC6U1 */


