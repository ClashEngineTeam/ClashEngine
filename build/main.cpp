﻿#ifdef __linux__

//olcPixelGameEngine:
#define OLC_PGE_APPLICATION
#include "../ClashEngine/olcPixelGameEngine/olcPixelGameEngine.h"
//format:
#include "../ClashEngine/Engine/Format.hpp"
//std:
#include <iostream>

int main()
{
    std::string s = ClashEngine::Format::format("{r}{g}{b}", 255, 0, 0);
    std::cout << s << "\n";
    std::cin.get();
    return 0;
}

#endif