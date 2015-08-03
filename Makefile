all: libsocxx.a

libsocxx.a: Socket.o ip.o File.o
	ar cru libsocxx.a Socket.o ip.o File.o
	ranlib libsocxx.a

Socket.o: Socket.cpp Socket.hpp.gch
	g++ Socket.cpp -o Socket.o -std=c++1y -Wall -Werror -Weffc++ --pedantic -c -g
ip.o: ip.cpp Socket.hpp.gch
	g++ ip.cpp -o ip.o -std=c++1y -Wall -Werror -Weffc++ --pedantic -c -g

Socket.hpp.gch: Socket.hpp
	g++ Socket.hpp -O3 -std=c++1y -Wall -Werror -Weffc++ --pedantic -g


File.o: File.cpp File.hpp.gch
	g++ File.cpp -O3 -c -std=c++1y -Wall -Werror -Weffc++ --pedantic -g 

File.hpp.gch: File.hpp
	g++ File.hpp -O3 -c -std=c++1y -Wall -Werror -Weffc++ --pedantic -g

.PHONY: clean

clean:
	rm *.o -f
	rm *.a -f
	rm *.gch -f

