#include "Socket.hpp"
#include <iostream>
#include "error.hpp"

int main()
{
    try
    {
        Socket sock1{PF_INET, SOCK_STREAM};
        sock1.setsockopt(SOL_SOCKET, SO_REUSEADDR);
        sock1.bind(Ipv4{INADDR_ANY, 1000});
        sock1.listen(20);

        Socket sock2{PF_INET, SOCK_STREAM};

        sock2.connect(Ipv4{"127.0.0.1", 1000});
        sock2.write("This is a test\n\r");

        Connection conn = sock1.accept();

        Socket sock = std::move(conn.first);

        std::string line =  sock.receive_line("\n\r");
        std::cout << line;
    } catch(socket_error& e)
    {
        std::cerr << e.what() << '\n';
        std::cout << strerror(e.err_num()) << '\n';
    }
}
