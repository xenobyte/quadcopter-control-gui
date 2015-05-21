
/**
 * @author Alexander Preisinger
 * @date   2013-11-14
 */

// C++
#include <iostream>

// C system header (socket, types)
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "SocketCommunication.h"
#include "TCP_TestConnection.h"

// don't send big packets
#define PACKET_SIZE 128

SocketBase::SocketBase() :
    mSockfd(-1),
    SOCK_LOG(LOG.createLogger("Socket"))
{
}

SocketBase::~SocketBase()
{
    close();
}

void SocketBase::close(void)
{
    if (is_valid()) {
        ::close(mSockfd);
        mSockfd = -1;
    }
}

bool SocketBase::is_blocking(void) const
{
    if (is_valid())
        return !!(fcntl(mSockfd, F_GETFL, 0) & O_NONBLOCK);
    else
        return false;
}

bool SocketBase::set_blocking(bool blocking)
{
    if (!is_valid())
        return false;

    int flags = fcntl(mSockfd, F_GETFL, 0);

    if (blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;

    if (flags < 0 || fcntl(mSockfd, F_SETFL, flags) < 0) {
        SOCK_LOG->error("fcntl error (set_blocking)");
        return false;
    }
    else {
        return true;
    }
}

bool SocketBase::is_valid(void) const
{
    return mSockfd != -1;
}



ServerSocket::ServerSocket()
{
    errno = 0;
    mSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mSockfd == -1) {
        SOCK_LOG->error(std::string("ServeSocket: ") + strerror(errno));
    }
    int yes = 1;
    if (setsockopt(mSockfd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) {
        SOCK_LOG->error("ServerSocket: can't set nodelay option on port");
    }
}

bool ServerSocket::bind(std::int16_t port) const
{
    struct sockaddr_in serv_addr = {
        .sin_family      = AF_INET,
        // apparently htons returns uint32_t which it shouldn't
        .sin_port        = static_cast<in_port_t>(htons(port)),
        .sin_addr = {
            .s_addr      = INADDR_ANY,
        },
        .sin_zero        = {0}
    };

    int yes = 1;
    // Reuse port and don't fail on bind
    if (setsockopt(mSockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        SOCK_LOG->error("ServerSocket::bind: can't set reuse option on port: "
                         + std::to_string(int(port)));
        return false;
    }

    if (::bind(mSockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        SOCK_LOG->error(std::string("ServerSocket::bind: can't bind port: ")
                         + std::to_string(port));
        return false;
    }
    else
        return true;
}

bool ServerSocket::listen(std::int32_t backlog) const
{
    if (::listen(mSockfd, backlog) == -1) {
        SOCK_LOG->error(std::string("ServerSocket::listen: backlog ")
                        + std::to_string(backlog));
        return false;
    }

    return true;
}

ClientSocket* ServerSocket::accept(void) const
{
    errno = 0;
    struct sockaddr_in client_addr;
    std::uint32_t client_addr_size = sizeof(client_addr);
    int client_fd = ::accept(mSockfd,
                             (struct sockaddr *)&client_addr,
                             &client_addr_size);

    if (client_fd != -1) {
        SOCK_LOG->info("client connected: " +
                       std::string(inet_ntoa(client_addr.sin_addr)));

        int yes = 1;
        if (setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) {
            SOCK_LOG->error("ServerSocket::accept: can't set nodelay option on port:");
            ::close(client_fd);
            return NULL;
        }
        return new ClientSocket(client_fd);
    }
    else {
        if (!(errno & (EAGAIN | EWOULDBLOCK)))
            SOCK_LOG->error(std::string("ServerSocket::accept: ") + strerror(errno));

        return NULL;
    }
}

ClientSocket * ServerSocket::connect(std::string address, std::int16_t port) const
{
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port   = static_cast<in_port_t>(htons(port)),
        .sin_addr = {
            .s_addr = inet_addr(address.c_str()),
        },
        .sin_zero   = {0}
    };

    errno = 0;
    int client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (client_fd < 0) {
        SOCK_LOG->error(std::string("ServerSocket::connect: ") + strerror(errno));
        return NULL;
    }

    int ret = ::connect(client_fd,
                        (struct sockaddr *) &server_addr,
                        sizeof(server_addr));

    if (ret < 0) {
        SOCK_LOG->error(std::string("ServerSocket::connect: ") + strerror(errno));
        ::close(client_fd);
        return NULL;
    }

    return new ClientSocket(client_fd);
}

ClientSocket::ClientSocket(int fd)
{
    mSockfd = fd;
}

TReadStatus ClientSocket::write(std::uint8_t const * const data, std::uint32_t len) const
{
    if (!is_valid())
        return eFailed;

    errno = 0;

    const size_t send_const = PACKET_SIZE;

    size_t bytesSend = 0;
    size_t bytesToSend = len;
    int err;
    while (bytesToSend != 0) {
        err = send(mSockfd, &data[bytesSend],
                   (send_const > bytesToSend ? bytesToSend : send_const),
                   MSG_NOSIGNAL);
        if (err == -1) {
            if (!!(errno & EAGAIN) || !!(errno & EWOULDBLOCK)) {
                SOCK_LOG->warn(std::string("ClientSocket::write: ") +
                               strerror(errno));
                return eWouldBlock;
            }
            else {
                SOCK_LOG->error(std::string("ClientSocket::write: ") +
                                strerror(errno));
                return eFailed;
            }
        }
        else if (err == 0) {
            SOCK_LOG->error(std::string("ClientSocket::write: ") +
                            "Error");
            return eFailed;
        }
        bytesSend += err;
        bytesToSend -= err;
    }
    return eSuccess;
}

TReadStatus ClientSocket::read(std::uint8_t * const data, std::uint32_t len) const
{
    if (!is_valid())
        return eFailed;

    errno = 0;

	memset(data,0,len);
    const size_t read_const = PACKET_SIZE;

    int bytesRead = 0;
    int bytesToRead = len;
    int err;
    //while (bytesToRead > 0) {
        err = recv(mSockfd, &data[bytesRead],
                   (bytesToRead > read_const ? bytesToRead : read_const),
                   MSG_NOSIGNAL);
        //std::cout << "err: " << err << std::endl;
        //std::cout << data[bytesRead] << std::endl;
        //std::cout << "bytes to Read: " << bytesToRead << std::endl;

        if (err == -1) {
            if ((errno & EAGAIN) || (errno & (EWOULDBLOCK))) {
                return eWouldBlock;
            }
            else {
                SOCK_LOG->error(std::string("ClientSocket::read: ") +
                                strerror(errno));
                return eFailed;
            }
        }
        else if (err == 0) {
            return eFailed;
        }
        bytesRead += err;
        bytesToRead -= err;
    //}
    return eSuccess;
}
