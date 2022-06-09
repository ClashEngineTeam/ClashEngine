#pragma once

#include "../olcPixelGameEngine/olcPixelGameEngine.h"
#include "../olcPGEX_TTF/olcPGEX_TTF.h"
#include "String.hpp"
#include "StringConverter.hpp"
#include "Vector2.hpp"
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
        static void DrawLine(olc::PixelGameEngine* engine, int x1, int y1, int x2, int y2, int r, int g, int b)
        {
            engine->DrawLine(x1, y1, x2, y2, olc::Pixel(r, g, b));
        }

        static void DrawLine(olc::PixelGameEngine* engine, int x1, int y1, int x2, int y2, int r, int g, int b, int thick)
        {
            Vector2 start(x1 - thick, y1 - thick);
            Vector2 end(x2 - thick, y2 - thick);
            int count = 1 + 2 * thick;
            for (int i = 0; i < count; i++)
            {
                for (int j = 0; j < count; j++)
                {
                    engine->DrawLine(start.x + j, start.y + i, end.x + j, end.y + i, olc::Pixel(r, g, b));
                }
            }
        }

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

        static void DrawString(olc::PixelGameEngine* engine, olc::Font* font, const std::string& s, int x, int y, int r, int g, int b)
        {
            font->DrawString(StringConverter::To_UTF32(s), x, y, olc::Pixel(r, g, b));
        }

        static void DrawString(olc::PixelGameEngine* engine, olc::Font* font, const std::string& s, int x, int y, int r, int g, int b, int interval)
        {
            int nextX = x;
            std::wstring ws = String::StringToWstring(s, Encoding::UTF8);
            for (const wchar& item : ws)
            {
                std::string str = String::WstringToString(std::wstring(1, item), Encoding::UTF8);
                std::u32string u32String = StringConverter::To_UTF32(str);
                //new:
                olc::Sprite* charSprite = font->RenderStringToSprite(u32String, olc::Pixel(r, g, b));
                //draw sprite:
                EngineAPI::DrawPNGSprite(engine, nextX, y, charSprite);
                nextX += (interval + charSprite->width);
                //delete:
                delete charSprite;
            }
        }

        static olc::Sprite* RenderStringToSprite(olc::PixelGameEngine* engine, olc::Font* font, const std::string& s, int r, int g, int b)
        {
            return font->RenderStringToSprite(StringConverter::To_UTF32(s), olc::Pixel(r, g, b));
        }

        static olc::Sprite* RenderStringToSprite(olc::PixelGameEngine* engine, olc::Font* font, const std::string& s, int r, int g, int b, int interval)
        {
            std::wstring ws = String::StringToWstring(s, Encoding::UTF8);
            int fontWidth = font->GetStringBounds(StringConverter::To_UTF32(s)).size.x;
            int fontHeight = font->GetStringBounds(StringConverter::To_UTF32(s)).size.y;

            int spriteWidth = fontWidth + (ws.size() - 1) * interval;
            int spriteHeight = fontHeight;
            olc::Sprite* fontSprite = new olc::Sprite(spriteWidth, spriteHeight);

            int x = 0;
            for (const wchar& item : ws)
            {
                std::string str = String::WstringToString(std::wstring(1, item), Encoding::UTF8);
                std::u32string u32String = StringConverter::To_UTF32(str);
                //new:
                olc::Sprite* charSprite = font->RenderStringToSprite(u32String, olc::Pixel(r, g, b));
                //draw char:
                for (int i = 0; i < spriteHeight; i++)
                {
                    for (int j = 0; j < charSprite->width; j++)
                    {
                        olc::Pixel pixel;
                        if (i < charSprite->height)
                        {
                            pixel = charSprite->GetPixel(j, i);
                        }
                        else
                        {
                            pixel = olc::Pixel(0, 0, 0, 0);
                        }
                        fontSprite->SetPixel(j + x, i, pixel);
                    }
                }
                x += charSprite->width;
                //draw interval:
                for (int i = 0; i < spriteHeight; i++)
                {
                    for (int j = 0; j < spriteWidth; j++)
                    {
                        fontSprite->SetPixel(j + x, i, olc::Pixel(0, 0, 0, 0));
                    }
                }
                x += interval;
                //delete:
                delete charSprite;
            }

            return fontSprite;
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