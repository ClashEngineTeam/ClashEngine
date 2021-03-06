#define WGUI

#include "Kaguya/kaguya.hpp"
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include "Random/random.hpp"
#include "olcPGEX_TTF/olcPGEX_TTF.h"
#include "ENet\enet\enet.h"
#include "Engine/LuaBinding.hpp"
#include "Engine/Console.hpp"
#include "Engine/String.hpp"
#include "Engine/File.hpp"
#include "Engine/Database.hpp"

using namespace ClashEngine;
using namespace std;

static void on_key_input(WCHAR c)
{
    //ui event:
    for (const auto& item : LuaBinding::ui_objects)
    {
        item->OnKeyInput(LuaBinding::engine, c);
    }
}

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
        //game logic:
        (*vm)["start"].call<void>();
        return LuaBinding::inited;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        //ui event:
        for (const auto& item : LuaBinding::ui_objects)
        {
            item->Update(LuaBinding::engine, LuaBinding::state);
        }
        //game logic:
        (*vm)["update"].call<void>();
        return LuaBinding::inited;
    }

    bool OnUserDestroy() override
    {
        //game logic:
        (*vm)["destroy"].call<void>();
        return LuaBinding::inited;
    }
};

int main
{
    //init lua:
    kaguya::State state;
    LuaBinding::state = &state;
    LuaBinding luaBinding(&state);
    luaBinding.Registe();

    //read config:
    Database config(L"config.txt");
    wstring mainPath = config.GetString(L"main", L"");
    if (String::CompareIgnoreCase(mainPath, L""))
    {
        return 0;
    }

    //init olc font:
    olc::Font::init();

    //init enet:
    bool initEnetSuccess = (enet_initialize() == 0);

    //load and execute lua code:
    std::string path = String::WstringToString(File::GetAbsolutePath(mainPath));
    kaguya::LuaFunction code = state.loadfile(path.c_str());
    code(); //execute

    //start engine:
    if (LuaBinding::inited)
    {
        Program program(&state);
        //bind on_key_input:
        program.onWindowInputChar = on_key_input;
        //init global pointer:
        LuaBinding::engine = &program;
        olc::rcode r = program.Construct(LuaBinding::screenWidth, LuaBinding::screenHeight, 1, 1);
        if (r == olc::OK)
        {
            program.Start();
        }
    }

    //deinit enet:
    if (initEnetSuccess)
    {
        enet_deinitialize();
    }

    return 0;
}