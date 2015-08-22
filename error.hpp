#ifndef SOCXX_H_INCLUDE
#define SOCXX_H_INCLUDE
#include <cstring>
#include <stdexcept>

struct error : std::exception
{
    virtual const char*
    what() const noexcept override = 0;
};

struct file_error   : error
{};

struct not_found : file_error
{
	virtual const char*
    what() const noexcept override {return "resource not found";}
};

struct access_denied : file_error
{
	virtual const char*
    what() const noexcept override {return "access denied";}
};

struct invalid_fd : file_error
{
	virtual const char*
    what() const noexcept override {return "invalid fd";}
};

struct insufficient_fd : file_error
{};

struct insufficient_fd_process : insufficient_fd
{
	virtual const char*
    what() const noexcept override {return "too many open fds for the process";}
};
struct insufficient_fd_system : insufficient_fd
{
	virtual const char*
    what() const noexcept override {return "too many open fds system-wide";}
};
struct file_exists : file_error
{
	virtual const char*
    what() const noexcept override {return "file already exists";}
};
struct signal_caught : file_error
{
	virtual const char*
    what() const noexcept override {return "file already exists";}
};
struct invalid_flag : file_error
{
	virtual const char*
    what() const noexcept override {return "invalid flag";}
};
struct stream_error : file_error
{
	virtual const char*
    what() const noexcept override {return "stream error";}
};
struct is_dir : file_error
{
	virtual const char*
    what() const noexcept override {return "is a directory";}
};
struct sym_loop : file_error
{
	virtual const char*
    what() const noexcept override {return "symbolic link loop or O_NOFOLLOW";}
};
struct name_too_long : file_error
{
	virtual const char*
    what() const noexcept override {return "file name too long";}
};
struct bad_stream_alloc : file_error
{
	virtual const char*
    what() const noexcept override {return "couldn't allocate stream resources";}
};
struct no_space : file_error
{
	virtual const char*
    what() const noexcept override {return "no space on device";}
};
struct not_dir : file_error
{
	virtual const char*
    what() const noexcept override {return "not a directory";}
};
struct would_block : file_error
{
	virtual const char*
    what() const noexcept override {return "action would block";}
};
struct too_large : file_error
{
	virtual const char*
    what() const noexcept override {return "file-size cant be represented";}
};
struct read_only_fs : file_error
{
	virtual const char*
    what() const noexcept override {return "tried to write on a read-only fs";}
};
struct invalid_buffer : file_error
{
	virtual const char*
    what() const noexcept override {return "buffer address out of address space";}
};
struct invalid_argument : file_error
{
	virtual const char*
    what() const noexcept override {return "invalid argument";}
};
struct io_error : file_error
{
	virtual const char*
    what() const noexcept override {return "io error ";}
};
struct socket_error : error
{};
struct no_socket : socket_error
{
    virtual const char*
    what() const noexcept override {return "Not a socket.";};
};

struct already_connected : socket_error
{
    virtual const char*
    what() const noexcept override {return "Socket already connected.";};
};

struct bind_not_supported : socket_error
{
    virtual const char*
    what() const noexcept override {return "Binding not supported.";};
};

struct af_not_implemented : socket_error
{
    virtual const char*
    what() const noexcept override {return "Address Family not implemented.";};
};

struct insufficient_resources : error
{
    virtual const char*
    what() const noexcept override
           {return "Insufficient resources to complete the task.";};
};

struct insufficient_buffer : error
{
    virtual const char*
    what() const noexcept override
           {return "Insufficient buffer to complete the task.";};
};

struct invalid_socket : socket_error
{
    virtual const char*
    what() const noexcept override {return "Socket is in an invalid state.";};
};

struct proto_not_supported : socket_error
{
    virtual const char*
    what() const noexcept override {return "Protocol not supported.";};
};

struct sock_not_supported : socket_error
{
    virtual const char*
    what() const noexcept override
           {return "The protocol doesn't support the socket type.";};
};

