#include "../Socket.hpp"

int main()
{
    Socket sock{PF_INET, SOCK_STREAM};

    sock.setsockopt(SOL_SOCKET, SO_REUSEADDR);

    sock.bind(Ipv4{1000});

    sock.listen(20);

    File f{"test.txt", File::RDONLY};

    Connection conn = sock.accept();

    conn.first << "1 Test\r\n";
    sock.send("2 Test\r\n");
    sock.write("3 Test\r\n");

    conn.first << f;
    while(true)
    {
    }
}
