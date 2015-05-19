
/**
 * @author Alexander Preisinger
 * @date   2013-11-14
 */

#ifndef SOCEKTCOMMUNICATION_H_AP3HYVIW
#define SOCEKTCOMMUNICATION_H_AP3HYVIW

#include <cstdint>
#include <memory>
#include "TCP_TestConnection.h"

class ClientSocket;
class ServerSocket;

typedef std::unique_ptr<ClientSocket> UnqClientSocket;

class SocketBase
{
public:
    virtual ~SocketBase();

    // TODO documentation
    void close(void);
    bool is_blocking(void) const;
    bool set_blocking(bool blocking);
    bool is_valid(void) const; // socket fd != -1

protected:
    SocketBase();
    SocketBase(SocketBase &);
    SocketBase(SocketBase const &);
    SocketBase(SocketBase &&);
    SocketBase(SocketBase const &&);
    SocketBase & operator= (SocketBase &);
    SocketBase & operator= (SocketBase const &);

    int mSockfd;
    UnqLogger SOCK_LOG;
};

class ClientSocket : public SocketBase
{
public:

    TReadStatus write(std::uint8_t const * const data, std::uint32_t len) const;
    TReadStatus read(std::uint8_t * const data, std::uint32_t len) const;

private:

    ClientSocket(int fd);
    friend class ServerSocket;

    ClientSocket();
    ClientSocket(ClientSocket &);
    ClientSocket(ClientSocket const &);
    ClientSocket(ClientSocket &&);
    ClientSocket(ClientSocket const &&);
    ClientSocket & operator= (ClientSocket &);
    ClientSocket & operator= (ClientSocket const &);
};


class ServerSocket : public SocketBase
{
public:
    ServerSocket();

    /**
     * @brief bind to a specific port
     */
    bool bind(std::int16_t port) const;

    /**
     * @brief Listen for a number of clients
     *
     * @param  backlog  Indicates the maximum number of incoming connections
     * 
     * @return True if the various operation were successful 
     */
    bool listen(std::int32_t backlog) const;

    /**
     * @brief Accept new client connect and return the client socket
     * 
     * @return ClientSocket or NULL on EWOULDBLOCK or failure
     */
    ClientSocket * accept() const;

    /**
     * @brief Tries to connect to a different Server
     *
     * @return ClientSocket
     */
    ClientSocket * connect(std::string address, std::int16_t port) const;

private:
    ServerSocket(ServerSocket &);
    ServerSocket(ServerSocket const &);
    ServerSocket(ServerSocket &&);
    ServerSocket(ServerSocket const &&);
    ServerSocket & operator= (ServerSocket &);
    ServerSocket & operator= (ServerSocket const &);
};

#endif /* end of include guard: SOCEKTCOMMUNICATION_H_AP3HYVIW */

