#include "Kaguya/kaguya.hpp"
#include "Engine/LuaBinding.hpp"

using namespace ClashEngine;

int main(int argc, const char** argv)
{
    kaguya::State state;
    
    LuaBinding binding(&state);
    binding.Registe();

    if (argc > 1)
    {
        const char* luaFilePath = argv[1];
        state.dofile(luaFilePath);
    }

    system("pause");
    return 0;
}