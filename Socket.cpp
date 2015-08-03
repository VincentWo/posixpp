#include <cassert>
#include <errno.h>

#include "Socket.hpp"
#include "error.hpp"

Socket::Socket(int domain, int type, int protocol)
              : file_desc{socket(domain, type, protocol)}
{
    if(file_desc < 0)
    {
         throw socket_error{"ERROR opening socket", errno};
    }
}

void Socket::bind(const Ipv4& address)
{
    if(::bind(file_desc,
                 (const sockaddr *) &address.get_raw_struct(),
                 sizeof(address))
       < 0)
    {
        throw socket_error{"ERROR on binding", errno};
    }
}

void Socket::listen(int times_to_try)
{
    if(::listen(file_desc, times_to_try) < 0)
    {
        throw socket_error{"ERROR on listening", errno};
    }
}

std::pair<Socket, Ipv4> Socket::accept()
{
    sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    int new_file_desc = ::accept(file_desc,
                                 (sockaddr *)&cli_addr,
                                 &clilen);

    if(new_file_desc < 0)
    {
        throw socket_error{"ERROR on accept", errno};
    }
    return std::make_pair(Socket{new_file_desc}, Ipv4{cli_addr});
}

std::string Socket::read()
{
    char buffer[256]{};
    int n = ::read(file_desc, buffer, 255);

    if(n < 0)
    {
        throw socket_error{"ERROR on read", errno};
    }
    else if(!n)
    {
        //TODO: Find out what this means
        assert("This should never happen");
        std::terminate();
    }
    buffer[n] = '\0';

    return std::string{buffer};
}

void Socket::setsockopt(int level, int optname)
{
    int _true = 1;
    if(::setsockopt(file_desc, level, optname, &_true, sizeof(_true)) == -1)
    {
        throw socket_error("ERORR on setsockopt", errno);
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

ssize_t Socket::write(const void* buf, size_t count)
{
    ssize_t bytes_written = ::write(file_desc, buf, count);
    if(bytes_written == -1)
    {
        throw socket_error{"Error on write", errno};
    }
    return bytes_written;
}

void Socket::shutdown(int how)
{
    if(::shutdown(file_desc, how) == -1)
    {
        throw socket_error{"Error on shutdown", errno};
    }
}

ssize_t Socket::receive(void* buffer, size_t buf_size, int flags)
{
    ssize_t size = ::recv(file_desc, buffer, buf_size, flags);
    if(size == 0)
    {
        throw socket_error{"Connection closed", 0};
    }
    if(size == -1)
    {
        throw socket_error{"Error on receive", errno};
    }
    return size;
}

void Socket::connect(Ipv4 address)
{
    if(::connect(file_desc,
                 (const sockaddr*) &address.get_raw_struct(),
                 sizeof(sockaddr_in)) == -1)
    {
        throw socket_error{"Error on connect", errno};
    }
}

ssize_t Socket::send(const void* buf, size_t len, int flags)
{
    ssize_t send_bytes = ::send(file_desc, buf, len, flags);
    if(send_bytes == -1)
    {
        throw socket_error{"send", errno};
    }
    return send_bytes;
}
