#include "LuaBinding.hpp"
#include "Audio.hpp"
#include "String.hpp"
#include "File.hpp"
#include "Vector2.hpp"
#include "Debug.hpp"
#include "Input.hpp"
#include "StringConverter.hpp"
#include <string>
#include <vector>
#include <cmath>

using namespace std;

namespace ClashEngine
{
    //=====================Audio APIs=====================

    //支持相对路径与绝对路径
    static MCIAudio* init_audio(const string& path)
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

    static float get_delta_time()
    {
        return LuaBinding::engine->GetElapsedTime();
    }

    //=====================Draw APIs=====================

    static void clear()
    {
        LuaBinding::engine->Clear(olc::BLACK);
    }

    static void clear_color(int r, int g, int b)
    {
        LuaBinding::engine->Clear(olc::Pixel(r, g, b));
    }

    static void draw_pixel(int x, int y, int r, int g, int b)
    {
        LuaBinding::engine->Draw(x, y, olc::Pixel(r, g, b));
    }

    static void draw_line(int x1, int y1, int x2, int y2, int r, int g, int b)
    {
        LuaBinding::engine->DrawLine(x1, y1, x2, y2, olc::Pixel(r, g, b));
    }

    static void draw_line_bold(int x1, int y1, int x2, int y2, int r, int g, int b, int thick)
    {
        Vector2 start(x1 - thick, y1 - thick);
        Vector2 end(x2 - thick, y2 - thick);
        int count = 1 + 2 * thick;
        for (int i = 0; i < count; i++)
        {
            for (int j = 0; j < count; j++)
            {
                LuaBinding::engine->DrawLine(start.x + j, start.y + i, end.x + j, end.y + i, olc::Pixel(r, g, b));
            }
        }
    }

    //=====================Image APIs=====================

    //支持相对路径与绝对路径
    static olc::Sprite* init_image(const string& path)
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

