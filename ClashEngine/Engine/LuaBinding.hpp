#pragma once

#include "../Kaguya/kaguya.hpp"

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

    public:
        LuaBinding(kaguya::State* vm);

        void Registe();
    };
}