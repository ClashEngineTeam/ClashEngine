#pragma once

#include "../Kaguya/kaguya.hpp"
#include "../olcPixelGameEngine/olcPixelGameEngine.h"
#include "../olcPGEX_TTF/olcPGEX_TTF.h"
#include "Vector2.hpp"
#include "String.hpp"
#include "Vector.hpp"
#include <vector>
#include <string>

namespace ClashEngine
{
    //所有UI物体的基类
    //特性:
    //disable(禁用)某个物体会导致该物体的所有子物体也不会被渲染
    //parent(父物体可以为空), 当一个物体设置了父物体后, 其localPosition代表Position - parent.Position
    //localPosition(本地坐标)是相对于父物体的坐标，如果没有父物体则表示相对于原点(左上角)的坐标
    //position(坐标)是相对于原点(左上角)的坐标
    class UIObject
    {
    public:
        //回调函数名:
        std::string on_mouse_enter;
        std::string on_mouse_stay;
        std::string on_mouse_exit;
        std::string on_mouse_click;
        std::string on_draw;

    private:
        bool active; //是否激活, 引擎不会渲染非激活的UI物体
        UIObject* parent; //父物体
        Vector2 localPosition; //本地坐标
        Vector2 size; //尺寸
        std::vector<UIObject*> children; //子物体集合

        bool entered = false;

    public:
        UIObject(Vector2 position, Vector2 size)
        {
            this->active = true;
            this->parent = nullptr;
            this->localPosition = position;
            this->size = size;
        }

        bool GetActive()
        {
            if (!this->active) return false;

            UIObject* temp = this->parent;
            while (temp != nullptr)
            {
                if (!temp->active)
                {
                    return false;
                }
                temp = temp->parent;
            }

            return true;
        }

        void SetActive(bool active)
        {
            this->active = active;
        }

        UIObject* GetParent()
        {
            return this->parent;
        }

        void SetParent(UIObject* parent)
        {
            //不设置父物体
            if (parent == nullptr)
            {
                this->localPosition = this->GetPosition();
            }
            //设置父物体
            else
            {
                this->localPosition = this->GetPosition() - parent->GetPosition();
            }
            //将自己从旧的父物体的子物体集合中移除:
            if (this->parent != nullptr)
            {
                Vector<UIObject*>::Remove(this->parent->children, this);
            }
            //设置新的父物体:
            this->parent = parent;
        }

        Vector2 GetLocalPosition()
        {
            return this->localPosition;
        }

        void SetLocalPosition(Vector2 localPosition)
        {
            this->localPosition = localPosition;
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

        Vector2 GetSize()
        {
            return this->size;
        }

        void SetSize(Vector2 size)
        {
            this->size = size;
        }

    public:
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
                        //执行Lua回调函数:
                        (*state)[on_mouse_enter].call<void>(this);
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
                    //执行Lua回调函数:
                    (*state)[on_mouse_stay].call<void>(this);
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
                        //执行Lua回调函数:
                        (*state)[on_mouse_click].call<void>(this);
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
                        //执行Lua回调函数:
                        (*state)[on_mouse_exit].call<void>(this);
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