﻿#include "../Random/random.hpp"
#include "LuaBinding.hpp"
#include "Audio.hpp"
#include "String.hpp"
#include "File.hpp"
#include "Vector2.hpp"
#include "Debug.hpp"
#include "Input.hpp"
#include "StringConverter.hpp"
#include "UIObject.hpp"
#include "UIButton.hpp"
#include "UIInputField.hpp"
#include "Vector.hpp"
#include "EngineAPI.hpp"
#include "Video.hpp"
#include <string>
#include <vector>
#include <cmath>

using namespace std;

using __Random = effolkronium::random_static;

namespace ClashEngine
{
    //=====================Math APIs=====================

    static int double2int(double number)
    {
        return (int)number;
    }

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

    //=====================Random APIs=====================

    //get a random int value [min, max]
    static int random_range(int min, int max)
    {
        return __Random::get(min, max);
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

    //该API需要写在start里面以防止engine为nullptr的情况
    static void set_window_title(const string& title)
    {
        //UTF8 => UTF16, UTF16=>Default
        wstring wtitle = String::StringToWstring(title, Encoding::UTF8);
        LuaBinding::engine->sAppName = String::WstringToString(wtitle, Encoding::Default);
    }

    static int get_screen_width()
    {
        return LuaBinding::engine->ScreenWidth();
    }

    static int get_screen_height()
    {
        return LuaBinding::engine->ScreenHeight();
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
        EngineAPI::DrawLine(LuaBinding::engine, x1, y1, x2, y2, r, g, b, thick);
    }

    static void draw_rect(int x, int y, int w, int h, int r, int g, int b)
    {
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                LuaBinding::engine->Draw(j + x, i + y, olc::Pixel(r, g, b));
            }
        }
    }

    static void draw_frame(int x, int y, int w, int h, int r, int g, int b)
    {
        LuaBinding::engine->DrawRect(x, y, w, h, olc::Pixel(r, g, b));
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

    static void draw_png_image(int x, int y, olc::Sprite* sprite)
    {
        LuaBinding::engine->SetPixelMode(olc::Pixel::ALPHA);
        LuaBinding::engine->DrawSprite(x, y, sprite);
        LuaBinding::engine->SetPixelMode(olc::Pixel::NORMAL);
    }

    //基于等间隔采样的图像缩放算法:https://blog.csdn.net/qq_37394634/article/details/99675686
    static void draw_image_scaling(int x, int y, olc::Sprite* sprite, int w, int h)
    {
        EngineAPI::DrawSprite(LuaBinding::engine, x, y, sprite, w, h);
    }

    //基于等间隔采样的图像缩放算法:https://blog.csdn.net/qq_37394634/article/details/99675686
    static void draw_image_scalingf(int x, int y, olc::Sprite* sprite, double sx, double sy)
    {
        EngineAPI::DrawSprite(LuaBinding::engine, x, y, sprite, sx, sy);
    }

    //基于等间隔采样的图像缩放算法:https://blog.csdn.net/qq_37394634/article/details/99675686
    static void draw_png_image_scaling(int x, int y, olc::Sprite* sprite, int w, int h)
    {
        LuaBinding::engine->SetPixelMode(olc::Pixel::ALPHA);
        draw_image_scaling(x, y, sprite, w, h);
        LuaBinding::engine->SetPixelMode(olc::Pixel::NORMAL);
    }

    //基于等间隔采样的图像缩放算法:https://blog.csdn.net/qq_37394634/article/details/99675686
    static void draw_png_image_scalingf(int x, int y, olc::Sprite* sprite, double sx, double sy)
    {
        LuaBinding::engine->SetPixelMode(olc::Pixel::ALPHA);
        draw_image_scalingf(x, y, sprite, sx, sy);
        LuaBinding::engine->SetPixelMode(olc::Pixel::NORMAL);
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

    //interval:字符与字符之间间隔的像素数量
    static void draw_font_ex(olc::Font* font, const string& s, int x, int y, int r, int g, int b, int interval)
    {
        int nextX = x;
        wstring ws = String::StringToWstring(s, Encoding::UTF8);
        for (const wchar& item : ws)
        {
            string str = String::WstringToString(wstring(1, item), Encoding::UTF8);
            u32string u32String = StringConverter::To_UTF32(str);
            //new:
            olc::Sprite* charSprite = font->RenderStringToSprite(u32String, olc::Pixel(r, g, b));
            //draw sprite:
            draw_png_image(nextX, y, charSprite);
            nextX += (interval + charSprite->width);
            //delete:
            delete charSprite;
        }
    }

    //!!!注意:渲染该olc::Sprite*需要开启Alpha混合
    //!!!需要回收返回值
    static olc::Sprite* render_font_to_sprite(olc::Font* font, const string& s, int r, int g, int b)
    {
        return font->RenderStringToSprite(StringConverter::To_UTF32(s), olc::Pixel(r, g, b));
    }

    //!!!注意:渲染该olc::Sprite*需要开启Alpha混合
    //!!!需要回收返回值
    //interval:字符与字符之间间隔的像素数量
    static olc::Sprite* render_font_to_sprite_ex(olc::Font* font, const string& s, int r, int g, int b, int interval)
    {
        wstring ws = String::StringToWstring(s, Encoding::UTF8);
        int fontWidth = font->GetStringBounds(StringConverter::To_UTF32(s)).size.x;
        int fontHeight = font->GetStringBounds(StringConverter::To_UTF32(s)).size.y;

        int spriteWidth = fontWidth + (ws.size() - 1) * interval;
        int spriteHeight = fontHeight;
        olc::Sprite* fontSprite = new olc::Sprite(spriteWidth, spriteHeight);

        int x = 0;
        for (const wchar& item : ws)
        {
            string str = String::WstringToString(wstring(1, item), Encoding::UTF8);
            u32string u32String = StringConverter::To_UTF32(str);
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

    //=====================UI APIs=====================

    static bool ui_active(UIObject* ui)
    {
        return ui->GetActive();
    }

    static void enable_ui(UIObject* ui)
    {
        ui->SetActive(true);
    }

    static void disable_ui(UIObject* ui)
    {
        ui->SetActive(false);
    }

    static UIObject* get_ui_parent(UIObject* ui)
    {
        return ui->GetParent();
    }

    static void set_ui_parent(UIObject* ui, UIObject* parent)
    {
        ui->SetParent(parent);
    }

    static Vector2 get_ui_local_pos(UIObject* ui)
    {
        return ui->GetLocalPosition();
    }

    static void set_ui_local_pos(UIObject* ui, int x, int y)
    {
        ui->SetLocalPosition(Vector2(x, y));
    }

    static Vector2 get_ui_pos(UIObject* ui)
    {
        return ui->GetPosition();
    }

    static void set_ui_pos(UIObject* ui, int x, int y)
    {
        ui->SetPosition(Vector2(x, y));
    }

    static Vector2 get_ui_size(UIObject* ui)
    {
        return ui->GetSize();
    }

    static void set_ui_size(UIObject* ui, Vector2 size)
    {
        ui->SetSize(size);
    }

    static void draw_ui(UIObject* ui)
    {
        if (!ui->GetActive()) return;
        //如果没有提供重写的OnDraw函数则调用默认的OnDraw函数
        if (String::Compare(ui->on_draw, ""))
        {
            ui->OnDraw(LuaBinding::engine);
        }
        else
        {
            //执行Lua回调函数:
            (*LuaBinding::state)[ui->on_draw].call<void>(ui);
        }
    }

    static void bind_mouse_enter(UIObject* ui, const string& function)
    {
        ui->on_mouse_enter = function;
    }

    static void bind_mouse_stay(UIObject* ui, const string& function)
    {
        ui->on_mouse_stay = function;
    }

    static void bind_mouse_exit(UIObject* ui, const string& function)
    {
        ui->on_mouse_exit = function;
    }

    static void bind_mouse_click(UIObject* ui, const string& function)
    {
        ui->on_mouse_click = function;
    }

    static void bind_draw(UIObject* ui, const string& function)
    {
        ui->on_draw = function;
    }

    //=====================Button APIs=====================

    static UIButton* init_button(int x, int y, int w, int h)
    {
        UIButton* button = new UIButton(Vector2(x, y), Vector2(w, h));
        LuaBinding::ui_objects.push_back(button);
        return button;
    }

    static void deinit_button(UIButton* button)
    {
        Vector<UIObject*>::Remove(LuaBinding::ui_objects, button);
        delete button;
    }

    //=====================InputField APIs=====================

    static UIInputField* init_input_field(int x, int y, int w, int h, olc::Font* font)
    {
        UIInputField* inputField = new UIInputField(Vector2(x, y), Vector2(w, h), font);
        LuaBinding::ui_objects.push_back(inputField);
        return inputField;
    }

    static void deinit_input_field(UIInputField* inputField)
    {
        Vector<UIObject*>::Remove(LuaBinding::ui_objects, inputField);
        delete inputField;
    }

    static std::string get_input_field_data(UIInputField* inputField)
    {
        std::wstring inputData = inputField->GetInputData();
        std::string data = String::WstringToString(inputData, Encoding::UTF8);
        return data;
    }

    //=====================Video APIs=====================

    static Video* init_video(const string& path)
    {
        wstring wpath = File::GetAbsolutePath(String::StringToWstring(path, Encoding::UTF8));
        Video* video = new Video(wpath);
        return video;
    }

    static void deinit_video(Video* video)
    {
        delete video;
    }

    static int get_video_width(Video* video)
    {
        return video->GetWidth();
    }

    static int get_video_height(Video* video)
    {
        return video->GetHeight();
    }

    //不需要释放该返回值, 该Sprite会在Video对象销毁时自动销毁
    static olc::Sprite* get_video_next_frame(Video* video)
    {
        return video->GetNextFrame();
    }

    kaguya::State* LuaBinding::state = nullptr;
    int LuaBinding::screenWidth = 0;
    int LuaBinding::screenHeight = 0;
    bool LuaBinding::inited = false;
    olc::PixelGameEngine* LuaBinding::engine = nullptr;
    Console* LuaBinding::console = nullptr;
    bool LuaBinding::console_active = false;
    bool LuaBinding::attched_console = false;
    std::vector<UIObject*> LuaBinding::ui_objects;

    LuaBinding::LuaBinding(kaguya::State* vm)
    {
        this->vm = vm;
    }

    void LuaBinding::Registe()
    {
        //Registering Vector2:
        //usage:
        //default  Constructor  : pos = Vector2.new()
        //int, int Constructor2 : pos = Vector2.new(10, 10)
        //x value               : pos_x = pos:x()
        //y value               : pos_y = pos:y()
        (*this->vm)["Vector2"].setClass
        (
            kaguya::UserdataMetatable<Vector2>()
            .setConstructors<Vector2(), Vector2(int, int)>()
            .addFunction("x", &Vector2::x)
            .addFunction("y", &Vector2::y)
        );
        //Math APIs:
        (*this->vm)["double2int"] = &double2int;
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
        //Random APIs:
        (*this->vm)["random_range"] = &random_range;
        //Engine APIs:
        (*this->vm)["init_engine"] = &init_engine;
        (*this->vm)["deinit_engine"] = &deinit_engine;
        (*this->vm)["get_delta_time"] = &get_delta_time;
        (*this->vm)["set_window_title"] = &set_window_title;
        (*this->vm)["get_screen_width"] = &get_screen_width;
        (*this->vm)["get_screen_height"] = &get_screen_height;
        //Draw APIs:
        (*this->vm)["clear"] = &clear;
        (*this->vm)["clear_color"] = &clear_color;
        (*this->vm)["draw_pixel"] = &draw_pixel;
        (*this->vm)["draw_line"] = &draw_line;
        (*this->vm)["draw_line_bold"] = &draw_line_bold;
        (*this->vm)["draw_rect"] = &draw_rect;
        (*this->vm)["draw_frame"] = &draw_frame;
        //Image APIs:
        (*this->vm)["init_image"] = &init_image;
        (*this->vm)["deinit_image"] = &deinit_image;
        (*this->vm)["draw_image"] = &draw_image;
        (*this->vm)["draw_png_image"] = &draw_png_image;
        (*this->vm)["draw_image_scaling"] = &draw_image_scaling;
        (*this->vm)["draw_image_scalingf"] = &draw_image_scalingf;
        (*this->vm)["draw_png_image_scaling"] = &draw_png_image_scaling;
        (*this->vm)["draw_png_image_scalingf"] = &draw_png_image_scalingf;
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
        //Space, Enter, Shift, Tab, Backspace, Escape, Ctrl, Alt, Caps, Home, End, Delete:
        (*this->vm)("Key_Space = " + std::to_string(VK_SPACE));
        (*this->vm)("Key_Enter = " + std::to_string(VK_RETURN));
        (*this->vm)("Key_Shift = " + std::to_string(VK_SHIFT));
        (*this->vm)("Key_Tab = " + std::to_string(VK_TAB));
        (*this->vm)("Key_BackSpace = " + std::to_string(VK_BACK));
        (*this->vm)("Key_Escape = " + std::to_string(VK_ESCAPE));
        (*this->vm)("Key_Ctrl = " + std::to_string(VK_CONTROL));
        (*this->vm)("Key_Caps = " + std::to_string(VK_CAPITAL));
        (*this->vm)("Key_Alt = " + std::to_string(VK_MENU));
        (*this->vm)("Key_Home = " + std::to_string(VK_HOME));
        (*this->vm)("Key_End = " + std::to_string(VK_END));
        (*this->vm)("Key_Delete = " + std::to_string(VK_DELETE));
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
        (*this->vm)("Key_Number1 = " + std::to_string(VK_NUMPAD1));
        (*this->vm)("Key_Number2 = " + std::to_string(VK_NUMPAD2));
        (*this->vm)("Key_Number3 = " + std::to_string(VK_NUMPAD3));
        (*this->vm)("Key_Number4 = " + std::to_string(VK_NUMPAD4));
        (*this->vm)("Key_Number5 = " + std::to_string(VK_NUMPAD5));
        (*this->vm)("Key_Number6 = " + std::to_string(VK_NUMPAD6));
        (*this->vm)("Key_Number7 = " + std::to_string(VK_NUMPAD7));
        (*this->vm)("Key_Number8 = " + std::to_string(VK_NUMPAD8));
        (*this->vm)("Key_Number9 = " + std::to_string(VK_NUMPAD9));
        (*this->vm)("Key_Number0 = " + std::to_string(VK_NUMPAD0));
        //计算符号
        (*this->vm)("Key_Multiply = " + std::to_string(VK_MULTIPLY));
        (*this->vm)("Key_Subtract = " + std::to_string(VK_SUBTRACT));
        (*this->vm)("Key_Add = " + std::to_string(VK_ADD));
        (*this->vm)("Key_Divide = " + std::to_string(VK_DIVIDE));
        (*this->vm)("Key_Decimal = " + std::to_string(VK_DECIMAL));
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
        (*this->vm)["draw_font_ex"] = &draw_font_ex;
        (*this->vm)["render_font_to_sprite"] = &render_font_to_sprite;
        (*this->vm)["render_font_to_sprite_ex"] = &render_font_to_sprite_ex;
        (*this->vm)["get_font_width"] = &get_font_width;
        (*this->vm)["get_font_height"] = &get_font_height;
        (*this->vm)["get_font_offset_x"] = &get_font_offset_x;
        (*this->vm)["get_font_offset_y"] = &get_font_offset_y;
        //UI APIs:
        (*this->vm)["ui_active"] = &ui_active;
        (*this->vm)["enable_ui"] = &enable_ui;
        (*this->vm)["disable_ui"] = &disable_ui;
        (*this->vm)["get_ui_parent"] = &get_ui_parent;
        (*this->vm)["set_ui_parent"] = &set_ui_parent;
        (*this->vm)["get_ui_local_pos"] = &get_ui_local_pos;
        (*this->vm)["set_ui_local_pos"] = &set_ui_local_pos;
        (*this->vm)["get_ui_pos"] = &get_ui_pos;
        (*this->vm)["set_ui_pos"] = &set_ui_pos;
        (*this->vm)["get_ui_size"] = &get_ui_size;
        (*this->vm)["set_ui_size"] = &set_ui_size;
        (*this->vm)["draw_ui"] = &draw_ui;
        (*this->vm)["bind_mouse_enter"] = &bind_mouse_enter;
        (*this->vm)["bind_mouse_stay"] = &bind_mouse_stay;
        (*this->vm)["bind_mouse_exit"] = &bind_mouse_exit;
        (*this->vm)["bind_mouse_click"] = &bind_mouse_click;
        (*this->vm)["bind_draw"] = &bind_draw;
        //Button APIs:
        (*this->vm)["init_button"] = &init_button;
        (*this->vm)["deinit_button"] = &deinit_button;
        //InputField APIs:
        (*this->vm)["init_input_field"] = &init_input_field;
        (*this->vm)["deinit_input_field"] = &deinit_input_field;
        (*this->vm)["get_input_field_data"] = &get_input_field_data;
        //Video APIs:
        (*this->vm)["init_video"] = &init_video;
        (*this->vm)["deinit_video"] = &deinit_video;
        (*this->vm)["get_video_width"] = &get_video_width;
        (*this->vm)["get_video_height"] = &get_video_height;
        (*this->vm)["get_video_next_frame"] = &get_video_next_frame;
    }
}