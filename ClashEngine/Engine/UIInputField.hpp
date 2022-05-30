#pragma once

#include "UIObject.hpp"

namespace ClashEngine
{
    class UIInputField : public UIObject
    {
    private:
        bool flash_cursor = false;          //是否开始闪烁光标
        const float cursor_flash_interval = 0.5f; //闪烁光标的间隔时间
        float cursor_timer;
        bool cursor_switch;

    public:
        UIInputField(Vector2 position, Vector2 size) : UIObject(position, size)
        {
        }

        void OnMouseEnter(olc::PixelGameEngine* engine) override
        {

        }

        void OnMouseStay(olc::PixelGameEngine* engine) override
        {

        }

        void OnMouseExit(olc::PixelGameEngine* engine) override
        {

        }

        void OnMouseClick(olc::PixelGameEngine* engine) override
        {
            flash_cursor = true;
            cursor_timer = cursor_flash_interval;
            cursor_switch = false;
        }

        void OnDraw(olc::PixelGameEngine* engine) override
        {
            Vector2 pos = GetPosition();
            Vector2 size = GetSize();
            engine->FillRect(pos.x, pos.y, size.x, size.y, olc::Pixel(155, 155, 155));
            engine->DrawRect(pos.x, pos.y, size.x, size.y, olc::Pixel(0, 0, 0));
            //draw cursor:
            if (cursor_switch && flash_cursor)
            {
                int offset = 5;
                olc::vi2d cursorOriginPos(pos.x + offset, pos.y + offset);
                olc::vi2d cursorTargetPos(pos.x + offset, pos.y + offset + size.y - offset * 2);
                engine->DrawLine(cursorOriginPos, cursorTargetPos, olc::Pixel(0, 0, 0));
            }
        }

        void OnUpdate(olc::PixelGameEngine* engine) override
        {
            if (engine->GetMouse(olc::Mouse::LEFT).bPressed)
            {
                if (flash_cursor) flash_cursor = false;
            }

            float deltaTime = engine->GetElapsedTime();
            cursor_timer += deltaTime;
            if (cursor_timer >= cursor_flash_interval)
            {
                cursor_timer = 0.0f;
                cursor_switch = !cursor_switch;
            }
        }
    };
}