#ifndef SOCXX_H_INCLUDE
#define SOCXX_H_INCLUDE

#include <cstring>
#include <stdexcept>

#define ERROR_PARENT_CTR(child, parent)                      \
            child(int error_number)                          \
            : parent{error_number} {}                        \
                                                             \
            child(const std::string& what_, int error_number)\
            : parent{what_, error_number} {}                 \
                                                             \
            child(const char* what_, int error_number)       \
            : parent{what_, error_number} {}

struct error : std::exception
{
    public:
        error(int error_number)
            : err_nr{error_number},
              what_{strerror(error_number)}{}
        error(const std::string& msg, int error_number)
            : err_nr{error_number},
              what_{msg}{}
        error(std::string&& msg, int error_number)
            : err_nr{error_number},
              what_{msg}{}
        error(const char* msg, int error_number)
            : err_nr{error_number},
              what_{msg}{}

        virtual const char* what() const noexcept{ return what_.c_str();}
        const int err_num() { return err_nr;}

    private:
        const int err_nr;
        const std::string what_;
};

struct file_error   : error
{
    ERROR_PARENT_CTR(file_error, error);
};

struct socket_error : error
{
    ERROR_PARENT_CTR(socket_error, error);
};
#undef ERROR_PARENT_CTR
#endif /*SOCXX_H_INCLUDE */
