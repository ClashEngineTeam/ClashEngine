#pragma once

#include "../Kaguya/kaguya.hpp"
#include "../olcPixelGameEngine/olcPixelGameEngine.h"
#include "Console.hpp"

namespace ClashEngine
{
    class LuaBinding
    {
    public:
        kaguya::State* vm;

    public:
        static int screenWidth;
        static int screenHeight;
        static bool inited;
        static olc::PixelGameEngine* engine;
        static Console* console;

    public:
        LuaBinding(kaguya::State* vm);

        void Registe();
    };
}