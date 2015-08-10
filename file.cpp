#include <cerrno>

#include "file.hpp"
#include "error.hpp"

File::File(const File& f) : fd{dup(f.fd)}
{
	if(fd == -1)
	{
		int errnum = errno;
		switch(errnum)
		{
			case EBADF:  throw invalid_fd{};
			case EMFILE: throw insufficient_fd_process{};
		}
	}
}

File::File(const char* path, flag flags) : fd(::open(path, flags))
{
	if(fd == -1)
	{
		switch(errno)
		{
			case EACCES:  throw permission_denied{};
			case EEXIST:  throw file_exists{};
			case EINTR:   throw signal_caught{};
			case EINVAL:  throw invalid_flag{};
			case EIO:     throw stream_error{};
			case EISDIR:  throw is_dir{};
			case ELOOP:   throw sym_loop{};
			case EMFILE:  throw insufficient_fd_process{};
			case ENAMETOOLONG: throw name_too_long{};
			case ENFILE:  throw insufficient_fd_system{};
			case ENOENT:  throw not_found{};
			case ENOSR:   throw bad_stream_alloc{};
			case ENOSPC:  throw no_space{};
			case ENOTDIR: throw not_dir{};
			case ENXIO:   throw would_block{};
			case EOVERFLOW: throw too_large{};
			case EROFS:   throw read_only_fs{};
		}
	}
}

[[noreturn]] static void
read_error_handling(int errnum)
{
	switch(errnum)
	{
#if EAGAIN == EWOULDBLOCK
		case EAGAIN: throw would_block{};
#else
		case EAGAIN: case EWOULDBLOCK: throw would_block{};
#endif
		case EBADF:  throw invalid_fd{};
		case EFAULT: throw invalid_buffer{};
		case EINTR:  throw signal_caught{};
		case EINVAL: throw invalid_argument{};
		case EIO:    throw io_error{};
		case EISDIR: throw is_dir{};
		default:     std::terminate();
	}
}

size_t File::read(void* buf, size_t count)
{
	ssize_t read_bytes = ::read(fd, buf, count);
	if(read_bytes == -1)
	{
		read_error_handling(errno);
	}
	return (size_t) read_bytes;
}
std::string File::to_string()
{
	std::string output;
	char buffer[256]; 
	ssize_t bytes_read;

	do
	{
		bytes_read = ::read(fd, buffer, 256);
		if(bytes_read == -1)
		{
			read_error_handling(errno);
		}
		output += std::string{buffer, (size_t)bytes_read};
	} while(bytes_read == 256);

	return output;
}

std::make_unsigned_t<off_t> File::size()
{
	struct stat stats;
	if(fstat(fd, &stats) == -1)
	{
		switch(errno)
		{
			case EBADF: throw invalid_fd{};
		  case EIO:   throw io_error{};
			case EOVERFLOW: throw too_large{};
		}
	}
	return static_cast<std::make_unsigned_t<off_t>>(stats.st_size);
}
