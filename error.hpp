#ifndef SOCXX_H_INCLUDE
#define SOCXX_H_INCLUDE
#include <cstring>
#include <stdexcept>

struct error : std::exception
{
	virtual const char* what() const noexcept override = 0;
};

struct file_error   : error
{
	virtual const char* what() const noexcept override = 0;
};

struct not_found : file_error
{
	virtual const char* what() const noexcept override {return "resource not found";}
};

struct permission_denied : file_error
{
	virtual const char* what() const noexcept override {return "permission denied";}
};

struct invalid_fd : file_error
{
	virtual const char* what() const noexcept override {return "invalid fd";}
};
struct insufficient_fd : file_error
{
	virtual const char* what() const noexcept override = 0;
};
struct insufficient_fd_process : insufficient_fd
{
	virtual const char* what() const noexcept override {return "too many open fds for the process";}
};
struct insufficient_fd_system : insufficient_fd
{
	virtual const char* what() const noexcept override {return "too many open fds system-wide";}
};
struct file_exists : file_error
{
	virtual const char* what() const noexcept override {return "file already exists";}
};
struct signal_caught : file_error
{
	virtual const char* what() const noexcept override {return "file already exists";}
};
struct invalid_flag : file_error
{
	virtual const char* what() const noexcept override {return "invalid flag";}
};
struct stream_error : file_error
{
	virtual const char* what() const noexcept override {return "stream error";}
};
struct is_dir : file_error
{
	virtual const char* what() const noexcept override {return "is a directory";}
};
struct sym_loop : file_error
{
	virtual const char* what() const noexcept override {return "symbolic link loop or O_NOFOLLOW";}
};
struct name_too_long : file_error
{
	virtual const char* what() const noexcept override {return "file name too long";}
};
struct bad_stream_alloc : file_error
{
	virtual const char* what() const noexcept override {return "couldn't allocate stream resources";}
};
struct no_space : file_error
{
	virtual const char* what() const noexcept override {return "no space on device";}
};
struct not_dir : file_error
{
	virtual const char* what() const noexcept override {return "not a directory";}
};
struct would_block : file_error
{
	virtual const char* what() const noexcept override {return "action would block";}
};
struct too_large : file_error
{
	virtual const char* what() const noexcept override {return "file-size cant be represented";}
};
struct read_only_fs : file_error
{
	virtual const char* what() const noexcept override {return "tried to write on a read-only fs";}
};
struct invalid_buffer : file_error
{
	virtual const char* what() const noexcept override {return "buffer address out of address space";}
};
struct invalid_argument : file_error
{
	virtual const char* what() const noexcept override {return "invalid argument";}
};
struct io_error : file_error
{
	virtual const char* what() const noexcept override {return "io error ";}
};
struct socket_error : error
{
	virtual const char* what() const noexcept override = 0;
};
#undef ERROR_PARENT_CTR
#endif /*SOCXX_H_INCLUDE */
