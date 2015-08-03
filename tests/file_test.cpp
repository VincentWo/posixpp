#include <iostream>

#include "../File.hpp"

int main()
{
    try
    {
        File f("test.txt", File::RDONLY);
        std::cout << f.to_string() << std::endl;
    }catch(std::exception& e)
    {
        std::cout << e.what();
    }
}
