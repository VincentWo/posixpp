#ifndef SOCXX_SOCKET_HPP_INCLUDE
#define SOCXX_SOCKET_HPP_INCLUDE
#include <sys/socket.h>
#include <sys/types.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include <map>
#include <cstdint>
#include <iostream>
#include <type_traits>

#include "File.hpp"
enum class Domain {
};

class Ipv4
{
public:
    Ipv4(uint32_t ip, short port);
    Ipv4(std::string ip, short port);
    Ipv4(short port);

    constexpr Ipv4(const sockaddr_in& address) : address{address}{}

    const sockaddr_in& get_raw_struct() const {return address;}
    sockaddr_in& get_raw_struct()             {return address;}
private:
    sockaddr_in address;
};

class Socket;
typedef std::pair<Socket, Ipv4> Connection;

class Socket
{
public:
    Socket(Domain domain, int type, int protocol = 0)
          : Socket{static_cast<int>(domain),
                   static_cast<int>(type),
                   static_cast<int>(protocol)} {}
    Socket(int domain, int type, int protocol = 0);
    explicit Socket(int fd) noexcept : file_desc(fd) {}

    void bind(const sockaddr_in& address);
    void bind(const Ipv4&);

    void listen(int times_to_try);
    Connection  accept();

    std::string read();

    ssize_t write(const std::string& str)
    {
        return write(&str[0], str.size());
    }
    ssize_t write(const char* str)
    {
        return write(str, std::strlen(str));
    }
    ssize_t write(const void*, size_t);

    ssize_t send(const std::string& msg, int flags = 0)
    {
        return send(&msg[0], msg.size(), flags);
    }
    ssize_t send(const char* msg, int flags = 0)
    {
        return send(msg, std::strlen(msg), flags);
    }
    ssize_t send(const void*, size_t, int flags = 0);
    
    ssize_t receive(void*, size_t, int flags = 0);

    template<typename ret_type> ret_type receive();
    template<typename ret_type> ret_type& receive(ret_type&, size_t count);

    std::string receive_line(const std::string& eol = "\r\n");

    void setsockopt(int level, int optname);

    void shutdown(int how = SHUT_RDWR);

    void connect(Ipv4);

    const Socket& operator=(const Socket&);
    const Socket& operator=(Socket&&);
    Socket(const Socket&);
    Socket(Socket&& mv_from) : file_desc(mv_from.file_desc)
    {
        mv_from.file_desc = -1;
    }
    ~Socket() noexcept
    {
        ::shutdown(file_desc, SHUT_RDWR);
        ::close(file_desc);
    }
private:
    int file_desc;
};

std::ostream& operator<<(std::ostream&, const Ipv4&);

template<typename T> inline Socket& operator<<(Socket&, T);
template<>           inline Socket& operator<<(Socket&, const char*);
template<>           inline Socket& operator<<(Socket&, std::string);

template<typename T> inline Socket& operator<<(Socket& lhs, T rhs)
{
    return(lhs << std::to_string(rhs));
}

template<> inline Socket& operator<<(Socket& lhs, const char* rhs)
{
    return operator<<(lhs, std::string{rhs});
}
template<> inline Socket& operator<<(Socket& lhs, std::string rhs)
{
    size_t sent_bytes;
    size_t bytes_to_send = rhs.size();
    const char* buffer = &rhs[0];
    while(bytes_to_send > 0)
    {
        sent_bytes = lhs.send((void*)buffer, bytes_to_send);
        bytes_to_send -= sent_bytes;
        buffer += sent_bytes;
    }
    return lhs;
}

template<> inline Socket& operator<<(Socket& lhs, File rhs)
{
    auto file_size = rhs.size();
    char* buffer = new char[file_size];

    rhs.read((void*)buffer, file_size);
    lhs.send((void*)buffer, file_size);

    delete buffer;
    return lhs;
}

template<typename ret_type> inline ret_type Socket::receive()
{
    static_assert(std::is_integral<ret_type>::value,
                  "you can only receive integral values");
    ret_type ret_val;
    receive(&ret_val, sizeof(ret_type));
    return ret_val;
}
#endif /* SOCXX_SOCKET_HPP_INCLUDE */
