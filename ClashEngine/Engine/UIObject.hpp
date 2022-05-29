#pragma once

#include "../Kaguya/kaguya.hpp"
#include "../olcPixelGameEngine/olcPixelGameEngine.h"
#include "../olcPGEX_TTF/olcPGEX_TTF.h"
#include "Vector2.hpp"
#include "String.hpp"
#include <vector>
#include <string>

namespace ClashEngine
{
    //所有UI物体的基类
    class UIObject
    {
    public:
        bool active; //是否激活, 引擎不会渲染非激活的UI物体

        UIObject* parent; //父物体

        std::vector<UIObject*> children; //子物体集合

        Vector2 localPosition; //本地坐标

        Vector2 size; //尺寸

        //回调函数名:
        std::string on_mouse_enter;
        std::string on_mouse_stay;
        std::string on_mouse_exit;
        std::string on_mouse_click;
        std::string on_draw;

    private:
        bool entered = false;

    public:
        UIObject(Vector2 position, Vector2 size)
        {
            this->active = true;
            this->parent = nullptr;
            this->SetPosition(position);
            this->size = size;
        }

        Vector2 GetPosition()
        {
            Vector2 position = Vector2::zero;
            UIObject* temp = this->parent;
            while (temp != nullptr)
            {
                position = position + temp->localPosition;
                temp = temp->parent;
            }
            return position + localPosition;
        }

        void SetPosition(Vector2 pos)
        {
            if (this->parent == nullptr)
            {
                localPosition = pos;
            }
            else
            {
                localPosition = pos - parent->GetPosition();
            }
        }

        void Update(olc::PixelGameEngine* engine, kaguya::State* state)
        {
            int mouseX = engine->GetMouseX();
            int mouseY = engine->GetMouseY();
            int posX = this->GetPosition().x;
            int posY = this->GetPosition().y;
            //鼠标进入范围:
            if (mouseX >= posX && mouseX < posX + size.x &&
                mouseY >= posY && mouseY < posY + size.y)
            {
                if (!entered)
                {
                    //如果没有提供重写的OnMouseEnter函数则调用默认的OnMouseEnter函数
                    if (String::Compare(on_mouse_enter, ""))
                    {
                        OnMouseEnter(engine);
                    }
                    else
                    {
                        (*state)[on_mouse_enter].call<void>();
                    }
                    entered = true;
                }

                //如果没有提供重写的OnMouseStay函数则调用默认的OnMouseStay函数
                if (String::Compare(on_mouse_stay, ""))
                {
                    OnMouseStay(engine);
                }
                else
                {
                    (*state)[on_mouse_stay].call<void>();
                }

                bool click_down = engine->GetMouse(olc::Mouse::LEFT).bPressed;
                if (click_down)
                {
                    //如果没有提供重写的OnMouseClick函数则调用默认的OnMouseClick函数
                    if (String::Compare(on_mouse_click, ""))
                    {
                        OnMouseClick(engine);
                    }
                    else
                    {
                        (*state)[on_mouse_click].call<void>();
                    }
                }
            }
            //不在范围里面
            else
            {
                if (entered)
                {
                    //如果没有提供重写的OnMouseExit函数则调用默认的OnMouseExit函数
                    if (String::Compare(on_mouse_exit, ""))
                    {
                        OnMouseExit(engine);
                    }
                    else
                    {
                        (*state)[on_mouse_exit].call<void>();
                    }
                    entered = false;
                }
            }
        }

        virtual void OnMouseEnter(olc::PixelGameEngine* engine)
        {
        }

        virtual void OnMouseStay(olc::PixelGameEngine* engine)
        {
        }

        virtual void OnMouseExit(olc::PixelGameEngine* engine)
        {
        }

        virtual void OnMouseClick(olc::PixelGameEngine* engine)
        {
        }

        virtual void OnDraw(olc::PixelGameEngine* engine) = 0;
    };
}