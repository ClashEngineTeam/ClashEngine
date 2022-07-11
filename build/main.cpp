#ifdef __linux__

#include "../ClashEngine/Engine/Format.hpp"
#include <iostream>

int main()
{
    std::string s = ClashEngine::Format::format("{r}{g}{b}", 255, 0, 0);
    std::cout << s << "\n";
    std::cin.get();
    return 0;
}

#endif