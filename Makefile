CXX=g++
CC=gcc
CXXFLAGS= -std=c++1y -Werror -pedantic -Wall -Wextra -Wmissing-include-dirs -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -Wfloat-equal -Wshadow -Wcast-qual -Wcast-align -Wconversion -Wzero-as-null-pointer-constant -Wuseless-cast -Wsign-conversion -Wsized-deallocation -Wlogical-op -Wno-aggressive-loop-optimizations -Wredundant-decls -Winline -Winvalid-pch -Wdisabled-optimization -Wundef
CPPFLAGS= -I /home/vincent/coding  -I /home/vincent/coding/web_server_posixpp
all: libsocxx.a

libsocxx.a: socket.o ip.o file.o socket.hpp.gch
	ar cru libsocxx.a socket.o ip.o file.o
	ranlib libsocxx.a

file.o: file.cpp file.hpp.gch

file.hpp.gch: file.hpp

socket.hpp.gch: socket.hpp
	$(CXX) socket.hpp $(CXXFLAGS)
file.hpp.gch: file.hpp
	$(CXX) file.hpp $(CXXFLAGS)
.PHONY: clean

clean:
	rm *.o -f
	rm *.a -f
	rm *.gch -f
