#include <cassert>
#include <errno.h>

#include "socket.hpp"
#include "error.hpp"

Socket::Socket(int domain, int type, int protocol)
              : File{socket(domain, type, protocol)}
{
    if(fd < 0)
    {
        switch(errno)
        {
            case EAFNOSUPPORT:    throw af_not_implemented{};
            case EPROTONOSUPPORT: throw proto_not_supported{};

            case EPROTOTYPE:      throw sock_not_supported{};

            case EMFILE:          throw insufficient_fd_system{};
            case ENFILE:          throw insufficient_fd_process{};

            case EACCES:          throw access_denied{};

            case ENOMEM:          throw insufficient_memory{};
            case ENOBUFS:         throw insufficient_resources{};
        }
    }
}

void Socket::bind(const Ipv4& address)
{
    if(::bind(fd,
                 (const sockaddr *) &address.get_raw_struct(),
                 sizeof(address))
       < 0)
    {
        switch(errno)
        {
            case EADDRINUSE:   throw addr_in_use{};
            case EADDRNOTAVAIL:throw addr_not_available{};
            case EAFNOSUPPORT: throw invalid_addr{};
            case EALREADY:     throw already_in_progress{};
            case EBADF:        throw invalid_fd{};
            case EINPROGRESS:  throw in_progress{};
            case EINVAL:       throw invalid_socket{};
            case ENOBUFS:      throw insufficient_resources{};
            case ENOTSOCK:     throw no_socket{};
            case EOPNOTSUPP:   throw bind_not_supported{};
            case EACCES:       throw access_denied{};
            case EDESTADDRREQ:
            case EISDIR:       throw invalid_argument{};
            case EIO:          throw io_error{};
            case ELOOP:        throw sym_loop{};
            case ENAMETOOLONG: throw name_too_long{};
            case ENOENT:       throw not_found{};
            case ENOTDIR:      throw not_dir{};
            case EROFS:        throw read_only_fs{};
            case EISCONN:      throw already_connected{};
        }
    }
}

void Socket::listen(int times_to_try)
{
    if(::listen(fd, times_to_try) < 0)
    {
        switch(errno)
        {
            case EBADF:        throw invalid_fd{};
            case EDESTADDRREQ: throw unbound_socket{};
            case EINVAL:       throw already_connected{};
            case ENOTSOCK:     throw no_socket{};
            case EOPNOTSUPP:   throw listen_not_supported{};
            case EACCES:       throw access_denied{};
            case ENOBUFS:      throw insufficient_buffer{};
        }
    }
}

std::pair<Socket, Ipv4> Socket::accept()
{
    sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    int new_fd = ::accept(fd,
                                 (sockaddr *)&cli_addr,
                                 &clilen);

    if(new_fd < 0)
    {
        switch(errno)
        {
#if EAGAIN != EWOULDBLOCK
            case EAGAIN:
#endif
            case EWOULDBLOCK: throw would_block{};
            case EBADF:       throw invalid_fd{};
            case ECONNABORTED:throw conn_aborted{};
            case EINTR:       throw signal_caught{};
            case EINVAL:      throw no_accept{};
            case EMFILE:      throw insufficient_fd_process{};
            case ENFILE:      throw insufficient_fd_system{};
            case ENOBUFS:     throw insufficient_buffer{};
            case ENOMEM:      throw insufficient_memory{};
            case ENOTSOCK:    throw no_socket{};
            case EOPNOTSUPP:  throw accept_not_supported{};
            case EPROTO:      throw protocol_error{};
        }
    }
    return std::make_pair(Socket{new_fd}, Ipv4{cli_addr});
}

void Socket::setsockopt(int level, int optname)
{
    int _true = 1;
    if(::setsockopt(fd, level, optname, &_true, sizeof(_true)) == -1)
    {
        switch(errno)
        {
            case EBADF:       throw invalid_fd{};
            case EDOM:        throw value_too_big{};
            case EINVAL:      throw invalid_option{};
            case EISCONN:     throw is_connected{};
            case ENOPROTOOPT: throw opt_not_supported{};
            case ENOTSOCK:    throw no_socket{};
        }
    }
}

