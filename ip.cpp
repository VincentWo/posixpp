#include "socket.hpp"
#include <arpa/inet.h>
#include "error.hpp"

Ipv4::Ipv4(uint32_t ip, uint16_t port)
          : address{AF_INET, hton(port), in_addr{htonl(ip)}, 0}
{}

Ipv4::Ipv4(std::string ip, uint16_t port)
          : address{AF_INET, hton(port), in_addr{0}, 0}
{
    if(!inet_aton(ip.c_str(), &address.sin_addr))
    {
        throw socket_error{"ERROR: " + ip + " is not a valid ip address", 0};
    }
}

Ipv4::Ipv4(uint16_t port) : address{AF_INET, hton(port),in_addr{INADDR_ANY}, 0}
{}

std::ostream& operator<<(std::ostream& lhs, const Ipv4& rhs)
{
    lhs << inet_ntoa(rhs.get_raw_struct().sin_addr)
        << ':'
        << ntoh(rhs.get_raw_struct().sin_port);
    return lhs;
}
