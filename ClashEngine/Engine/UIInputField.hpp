#pragma once

#include "UIObject.hpp"
#include "String.hpp"
#include "StringConverter.hpp"
#include "EngineAPI.hpp"
#include "LuaBinding.hpp"
#include "WinIMEHelper.hpp"
#include "Input.hpp"
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
        const int cursorOffsetX = 10;               //坐标X方向偏移
        const int cursorOffsetY = 8;                //坐标Y方向偏移
        float cursor_timer = 0.0f;                  //计时器
        bool cursor_switch = false;                 //光标亮灭开关

    private:
        olc::Font* font;                            //输入框显示文字所需的字体
        std::wstring inputData = L"";               //输入的内容字符串
        int inputDataIndex = 0;                     //输入的内容下标
        int maximumInputDataSize = 100;             //输入字符数量上限

    public:
        std::wstring GetInputData()
        {
            return this->inputData;
        }

        void SetInputDataLimit(int maximumInputDataSize)
        {
            this->maximumInputDataSize = maximumInputDataSize;
        }

    public:
        UIInputField(Vector2 position, Vector2 size, olc::Font* font) : UIObject(position, size)
        {
            this->font = font;
        }

        void OnKeyInput(olc::PixelGameEngine* engine, WCHAR c) override
        {
            if (!flash_cursor) return;

            //保持光标闪烁:
            cursor_timer = 0;
            cursor_switch = true;
            //将输入法窗体显示在合适的位置:
            Vector2 pos = GetPosition();
            Vector2 size = GetSize();
            WinIMEHelper::ShowIMEWindow(LuaBinding::engine->hInstance, pos.x, pos.y + size.y);

            if (c == VK_BACK) //退格
            {
                if (inputData.size() > 0)
                {
                    if (inputDataIndex > 0)
                    {
                        inputDataIndex--;
                        inputData.erase(inputData.begin() + inputDataIndex);
                    }
                }
            }
            else if (c == VK_RETURN) //回车
            {
            }
            else
            {
                if (this->inputData.size() < this->maximumInputDataSize)
                {
                    inputData.insert(inputData.begin() + inputDataIndex, c);
                    inputDataIndex++;
                }
            }
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
            //zoom ratio:
            double yZoomRatio = 0.0;
            //draw rect:
            engine->FillRect(pos.x, pos.y, size.x, size.y, olc::Pixel(155, 155, 155));
            engine->DrawRect(pos.x, pos.y, size.x, size.y, olc::Pixel(0, 0, 0));
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
                wstring sample = L"你好abAB12,./<>?-=_+`~"; //为了辅助字符排版而存在

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

                yZoomRatio = (double)size.y / fontSprite->height;
                EngineAPI::DrawPNGSprite(engine, pos.x, pos.y, diffSprite, yZoomRatio, yZoomRatio);

                delete sampleSprite;
                delete fontSprite;
                delete diffSprite;
#endif
            }
            //draw cursor:
            if (cursor_switch && flash_cursor)
            {
                olc::FontRect fontRect;
                if (!this->inputData.empty())
                {
                    wstring subString = this->inputData.substr(0, this->inputDataIndex);
                    if (!subString.empty())
                    {
                        std::string str = String::WstringToString(subString, Encoding::UTF8);
                        u32string u32str = StringConverter::To_UTF32(str);
                        //GetStringBounds存在BUG, 如果字符串为空字符串, 返回的FontRect将为0
                        //所以这里采取替代的方式绕开这个BUG:
                        //olc::FontRect _aRect;
                        //_aRect.size.x = 0;
                        //_aRect.size.y = 0;
                        ////wstring subString2 = this->inputData.substr(this->inputDataIndex);
                        ////std::string stringAfterIndex = String::WstringToString(subString2, Encoding::UTF8);
                        //std::string _a = "";
                        //for (const auto& item : str)
                        //{
                        //    if (item == ' ')
                        //    {
                        //        _a += "a";
                        //    }
                        //}
                        //if (!_a.empty())
                        //{
                        //    _aRect = font->GetStringBounds(StringConverter::To_UTF32(_a));
                        //}
                        fontRect = font->GetStringBounds(u32str);
                        //加上_a的宽度:
                        //fontRect.size.x += _aRect.size.x;
                    }
                }
                olc::vi2d cursorOriginPos(pos.x + fontRect.size.x * yZoomRatio + cursorOffsetX, pos.y + cursorOffsetY);
                olc::vi2d cursorTargetPos(pos.x + fontRect.size.x * yZoomRatio + cursorOffsetX, pos.y + size.y - cursorOffsetY);
                olc::Pixel cursorColor(0, 240, 0);
                EngineAPI::DrawLine(LuaBinding::engine, cursorOriginPos.x, cursorOriginPos.y, cursorTargetPos.x, cursorTargetPos.y, cursorColor.r, cursorColor.g, cursorColor.b, 1);
            }
        }

        //该函数在OnXXX事件之前调用
        void OnUpdate(olc::PixelGameEngine* engine) override
        {
            if (engine->GetMouse(olc::Mouse::LEFT).bPressed)
            {
                if (flash_cursor) flash_cursor = false;
            }
            //点击输入框可以使用方向键控制光标移动:
            if (flash_cursor)
            {
                if (Input::GetKeyPressed(VK_LEFT))
                {
                    inputDataIndex--;
                    if (inputDataIndex < 0)
                    {
                        inputDataIndex = 0;
                    }
                    cursor_timer = 0;
                    cursor_switch = true;
                }
                else if (Input::GetKeyPressed(VK_RIGHT))
                {
                    inputDataIndex++;
                    if (inputDataIndex > inputData.size())
                    {
                        inputDataIndex = inputData.size();
                    }
                    cursor_timer = 0;
                    cursor_switch = true;
                }
                else if (Input::GetKeyPressed(VK_HOME))
                {
                    inputDataIndex = 0;
                    cursor_timer = 0;
                    cursor_switch = true;
                }
                else if (Input::GetKeyPressed(VK_END))
                {
                    inputDataIndex = this->inputData.size();
                    cursor_timer = 0;
                    cursor_switch = true;
                }
                else if (Input::GetKeyPressed(VK_DELETE))
                {
                    if (inputData.size() > 0)
                    {
                        inputData.erase(inputData.begin() + inputDataIndex);
                    }
                    cursor_timer = 0;
                    cursor_switch = true;
                }
            }
            //update timer:
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