#include <cerrno>

#include "File.hpp"
#include "error.hpp"

File::File(const File& f) : fd(dup(f.fd))
{
    if(fd == -1)
    {
        throw file_error{"dup", errno};
    }
}

File::File(const char* path, flag flags) : fd(::open(path, flags))
{
    if(fd == -1)
    {
        throw file_error{"open", errno};
    }
}

ssize_t File::read(void* buf, size_t count)
{
    ssize_t read_bytes = ::read(fd, buf, count);
    if(read_bytes == -1)
    {
        throw file_error{"on read", errno};
    }

    return read_bytes;
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
            throw file_error{"on read in to_string", errno};
        }
        output += std::string{buffer, (size_t)bytes_read};
    } while(bytes_read == 256);

    return output;
}

off_t File::size()
{
    struct stat stats;
    if(fstat(fd, &stats) == -1)
    {
        throw file_error{"ERROR on size()", errno};
    }
    return stats.st_size;
}
