#pragma once

#include "../Kaguya/kaguya.hpp"

namespace ClashEngine
{
    class LuaBinding
    {
    public:
        kaguya::State* vm;

    public:
        LuaBinding(kaguya::State* vm);

        void Registe();
    };
}