    //基于等间隔采样的图像缩放算法:https://blog.csdn.net/qq_37394634/article/details/99675686
    static void draw_image_scaling(int x, int y, olc::Sprite* sprite, int w, int h)
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
                LuaBinding::engine->Draw(x + j, y + i, pixel);
            }
        }
    }

    //基于等间隔采样的图像缩放算法:https://blog.csdn.net/qq_37394634/article/details/99675686
    static void draw_image_scalingf(int x, int y, olc::Sprite* sprite, double sx, double sy)
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
                LuaBinding::engine->Draw(x + j, y + i, pixel);
            }
        }
    }

    static int get_image_width(olc::Sprite* sprite)
    {
        return sprite->width;
    }

    static int get_image_height(olc::Sprite* sprite)
    {
        return sprite->height;
    }

    //=====================Input APIs=====================

    static bool get_key(int key)
    {
        return LuaBinding::engine->GetKey((olc::Key)key).bHeld;
    }

    static bool get_key_down(int key)
    {
        return LuaBinding::engine->GetKey((olc::Key)key).bPressed;
    }

    static bool get_key_up(int key)
    {
        return LuaBinding::engine->GetKey((olc::Key)key).bReleased;
    }

    static bool get_mouse(int mouse)
    {
        return LuaBinding::engine->GetMouse(mouse).bHeld;
    }

    static bool get_mouse_down(int mouse)
    {
        return LuaBinding::engine->GetMouse(mouse).bPressed;
    }

    static bool get_mouse_up(int mouse)
    {
        return LuaBinding::engine->GetMouse(mouse).bReleased;
    }

    static int get_mouse_x()
    {
        return LuaBinding::engine->GetMouseX();
    }

    static int get_mouse_y()
    {
        return LuaBinding::engine->GetMouseY();
    }

    static int get_mouse_wheel()
    {
        return LuaBinding::engine->GetMouseWheel();
    }

    //=====================new Input APIs=====================

    static bool GetKey(int key)
    {
        return Input::GetKey(key);
    }

    static bool GetKeyDown(int key)
    {
        return Input::GetKeyDown(key);
    }

    static bool GetKeyUp(int key)
    {
        return Input::GetKeyUp(key);
    }

    //=====================Console APIs=====================

    static Console* init_console()
    {
        if (LuaBinding::console == nullptr)
        {
            //首先判断当前程序是否附加了控制台
            HWND consoleWindow = Console::GetConsoleWindow();
            //没有附加控制台
            if (consoleWindow == nullptr)
            {
                ConsoleSession session = Console::AllocConsole();
                //初始化成功
                if (session.consoleInput != 0 && session.consoleOutput != 0 && session.consoleWindow != 0)
                {
                    LuaBinding::console = new Console(session);
                    LuaBinding::console_active = true;
                }
                else
                {
                    LuaBinding::console = nullptr;
                    LuaBinding::console_active = false;
                }
            }
            //已经附加了控制台
            else
            {
                LuaBinding::console = &ClashEngine::console;
                LuaBinding::console_active = true;
                LuaBinding::attched_console = true;
            }
        }

        return LuaBinding::console;
    }

    static void deinit_console(Console* console)
    {
        if (console != nullptr && LuaBinding::console_active)
        {
            Console::FreeConsole();
            if (!LuaBinding::attched_console)
            {
                delete console;
            }
            LuaBinding::console_active = false;
        }
    }

    static void write_console(Console* console, const string& s)
    {
        console->Write(String::StringToWstring(s, Encoding::UTF8));
    }

    static void writeline_console(Console* console, const string& s)
    {
        console->WriteLine(String::StringToWstring(s, Encoding::UTF8));
    }

    static char readkey_console(Console* console)
    {
        wchar wc = console->ReadKey().KeyChar;
        return String::WcharToChar(wc, Encoding::UTF8);
    }

    static string readline_console(Console* console)
    {
        wstring ws = console->ReadLine();
        return String::WstringToString(ws, Encoding::UTF8);
    }

    //=====================Font APIs=====================

    //支持相对路径与绝对路径
    static olc::Font* init_font(const string& path, int fontSize)
    {
        wstring wpath = File::GetAbsolutePath(String::StringToWstring(path, Encoding::UTF8));
        olc::Font* font = new olc::Font(String::WstringToString(wpath), fontSize);
        return font;
    }

    static void deinit_font(olc::Font* font)
    {
        delete font;
    }

    static void draw_font(olc::Font* font, const string& s, int x, int y, int r, int g, int b)
    {
        font->DrawString(StringConverter::To_UTF32(s), x, y, olc::Pixel(r, g, b));
    }

    static int get_font_width(olc::Font* font, const string& s)
    {
        olc::FontRect rect = font->GetStringBounds(StringConverter::To_UTF32(s));
        return rect.size.x;
    }

    static int get_font_height(olc::Font* font, const string& s)
    {
        olc::FontRect rect = font->GetStringBounds(StringConverter::To_UTF32(s));
        return rect.size.y;
    }

    static int get_font_offset_x(olc::Font* font, const string& s)
    {
        olc::FontRect rect = font->GetStringBounds(StringConverter::To_UTF32(s));
        return rect.offset.x;
    }

    static int get_font_offset_y(olc::Font* font, const string& s)
    {
        olc::FontRect rect = font->GetStringBounds(StringConverter::To_UTF32(s));
        return rect.offset.y;
    }

    int LuaBinding::screenWidth = 0;
    int LuaBinding::screenHeight = 0;
    bool LuaBinding::inited = false;
    olc::PixelGameEngine* LuaBinding::engine = nullptr;
    Console* LuaBinding::console = nullptr;
    bool LuaBinding::console_active = false;
    bool LuaBinding::attched_console = false;

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
        (*this->vm)["get_delta_time"] = &get_delta_time;
        //Draw APIs:
        (*this->vm)["clear"] = &clear;
        (*this->vm)["clear_color"] = &clear_color;
        (*this->vm)["draw_pixel"] = &draw_pixel;
        (*this->vm)["draw_line"] = &draw_line;
        (*this->vm)["draw_line_bold"] = &draw_line_bold;
        //Image APIs:
        (*this->vm)["init_image"] = &init_image;
        (*this->vm)["deinit_image"] = &deinit_image;
        (*this->vm)["draw_image"] = &draw_image;
        (*this->vm)["draw_image_scaling"] = &draw_image_scaling;
        (*this->vm)["draw_image_scalingf"] = &draw_image_scalingf;
        (*this->vm)["get_image_width"] = &get_image_width;
        (*this->vm)["get_image_height"] = &get_image_height;
        //Input APIs:
        (*this->vm)["get_key"] = &get_key;
        (*this->vm)["get_key_down"] = &get_key_down;
        (*this->vm)["get_key_up"] = &get_key_up;
        (*this->vm)["get_mouse"] = &get_mouse;
        (*this->vm)["get_mouse_down"] = &get_mouse_down;
        (*this->vm)["get_mouse_up"] = &get_mouse_up;
        (*this->vm)["get_mouse_x"] = &get_mouse_x;
        (*this->vm)["get_mouse_y"] = &get_mouse_y;
        (*this->vm)["get_mouse_wheel"] = &get_mouse_wheel;
        //new Input APIs:
        (*this->vm)["GetKey"] = &GetKey;
        (*this->vm)["GetKeyDown"] = &GetKeyDown;
        (*this->vm)["GetKeyUp"] = &GetKeyUp;
        //Mouse Buttons:
        (*this->vm)("MOUSE_LEFT = " + std::to_string(olc::Mouse::LEFT));
        (*this->vm)("MOUSE_RIGHT = " + std::to_string(olc::Mouse::RIGHT));
        (*this->vm)("MOUSE_MIDDLE = " + std::to_string(olc::Mouse::MIDDLE));
        //KEYS:
        //按键A-Z:
        (*this->vm)("KEY_A = " + std::to_string(olc::Key::A));
        (*this->vm)("KEY_B = " + std::to_string(olc::Key::B));
        (*this->vm)("KEY_C = " + std::to_string(olc::Key::C));
        (*this->vm)("KEY_D = " + std::to_string(olc::Key::D));
        (*this->vm)("KEY_E = " + std::to_string(olc::Key::E));
        (*this->vm)("KEY_F = " + std::to_string(olc::Key::F));
        (*this->vm)("KEY_G = " + std::to_string(olc::Key::G));
        (*this->vm)("KEY_H = " + std::to_string(olc::Key::H));
        (*this->vm)("KEY_I = " + std::to_string(olc::Key::I));
        (*this->vm)("KEY_J = " + std::to_string(olc::Key::J));
        (*this->vm)("KEY_K = " + std::to_string(olc::Key::K));
        (*this->vm)("KEY_L = " + std::to_string(olc::Key::L));
        (*this->vm)("KEY_M = " + std::to_string(olc::Key::M));
        (*this->vm)("KEY_N = " + std::to_string(olc::Key::N));
        (*this->vm)("KEY_O = " + std::to_string(olc::Key::O));
        (*this->vm)("KEY_P = " + std::to_string(olc::Key::P));
        (*this->vm)("KEY_Q = " + std::to_string(olc::Key::Q));
        (*this->vm)("KEY_R = " + std::to_string(olc::Key::R));
        (*this->vm)("KEY_S = " + std::to_string(olc::Key::S));
        (*this->vm)("KEY_T = " + std::to_string(olc::Key::T));
        (*this->vm)("KEY_U = " + std::to_string(olc::Key::U));
        (*this->vm)("KEY_V = " + std::to_string(olc::Key::V));
        (*this->vm)("KEY_W = " + std::to_string(olc::Key::W));
        (*this->vm)("KEY_X = " + std::to_string(olc::Key::X));
        (*this->vm)("KEY_Y = " + std::to_string(olc::Key::Y));
        (*this->vm)("KEY_Z = " + std::to_string(olc::Key::Z));
        //按键F1-F12:
        (*this->vm)("KEY_F1 = " + std::to_string(olc::Key::F1));
        (*this->vm)("KEY_F2 = " + std::to_string(olc::Key::F2));
        (*this->vm)("KEY_F3 = " + std::to_string(olc::Key::F3));
        (*this->vm)("KEY_F4 = " + std::to_string(olc::Key::F4));
        (*this->vm)("KEY_F5 = " + std::to_string(olc::Key::F5));
        (*this->vm)("KEY_F6 = " + std::to_string(olc::Key::F6));
        (*this->vm)("KEY_F7 = " + std::to_string(olc::Key::F7));
        (*this->vm)("KEY_F8 = " + std::to_string(olc::Key::F8));
        (*this->vm)("KEY_F9 = " + std::to_string(olc::Key::F9));
        (*this->vm)("KEY_F10 = " + std::to_string(olc::Key::F10));
        (*this->vm)("KEY_F11 = " + std::to_string(olc::Key::F11));
        (*this->vm)("KEY_F12 = " + std::to_string(olc::Key::F12));
        //方向键:
        (*this->vm)("KEY_UP = " + std::to_string(olc::Key::UP));
        (*this->vm)("KEY_DOWN = " + std::to_string(olc::Key::DOWN));
        (*this->vm)("KEY_LEFT = " + std::to_string(olc::Key::LEFT));
        (*this->vm)("KEY_RIGHT = " + std::to_string(olc::Key::RIGHT));
        //Keys:
        //A-Z:
        (*this->vm)("Key_A = " + std::to_string((int)('A')));
        (*this->vm)("Key_B = " + std::to_string((int)('B')));
        (*this->vm)("Key_C = " + std::to_string((int)('C')));
        (*this->vm)("Key_D = " + std::to_string((int)('D')));
        (*this->vm)("Key_E = " + std::to_string((int)('E')));
        (*this->vm)("Key_F = " + std::to_string((int)('F')));
        (*this->vm)("Key_G = " + std::to_string((int)('G')));
        (*this->vm)("Key_H = " + std::to_string((int)('H')));
        (*this->vm)("Key_I = " + std::to_string((int)('I')));
        (*this->vm)("Key_J = " + std::to_string((int)('J')));
        (*this->vm)("Key_K = " + std::to_string((int)('K')));
        (*this->vm)("Key_L = " + std::to_string((int)('L')));
        (*this->vm)("Key_M = " + std::to_string((int)('M')));
        (*this->vm)("Key_N = " + std::to_string((int)('N')));
        (*this->vm)("Key_O = " + std::to_string((int)('O')));
        (*this->vm)("Key_P = " + std::to_string((int)('P')));
        (*this->vm)("Key_Q = " + std::to_string((int)('Q')));
        (*this->vm)("Key_R = " + std::to_string((int)('R')));
        (*this->vm)("Key_S = " + std::to_string((int)('S')));
        (*this->vm)("Key_T = " + std::to_string((int)('T')));
        (*this->vm)("Key_U = " + std::to_string((int)('U')));
        (*this->vm)("Key_V = " + std::to_string((int)('V')));
        (*this->vm)("Key_W = " + std::to_string((int)('W')));
        (*this->vm)("Key_X = " + std::to_string((int)('X')));
        (*this->vm)("Key_Y = " + std::to_string((int)('Y')));
        (*this->vm)("Key_Z = " + std::to_string((int)('Z')));
        //按键F1-F12:
        (*this->vm)("Key_F1 = " + std::to_string(VK_F1));
        (*this->vm)("Key_F2 = " + std::to_string(VK_F2));
        (*this->vm)("Key_F3 = " + std::to_string(VK_F3));
        (*this->vm)("Key_F4 = " + std::to_string(VK_F4));
        (*this->vm)("Key_F5 = " + std::to_string(VK_F5));
        (*this->vm)("Key_F6 = " + std::to_string(VK_F6));
        (*this->vm)("Key_F7 = " + std::to_string(VK_F7));
        (*this->vm)("Key_F8 = " + std::to_string(VK_F8));
        (*this->vm)("Key_F9 = " + std::to_string(VK_F9));
        (*this->vm)("Key_F10 = " + std::to_string(VK_F10));
        (*this->vm)("Key_F11 = " + std::to_string(VK_F11));
        (*this->vm)("Key_F12 = " + std::to_string(VK_F12));
        //方向键:
        (*this->vm)("Key_Up = " + std::to_string(VK_UP));
        (*this->vm)("Key_Down = " + std::to_string(VK_DOWN));
        (*this->vm)("Key_Left = " + std::to_string(VK_LEFT));
        (*this->vm)("Key_Right = " + std::to_string(VK_RIGHT));
        //Space, Enter, Shift, Tab, Backspace, Escape, Ctrl, Alt, Caps:
        (*this->vm)("Key_Space = " + std::to_string(VK_SPACE));
        (*this->vm)("Key_Enter = " + std::to_string(VK_RETURN));
        (*this->vm)("Key_Shift = " + std::to_string(VK_SHIFT));
        (*this->vm)("Key_Tab = " + std::to_string(VK_TAB));
        (*this->vm)("Key_BackSpace = " + std::to_string(VK_BACK));
        (*this->vm)("Key_Escape = " + std::to_string(VK_ESCAPE));
        (*this->vm)("Key_Ctrl = " + std::to_string(VK_CONTROL));
        (*this->vm)("Key_Caps = " + std::to_string(VK_CAPITAL));
        (*this->vm)("Key_Alt = " + std::to_string(VK_MENU));
        //大键盘的0-9
        (*this->vm)("Key_1 = " + std::to_string((int)('1')));
        (*this->vm)("Key_2 = " + std::to_string((int)('2')));
        (*this->vm)("Key_3 = " + std::to_string((int)('3')));
        (*this->vm)("Key_4 = " + std::to_string((int)('4')));
        (*this->vm)("Key_5 = " + std::to_string((int)('5')));
        (*this->vm)("Key_6 = " + std::to_string((int)('6')));
        (*this->vm)("Key_7 = " + std::to_string((int)('7')));
        (*this->vm)("Key_8 = " + std::to_string((int)('8')));
        (*this->vm)("Key_9 = " + std::to_string((int)('9')));
        (*this->vm)("Key_0 = " + std::to_string((int)('0')));
        //小键盘的0-9:
        (*this->vm)("Key_Number1  = " + std::to_string(VK_NUMPAD1));
        (*this->vm)("Key_Number2  = " + std::to_string(VK_NUMPAD2));
        (*this->vm)("Key_Number3  = " + std::to_string(VK_NUMPAD3));
        (*this->vm)("Key_Number4  = " + std::to_string(VK_NUMPAD4));
        (*this->vm)("Key_Number5  = " + std::to_string(VK_NUMPAD5));
        (*this->vm)("Key_Number6  = " + std::to_string(VK_NUMPAD6));
        (*this->vm)("Key_Number7  = " + std::to_string(VK_NUMPAD7));
        (*this->vm)("Key_Number8  = " + std::to_string(VK_NUMPAD8));
        (*this->vm)("Key_Number9  = " + std::to_string(VK_NUMPAD9));
        (*this->vm)("Key_Number0  = " + std::to_string(VK_NUMPAD0));
        //Console APIs:
        (*this->vm)["init_console"] = &init_console;
        (*this->vm)["deinit_console"] = &deinit_console;
        (*this->vm)["write_console"] = &write_console;
        (*this->vm)["writeline_console"] = &writeline_console;
        (*this->vm)["readkey_console"] = &readkey_console;
        (*this->vm)["readline_console"] = &readline_console;
        //Font APIs:
        (*this->vm)["init_font"] = &init_font;
        (*this->vm)["deinit_font"] = &deinit_font;
        (*this->vm)["draw_font"] = &draw_font;
        (*this->vm)["get_font_width"] = &get_font_width;
        (*this->vm)["get_font_height"] = &get_font_height;
        (*this->vm)["get_font_offset_x"] = &get_font_offset_x;
        (*this->vm)["get_font_offset_y"] = &get_font_offset_y;
    }
}