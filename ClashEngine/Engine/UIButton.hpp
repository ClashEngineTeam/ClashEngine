#pragma once

#include "UIObject.hpp"

namespace ClashEngine
{
    class UIButton : public UIObject
    {
    public:
        olc::Pixel backColor;
        olc::Pixel borderColor;
        //默认颜色:
        olc::Pixel defaultBackColor;
        olc::Pixel defaultBorderColor;
        bool change_border_color = true;

    public:
        UIButton(Vector2 position, Vector2 size) : UIObject(position, size)
        {
            defaultBackColor = olc::Pixel(128, 128, 128); //Gray
            defaultBorderColor = olc::Pixel(255, 215, 0); //Gold
            backColor = defaultBackColor;
            borderColor = defaultBorderColor;
        }

        void OnMouseEnter(olc::PixelGameEngine* engine) override
        {
            backColor = olc::Pixel(192, 192, 192); //Silver
        }

        void OnMouseStay(olc::PixelGameEngine* engine) override
        {
        }

        void OnMouseExit(olc::PixelGameEngine* engine) override
        {
            backColor = defaultBackColor;
        }

        void OnMouseClick(olc::PixelGameEngine* engine) override
        {
            if (change_border_color)
            {
                borderColor = olc::Pixel(0, 139, 139); //DarkCyan
            }
            else
            {
                borderColor = defaultBorderColor;
            }
            change_border_color = !change_border_color;
        }

        void OnDraw(olc::PixelGameEngine* engine) override
        {
            Vector2 pos = GetPosition();
            Vector2 size = GetSize();
            for (int i = 0; i < size.y; i++)
            {
                for (int j = 0; j < size.x; j++)
                {
                    engine->Draw(j + pos.x, i + pos.y, this->backColor);
                }
            }
            engine->DrawRect(pos.x, pos.y, size.x, size.y, this->borderColor);
        }
    };
}