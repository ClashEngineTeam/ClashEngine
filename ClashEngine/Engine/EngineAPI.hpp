#pragma once

#include "../olcPixelGameEngine/olcPixelGameEngine.h"

namespace ClashEngine
{
    class EngineAPI
    {
    public:
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
    };
}