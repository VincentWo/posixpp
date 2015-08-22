#ifndef SOCXX_FILE_HPP_INCLUDE
#define SOCXX_FILE_HPP_INCLUDE
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <unistd.h>

class File
{
    public:
        enum flag : int;

        explicit File(const std::string& path, flag flags)
            : File(path.c_str(), flags) {}
        explicit File(const char* path, flag flags);
        explicit File(int file_descriptor) : fd{file_descriptor} {}

        File(const File& f);
        File(File&&);

        File& operator=(const File&);
        File& operator=(File&&);

        size_t read(void* buf, size_t count);

        void write(const std::string& str)
        {
            write(&str[0], str.size());
        }
        void write(const std::string& str, size_t count)
        {
            write(&str[0], std::min(str.size(), count));
        }
        void write(const char* str)
        {
            write(str, std::strlen(str));
        }
        void write(const void*, size_t);

        void close();

        void open(const std::string& path, flag flags)
        {open(path.c_str(), flags);}

        void open(const char* path, flag flags);

				std::make_unsigned_t<off_t> size();
        ~File() { ::close(fd);}
        std::string to_string();
        enum flag : int
        {
#ifdef O_EXEC
            EXEC = O_EXEC,
#endif
#ifdef O_RDONLY
            RDONLY = O_RDONLY,
#endif
#ifdef O_RDWR
            RDWR = O_RDWR,
#endif
#ifdef O_SEARCh
            SEARCH = O_SEARCH,
#endif
#ifdef O_WRONLY
            WRONLY = O_WRONLY,
#endif
#ifdef O_APPEND
            APPEND = O_APPEND,
#endif
#ifdef O_CLOEXEC
            CLOEXEC = O_CLOEXEC,
#endif
#ifdef O_CREAT
            CREAT = O_CREAT,
#endif
#ifdef O_DIRECTORY
            DIRECTORY = O_DIRECTORY,
#endif
#ifdef O_DSYNC
            DSYNC = O_DSYNC,
#endif
#ifdef O_EXCL
            EXCL = O_EXCL,
#endif
#ifdef O_NOCTTY
            NOCTTY = O_NOCTTY,
#endif
#ifdef O_NOFOLLOW
            NOFOLLOW = O_NOFOLLOW,
#endif
#ifdef O_NONBLOCK
            NONBLOCK = O_NONBLOCK,
#endif
#ifdef O_RSYNC
            RSYNC = O_RSYNC,
#endif
#ifdef O_SYNC
            SYNC = O_SYNC,
#endif
#ifdef O_TRUNC
            TRUNC = O_TRUNC,
#endif
#ifdef O_TTY_INIT
            TTY_INIT = O_TTY_INIT
#endif
        };

    protected:
        int fd; 
};
#endif /* SOCXX_FILE_HPP_INCLUDE */

