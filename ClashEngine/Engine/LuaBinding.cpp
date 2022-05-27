﻿#include "LuaBinding.hpp"
#include "Audio.hpp"
#include "String.hpp"
#include "File.hpp"
#include <string>

using namespace std;

namespace ClashEngine
{
    //=====================Audio APIs=====================

    //支持相对路径与绝对路径
    static MCIAudio* init_audio(const char* path)
    {
        wstring wpath = String::StringToWstring(path, Encoding::UTF8);
        return MinInitMCIAudio(File::GetAbsolutePath(wpath).c_str());
    }

    static void deinit_audio(MCIAudio* audio)
    {
        MinDeinitMCIAudio(audio);
    }

    static bool play_audio(MCIAudio* audio, bool repeat, bool wait)
    {
        return MinPlayMCIAudio(audio, repeat, wait);
    }

    static bool play_audio_ex(MCIAudio* audio, bool repeat, bool wait, int from, int to)
    {
        return MinPlayMCIAudioEx(audio, repeat, wait, from, to);
    }

    static bool stop_audio(MCIAudio* audio)
    {
        return MinStopMCIAudio(audio);
    }

    static bool pause_audio(MCIAudio* audio)
    {
        return MinPauseMCIAudio(audio);
    }

    static bool resume_audio(MCIAudio* audio)
    {
        return MinResumeMCIAudio(audio);
    }

    static int get_audio_volume(MCIAudio* audio)
    {
        return MinGetMCIAudioVolume(audio);
    }

    static bool set_audio_volume(MCIAudio* audio, int volume)
    {
        return MinSetMCIAudioVolume(audio, volume);
    }

    static int get_audio_position(MCIAudio* audio)
    {
        return MinGetMCIAudioPosition(audio);
    }

    static bool set_audio_position(MCIAudio* audio, int position)
    {
        return MinSetMCIAudioPosition(audio, position);
    }

    static int get_audio_speed(MCIAudio* audio)
    {
        return MinGetMCIAudioSpeed(audio);
    }

    static bool set_audio_speed(MCIAudio* audio, int speed)
    {
        return MinSetMCIAudioSpeed(audio, speed);
    }

    static int get_audio_mode(MCIAudio* audio)
    {
        return (int)MinGetMCIAudioMode(audio);
    }

    static bool audio_is_playing(MCIAudio* audio)
    {
        return MinGetMCIAudioIsPlaying(audio);
    }

    static bool audio_is_over(MCIAudio* audio)
    {
        return MinGetMCIAudioIsOver(audio);
    }

    static bool audio_is_over_ex(MCIAudio* audio, int length)
    {
        return MinGetMCIAudioIsOverEx(audio, length);
    }

    //=====================Engine APIs=====================

    static bool init_engine(int screenWidth, int screenHeight)
    {
        LuaBinding::screenWidth = screenWidth;
        LuaBinding::screenHeight = screenHeight;
        LuaBinding::inited = true;
        return true;
    }

    static void deinit_engine()
    {
        LuaBinding::inited = false;
    }

    //=====================Image APIs=====================

    //支持相对路径与绝对路径
    static olc::Sprite* init_image(const char* path)
    {
        wstring wpath = String::StringToWstring(path, Encoding::UTF8);
        olc::Sprite* sprite = new olc::Sprite(String::WstringToString(File::GetAbsolutePath(wpath)));
        return sprite;
    }

    static void deinit_image(olc::Sprite* sprite)
    {
        delete sprite;
    }

    static void draw_image(int x, int y, olc::Sprite* sprite)
    {
        LuaBinding::engine->DrawSprite(x, y, sprite);
    }

    //=====================Draw APIs=====================

    static void draw_pixel(int x, int y, int r, int g, int b)
    {
        LuaBinding::engine->Draw(x, y, olc::Pixel(r, g, b));
    }

    int LuaBinding::screenWidth = 0;
    int LuaBinding::screenHeight = 0;
    bool LuaBinding::inited = false;
    olc::PixelGameEngine* LuaBinding::engine = nullptr;

    LuaBinding::LuaBinding(kaguya::State* vm)
    {
        this->vm = vm;
    }

    void LuaBinding::Registe()
    {
        //Audio APIs:
        (*this->vm)["init_audio"] = &init_audio;
        (*this->vm)["deinit_audio"] = &deinit_audio;
        (*this->vm)["play_audio"] = &play_audio;
        (*this->vm)["play_audio_ex"] = &play_audio_ex;
        (*this->vm)["stop_audio"] = &stop_audio;
        (*this->vm)["pause_audio"] = &pause_audio;
        (*this->vm)["resume_audio"] = &resume_audio;
        (*this->vm)["get_audio_volume"] = &get_audio_volume;
        (*this->vm)["set_audio_volume"] = &set_audio_volume;
        (*this->vm)["get_audio_position"] = &get_audio_position;
        (*this->vm)["set_audio_position"] = &set_audio_position;
        (*this->vm)["get_audio_speed"] = &get_audio_speed;
        (*this->vm)["set_audio_speed"] = &set_audio_speed;
        (*this->vm)["get_audio_mode"] = &get_audio_mode;
        (*this->vm)["audio_is_playing"] = &audio_is_playing;
        (*this->vm)["audio_is_over"] = &audio_is_over;
        (*this->vm)["audio_is_over_ex"] = &audio_is_over_ex;
        //Engine APIs:
        (*this->vm)["init_engine"] = &init_engine;
        (*this->vm)["deinit_engine"] = &deinit_engine;
        //Image APIs:
        (*this->vm)["init_image"] = &init_image;
        (*this->vm)["deinit_image"] = &deinit_image;
        (*this->vm)["draw_image"] = &draw_image;
        //Draw functions:
        (*this->vm)["draw_pixel"] = &draw_pixel;
        //Input functions:
    }
}