#define WGUI

#include "Kaguya/kaguya.hpp"
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include "Random/random.hpp"
#include "Engine/LuaBinding.hpp"
#include "Engine/Console.hpp"
#include "Engine/String.hpp"
#include "Engine/File.hpp"

using namespace ClashEngine;

class Program : public olc::PixelGameEngine
{
private:
    kaguya::State* vm;

public:
    Program(kaguya::State* vm)
    {
        this->vm = vm;
    }

    bool OnUserCreate() override
    {
        (*vm)["start"].call<void>();
        return LuaBinding::inited;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        (*vm)["update"].call<void>();
        return LuaBinding::inited;
    }

    bool OnUserDestroy() override
    {
        (*vm)["destroy"].call<void>();
        return LuaBinding::inited;
    }
};

#if defined(WGUI)
int main
{
    kaguya::State state;
    LuaBinding luaBinding(&state);
    luaBinding.Registe();

    std::string path = String::WstringToString(File::Combine(File::GetDirectoryPath(), L"game.lua"));
    kaguya::LuaFunction code = state.loadfile(path.c_str());
    code(); //execute

    if (LuaBinding::inited)
    {
        Program program(&state);
        LuaBinding::engine = &program;
        olc::rcode r = program.Construct(LuaBinding::screenWidth, LuaBinding::screenHeight, 1, 1);
        if (r == olc::OK)
        {
            program.Start();
        }
    }

    return 0;
}
#endif