struct insufficient_memory : error
{
    virtual const char*
    what() const noexcept override { return "Not enough memory.";};
};

struct addr_in_use : socket_error
{
    virtual const char*
    what() const noexcept override {return "Address already in use.";};
};

struct addr_not_available : socket_error
{
    virtual const char*
    what() const noexcept override {return "Address not available.";};
};

struct invalid_addr : socket_error
{
    virtual const char*
    what() const noexcept override {return "Address invalid.";};
};

struct already_in_progress : socket_error
{
    virtual const char*
    what() const noexcept override{return "A binding is already in progress.";};
};

struct in_progress : socket_error
{
    virtual const char*
    what() const noexcept override {return "Performs asynchronously.";};
};

struct protocol_error : socket_error
{
    virtual const char*
    what() const noexcept override {return "A protocol-level error occured.";};
};

struct accept_not_supported : socket_error
{
    virtual const char*
    what() const noexcept override {return "Accept isnt supported.";};
};

struct no_accept : socket_error
{
    virtual const char*
    what() const noexcept override
    {return "Does not accept connections at the moment";};
};

struct conn_aborted : socket_error
{
    virtual const char*
    what() const noexcept override {return "Connection was aborted.";};
};

struct unbound_socket : socket_error
{
    virtual const char*
    what() const noexcept override {return "Socket is not bound.";};
};

struct listen_not_supported : socket_error
{
    virtual const char*
    what() const noexcept override {return "Listen is not supported.";};
};

struct invalid_size : socket_error
{
    virtual const char*
    what() const noexcept override {return "Invalid size.";};
};

struct value_too_big : socket_error
{
    virtual const char*
    what() const noexcept override
    {return "Value too big to fit in the structure";};
};

struct invalid_option : socket_error
{
    virtual const char*
    what() const noexcept override {return "Option invalid.";};
};

struct opt_not_supported : socket_error
{
    virtual const char*
    what() const noexcept override {return "Option not supported.";};
};

struct is_connected : socket_error
{
    virtual const char*
    what() const noexcept override
    {return "cant change option while connected.";};
};

struct no_network : socket_error
{
    virtual const char*
    what() const noexcept override { return "No network connection.";};
};

struct net_down : socket_error
{
    virtual const char*
    what() const noexcept override { return "Network is down";};
};

struct not_connected : socket_error
{
    virtual const char*
    what() const noexcept override { return "Socket isnt connected";};
};

struct msg_too_large : socket_error
{
    virtual const char*
    what() const noexcept override
    { return "Message is too large to be sent in one package.";};
};
struct no_addr : socket_error
{
    virtual const char*
    what() const noexcept override { return "Need a destination address.";};
};


struct conn_closed : socket_error
{
    virtual const char*
    what() const noexcept override{ return "The connection is already closed";};
};

struct is_listening : socket_error
{
    virtual const char*
    what() const noexcept override
    { return "The socket is listening at the moment";};
};

struct invalid_addr_length: socket_error
{
    virtual const char*
    what() const noexcept override { return "Invalid address length.";};
};

struct host_unreachable: socket_error
{
    virtual const char*
    what() const noexcept override { return "The host is unreachable.";};
};

struct timeout : socket_error
{
    virtual const char*
    what() const noexcept override { return "Connection timed out.";};
};
struct already_shutdowned : socket_error
{
    virtual const char*
    what() const noexcept override{ return "The socket is already shutdowned";};
};
struct no_oob_data : socket_error
{
    virtual const char*
    what() const noexcept override { return "No out of band data available";};
};
struct conn_refused : socket_error
{
    virtual const char*
    what() const noexcept override { return "The connection was refused.";};
};
struct wrong_addr_type : socket_error
{
    virtual const char*
    what() const noexcept override 
    { return "The given address has the wrong type.";};
};
#endif /*SOCXX_H_INCLUDE */
