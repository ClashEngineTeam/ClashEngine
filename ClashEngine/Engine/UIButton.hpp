#pragma once

#include "UIObject.hpp"

namespace ClashEngine
{
    class UIButton : public UIObject
    {
    public:
        UIButton(Vector2 position, Vector2 size) : UIObject(position, size)
        {
        }

        void OnDraw(olc::PixelGameEngine* engine) override
        {
            Vector2 pos = GetPosition();
            for (int i = 0; i < size.y; i++)
            {
                for (int j = 0; j < size.x; j++)
                {
                    engine->Draw(j + pos.x, i + pos.y, olc::Pixel(255, 255, 255));
                }
            }
            engine->DrawRect(pos.x, pos.y, size.x, size.y, olc::Pixel(255, 0, 255));
        }
    };
}