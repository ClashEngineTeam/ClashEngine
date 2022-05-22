﻿#include "LuaBinding.hpp"
#include "Audio.hpp"
#include "String.hpp"

using namespace std;

namespace ClashEngine
{
    //local functions:
    static MCIAudio* init_audio(const char* path)
    {
        wstring str = String::StringToWstring(path, Encoding::UTF8);
        return MinInitMCIAudio(str.c_str());
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

    LuaBinding::LuaBinding(kaguya::State* vm)
    {
        this->vm = vm;
    }

    void LuaBinding::Registe()
    {
        //Audio functions:
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
    }
}