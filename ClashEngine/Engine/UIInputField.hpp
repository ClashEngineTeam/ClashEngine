#pragma once

#include "UIObject.hpp"
#include "String.hpp"
#include "StringConverter.hpp"
#include "EngineAPI.hpp"
#include "LuaBinding.hpp"
#include "WinIMEHelper.hpp"
#include <string>

using namespace std;

namespace ClashEngine
{
    //输入框
    class UIInputField : public UIObject
    {
    private:
        bool flash_cursor = false;                  //是否开始闪烁光标(该标签可以判断输入框是否被激活)
        const float cursor_flash_interval = 0.5f;   //闪烁光标的间隔时间
        const int offset = 8;                       //坐标偏移
        float cursor_timer = 0.0f;                  //计时器
        bool cursor_switch = false;                 //光标亮灭开关

    private:
        std::wstring inputData = L"";               //输入的内容字符串
        olc::Font* font;                            //输入框显示文字所需的字体

    public:
        std::wstring GetInputData()
        {
            return this->inputData;
        }

    public:
        UIInputField(Vector2 position, Vector2 size, olc::Font* font) : UIObject(position, size)
        {
            this->font = font;
        }

        void OnKeyInput(olc::PixelGameEngine* engine, WCHAR c) override
        {
            if (!flash_cursor) return;

            if (c == VK_BACK)
            {
                if (inputData.size() > 0)
                {
                    inputData.pop_back();
                }
            }
            else
            {
                inputData.push_back(c);
            }
            //将输入法窗体显示在合适的位置:
            Vector2 pos = GetPosition();
            Vector2 size = GetSize();
            WinIMEHelper::ShowIMEWindow(LuaBinding::engine->hInstance, pos.x, pos.y + size.y);
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
                olc::vi2d cursorOriginPos(pos.x + offset, pos.y + offset);
                olc::vi2d cursorTargetPos(pos.x + offset, pos.y + offset + size.y - offset * 2);
                olc::Pixel cursorColor(0, 0, 0);
                EngineAPI::DrawLine(LuaBinding::engine, cursorOriginPos.x, cursorOriginPos.y, cursorTargetPos.x, cursorTargetPos.y, cursorColor.r, cursorColor.g, cursorColor.b, 1);
            }
            //draw string:
            if (inputData.size() > 0)
            {
#ifdef UI_TEST
                vector<FontSprite*> sprites = EngineAPI::StringToSprites(this->font, inputData, 255, 255, 255);
                int width = 0;
                for (auto& item : sprites)
                {
                    double yZoomRatio = (double)size.y / item->sprite->height;
                    int newWidth = item->sprite->width * yZoomRatio;
                    int newHeight = item->sprite->height * yZoomRatio;
                    EngineAPI::DrawPNGSprite(engine, pos.x + width, pos.y, item->sprite, newWidth, newHeight);
                    width += newWidth + (item->offset.x);
                }
                EngineAPI::DeleteSprites(sprites);
#else
                wstring sample = L"你好abAB12,./<>?-=_+`~";

                std::string sampleStr = String::WstringToString(sample, Encoding::UTF8);
                u32string u32SampleStr = StringConverter::To_UTF32(sampleStr);
                olc::Sprite* sampleSprite = font->RenderStringToSprite(u32SampleStr, olc::BLACK);

                std::string str = String::WstringToString(sample + inputData, Encoding::UTF8);
                u32string u32str = StringConverter::To_UTF32(str);
                olc::Sprite* fontSprite = font->RenderStringToSprite(u32str, olc::Pixel(33, 33, 33));

                int diffWidth = fontSprite->width - sampleSprite->width;
                olc::Sprite* diffSprite = new olc::Sprite(diffWidth, fontSprite->height);
                for (int i = 0; i < diffSprite->height; i++)
                {
                    for (int j = 0; j < diffSprite->width; j++)
                    {
                        olc::Pixel p = fontSprite->GetPixel(j + sampleSprite->width, i);
                        diffSprite->SetPixel(j, i, p);
                    }
                }

                double yZoomRatio = (double)size.y / fontSprite->height;
                EngineAPI::DrawPNGSprite(engine, pos.x, pos.y, diffSprite, yZoomRatio, yZoomRatio);

                delete sampleSprite;
                delete fontSprite;
                delete diffSprite;
#endif
            }
        }

        //该函数在OnXXX事件之前调用
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