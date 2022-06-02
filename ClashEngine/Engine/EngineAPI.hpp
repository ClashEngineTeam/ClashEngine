#pragma once

#include "../olcPixelGameEngine/olcPixelGameEngine.h"
#include "../olcPGEX_TTF/olcPGEX_TTF.h"
#include "String.hpp"
#include "StringConverter.hpp"
#include <vector>
#include <string>

namespace ClashEngine
{
    struct FontSprite
    {
    public:
        olc::Sprite* sprite;
        olc::vi2d offset;
        olc::vi2d size;

        FontSprite(olc::Sprite* sprite, olc::vi2d offset, olc::vi2d size)
        {
            this->sprite = sprite;
            this->offset = offset;
            this->size = size;
        }

        ~FontSprite()
        {
            delete this->sprite;
        }
    };

    class EngineAPI
    {
    public:
        static void DrawSprite(olc::PixelGameEngine* engine, int x, int y, olc::Sprite* sprite)
        {
            engine->DrawSprite(x, y, sprite);
        }

        //基于等间隔采样的图像缩放算法:https://blog.csdn.net/qq_37394634/article/details/99675686
        static void DrawSprite(olc::PixelGameEngine* engine, int x, int y, olc::Sprite* sprite, int w, int h)
        {
            double scalingRatioX = abs((double)w / sprite->width);
            double scalingRatioY = abs((double)h / sprite->height);

            int newWidth = sprite->width * scalingRatioX;
            int newHeight = sprite->height * scalingRatioY;

            scalingRatioX = 1 / scalingRatioX;
            scalingRatioY = 1 / scalingRatioY;

            for (int i = 0; i < newHeight; i++)
            {
                for (int j = 0; j < newWidth; j++)
                {
                    int xx = 0, yy = 0;
                    if (w > 0)
                    {
                        xx = j * scalingRatioX;
                    }
                    else if (w < 0)
                    {
                        xx = sprite->width - 1 - j * scalingRatioX;
                    }
                    if (h > 0)
                    {
                        yy = i * scalingRatioY;
                    }
                    else if (h < 0)
                    {
                        yy = sprite->height - 1 - i * scalingRatioY;
                    }
                    //clamp:
                    if (xx < 0) xx = 0;
                    if (yy < 0) yy = 0;
                    if (xx >= sprite->width) xx = sprite->width - 1;
                    if (yy >= sprite->height) yy = sprite->height - 1;
                    //sample:
                    olc::Pixel pixel = sprite->GetPixel(xx, yy);
                    //draw pixel:
                    engine->Draw(x + j, y + i, pixel);
                }
            }
        }

        //基于等间隔采样的图像缩放算法:https://blog.csdn.net/qq_37394634/article/details/99675686
        static void DrawSprite(olc::PixelGameEngine* engine, int x, int y, olc::Sprite* sprite, double sx, double sy)
        {
            double scalingRatioX = abs(sx);
            double scalingRatioY = abs(sy);

            int newWidth = sprite->width * scalingRatioX;
            int newHeight = sprite->height * scalingRatioY;

            scalingRatioX = 1 / scalingRatioX;
            scalingRatioY = 1 / scalingRatioY;

            for (int i = 0; i < newHeight; i++)
            {
                for (int j = 0; j < newWidth; j++)
                {
                    int xx = 0, yy = 0;
                    if (sx > 0)
                    {
                        xx = j * scalingRatioX;
                    }
                    else if (sx < 0)
                    {
                        xx = sprite->width - 1 - j * scalingRatioX;
                    }
                    if (sy > 0)
                    {
                        yy = i * scalingRatioY;
                    }
                    else if (sy < 0)
                    {
                        yy = sprite->height - 1 - i * scalingRatioY;
                    }
                    //clamp:
                    if (xx < 0) xx = 0;
                    if (yy < 0) yy = 0;
                    if (xx >= sprite->width) xx = sprite->width - 1;
                    if (yy >= sprite->height) yy = sprite->height - 1;
                    //sample:
                    olc::Pixel pixel = sprite->GetPixel(xx, yy);
                    //draw pixel:
                    engine->Draw(x + j, y + i, pixel);
                }
            }
        }

        static void DrawPNGSprite(olc::PixelGameEngine* engine, int x, int y, olc::Sprite* sprite)
        {
            engine->SetPixelMode(olc::Pixel::ALPHA);
            engine->DrawSprite(x, y, sprite);
            engine->SetPixelMode(olc::Pixel::NORMAL);
        }

        static void DrawPNGSprite(olc::PixelGameEngine* engine, int x, int y, olc::Sprite* sprite, int w, int h)
        {
            engine->SetPixelMode(olc::Pixel::ALPHA);
            DrawSprite(engine, x, y, sprite, w, h);
            engine->SetPixelMode(olc::Pixel::NORMAL);
        }

        static void DrawPNGSprite(olc::PixelGameEngine* engine, int x, int y, olc::Sprite* sprite, double sx, double sy)
        {
            engine->SetPixelMode(olc::Pixel::ALPHA);
            DrawSprite(engine, x, y, sprite, sx, sy);
            engine->SetPixelMode(olc::Pixel::NORMAL);
        }

        //!!!需要使用DeleteSprites来释放创建的所有Sprite*
        static std::vector<FontSprite*> StringToSprites(olc::Font* font, const std::wstring& ws, int r, int g, int b)
        {
            std::vector<FontSprite*> sprites;

            for (const wchar& item : ws)
            {
                std::string str = String::WstringToString(std::wstring(1, item), Encoding::UTF8);
                std::u32string u32String = StringConverter::To_UTF32(str);
                olc::FontRect rect = font->GetStringBounds(u32String);
                //new sprite:
                olc::Sprite* charSprite = font->RenderStringToSprite(u32String, olc::Pixel(r, g, b));
                //new FontSprite:
                FontSprite* fs = new FontSprite(charSprite, rect.offset, rect.size);
                sprites.push_back(fs);
            }

            return sprites;
        }

        static void DeleteSprites(std::vector<FontSprite*>& sprites)
        {
            for (const auto& item : sprites)
            {
                if (item != nullptr) delete item;
            }
            sprites.clear();
        }
    };
}