#pragma once

#include <string>
#include "String.hpp"
#include "../olcPixelGameEngine/olcPixelGameEngine.h"

extern "C"
{
#include "libavcodec\avcodec.h"
#include "libavformat/avformat.h"
#include <libswscale/swscale.h>
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "avutil.lib")
}

namespace ClashEngine
{
    //灵感来源:https://www.youtube.com/watch?v=W6Yx3injNZs
    class Video
    {
    private:
        AVFormatContext* format;
        AVCodecContext* codecContext;
        int videoStreamIndex;
        int videoWidth;
        int videoHeight;
        olc::Sprite* frameSprite;
        uint8_t* data;

    public:
        std::wstring path;

        Video(const std::wstring& path)
        {
            this->path = path;
            //初始化视频:
            this->format = avformat_alloc_context();
            avformat_open_input(&this->format, String::WstringToString(path).c_str(), nullptr, nullptr);
            //解析视频:
            AVCodecParameters* params = nullptr;
            AVCodec* codec = nullptr;
            for (int i = 0; i < format->nb_streams; i++)
            {
                params = format->streams[i]->codecpar;
                codec = (AVCodec*)avcodec_find_decoder(params->codec_id);
                if (codec->type == AVMEDIA_TYPE_VIDEO)
                {
                    this->videoStreamIndex = i;
                    break;
                }
            }
            //解码器初始化(codec context for decoder):
            this->codecContext = avcodec_alloc_context3(codec);
            avcodec_parameters_to_context(this->codecContext, params);
            avcodec_open2(this->codecContext, codec, nullptr);
            this->videoWidth = codecContext->width;
            this->videoHeight = codecContext->height;
            //分配帧内存:
            this->frameSprite = new olc::Sprite(this->videoWidth, this->videoHeight);
            this->data = new uint8_t[this->videoWidth * this->videoHeight * 4]; //RGBA
        }

        ~Video()
        {
            avformat_close_input(&this->format);
            avformat_free_context(this->format);

            avcodec_close(this->codecContext);
            avcodec_free_context(&this->codecContext);

            //释放帧内存:
            delete this->frameSprite;
            delete[] this->data;
        }

        int GetWidth()
        {
            return this->videoWidth;
        }

        int GetHeight()
        {
            return this->videoHeight;
        }

        olc::Sprite* GetNextFrame()
        {
            AVFrame* frame = av_frame_alloc();
            AVPacket* packet = av_packet_alloc();

            int n;
            while ((n = av_read_frame(this->format, packet)) >= 0)
            {
                if (packet->stream_index != videoStreamIndex)
                {
                    continue;
                }
                int response = avcodec_send_packet(codecContext, packet);
                response = avcodec_receive_frame(codecContext, frame);
                if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
                {
                    continue;
                }
                av_packet_unref(packet);
                break;
            }

            //alloc SwsContext:
            SwsContext* scalerContext = sws_getContext(
                frame->width, frame->height, codecContext->pix_fmt,
                frame->width, frame->height, AV_PIX_FMT_RGB0,
                SWS_BILINEAR, nullptr, nullptr, nullptr
            );
            if (scalerContext != nullptr)
            {
                //赋值:
                uint8_t* dest[4] = { this->data, nullptr, nullptr, nullptr };
                int dest_linesize[4] = { frame->width * 4, 0, 0, 0 };
                sws_scale(scalerContext, frame->data, frame->linesize, 0, frame->height, dest, dest_linesize);
                //free SwsContext:
                sws_freeContext(scalerContext);
                //赋值彩色:
                for (int i = 0; i < frame->height; i++)
                {
                    for (int j = 0; j < frame->width; j++)
                    {
                        byte r = this->data[(i * frame->width + j) * 4];
                        byte g = this->data[(i * frame->width + j) * 4 + 1];
                        byte b = this->data[(i * frame->width + j) * 4 + 2];
                        this->frameSprite->SetPixel(j, i, olc::Pixel(r, g, b));
                    }
                }
            }

            av_frame_free(&frame);
            av_packet_free(&packet);

            return this->frameSprite;
        }
    };
}