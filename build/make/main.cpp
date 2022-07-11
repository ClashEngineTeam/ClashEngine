#ifdef __linux__

//olcPixelGameEngine:
#define OLC_PGE_APPLICATION
#include "../../ClashEngine/olcPixelGameEngine/olcPixelGameEngine.h"
//format:
#include "../../ClashEngine/Engine/Format.hpp"
//std:
#include <iostream>

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand()% 256));	
		return true;
	}
};

int main()
{
    Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

    std::string s = ClashEngine::Format::format("{r}{g}{b}", 255, 0, 0);
    std::cout << s << "\n";
    std::cin.get();
    
    return 0;
}

#endif