std::string Socket::receive_line(const std::string& eol)
{
    std::string buffer{};
    
    std::string::size_type eol_matched = 0;
    std::string::size_type eol_size = eol.size();
    while(true)
    {
        char c = receive<char>();
        buffer += c;
        if(c == eol[eol_matched])
        {
            ++eol_matched;    
            if(eol_matched == eol_size)
            {
                buffer.erase(buffer.size() - eol_size);
                return buffer;
            }
        }
        else
        {
            eol_matched = 0;
        }
    }
}

void Socket::shutdown(int how)
{
    if(::shutdown(fd, how) == -1)
    {
        switch(errno)
        {
            case EBADF:    throw invalid_fd{};
            case EINVAL:   throw invalid_argument{};
            case ENOTCONN: throw not_connected{};
            case ENOTSOCK: throw no_socket{};
        }
    }
}

size_t Socket::receive(void* buffer, size_t buf_size, int flags)
{
    ssize_t size = ::recv(fd, buffer, buf_size, flags);
    if(size == 0)
    {
        throw already_shutdowned{};
    }
    if(size == -1)
    {
        switch(errno)
        {
#if EAGAIN != EWOULDBLOCK
            case EAGAIN:
#endif
            case EWOULDBLOCK: throw would_block{};
            case EBADF:       throw invalid_fd{};
            case ECONNRESET:  throw conn_closed{};
            case EINTR:       throw signal_caught{};
            case EINVAL:      throw no_oob_data{};
            case ENOTCONN:    throw not_connected{};
            case ENOTSOCK:    throw no_socket{};
            case EOPNOTSUPP:  throw invalid_flag{};
            case ETIMEDOUT:   throw timeout{};
            case EIO:         throw io_error{};
            case ENOBUFS:     throw insufficient_buffer{};
            case ENOMEM:      throw insufficient_memory{};
        }
    }
    return (size_t) size;
}

void Socket::connect(Ipv4 address)
{
    if(::connect(fd,
                 (const sockaddr*) &address.get_raw_struct(),
                 sizeof(sockaddr_in)) == -1)
    {
        switch(errno)
        {
            case EADDRNOTAVAIL: throw addr_not_available{};
            case EAFNOSUPPORT:  throw invalid_addr{};
            case EALREADY:      throw already_in_progress{};
            case EBADF:         throw invalid_fd{};
            case ECONNREFUSED:  throw conn_refused{};
            case EINPROGRESS:   throw in_progress{};
            case EINTR:         throw signal_caught{};
            case EISCONN:       throw already_connected{};
            case ENETUNREACH:   throw no_network{};
            case ENOTSOCK:      throw no_socket{};
            case EPROTOTYPE:    throw wrong_addr_type{};
            case ETIMEDOUT:     throw timeout{};
            case EIO:           throw io_error{};
            case ELOOP:         throw sym_loop{};
            case ENAMETOOLONG:  throw name_too_long{};
            case ENOENT:        throw not_found{};
            case ENOTDIR:       throw not_dir{};
            case EACCES:        throw access_denied{};
            case EADDRINUSE:    throw addr_in_use{};
            case ECONNRESET:    throw conn_closed{};
            case EHOSTUNREACH:  throw host_unreachable{};
            case EINVAL:        throw invalid_addr_length{};
            case ENETDOWN:      throw net_down{};
            case ENOBUFS:       throw insufficient_buffer{};
            case EOPNOTSUPP:    throw is_listening{};
        }
    }
}

size_t Socket::send(const void* buf, size_t len, int flags)
{
    ssize_t send_bytes = ::send(fd, buf, len, flags);
    if(send_bytes == -1)
    {
        switch(errno)
        {
#if EAGAIN != EWOULDBLOCK
            case EAGAIN:
#endif
            case EWOULDBLOCK:  throw would_block{};
            case EBADF:        throw invalid_fd{};
            case ECONNRESET:   throw conn_closed{};
            case EDESTADDRREQ: throw no_addr{};
            case EINTR:        throw signal_caught{};
            case EMSGSIZE:     throw msg_too_large{};
            case ENOTCONN:     throw not_connected{};
            case ENOTSOCK:     throw no_socket{};
            case EOPNOTSUPP:   throw invalid_flag{};
            case EPIPE:        throw invalid_socket{};
            case EACCES:       throw access_denied{};
            case EIO:          throw io_error{};
            case ENETDOWN:     throw net_down{};
            case ENETUNREACH:  throw no_network{};
            case ENOBUFS:      throw insufficient_buffer{};
        }
    }
    return (size_t) send_bytes;
}
