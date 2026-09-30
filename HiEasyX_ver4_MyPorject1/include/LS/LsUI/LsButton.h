#ifndef LsUI_Button 

#pragma once
#include <graphics.h>
#include <functional>
#include <string>
#include "LsDPIManager.h"

namespace Ls_UI
{
    // 按钮样式枚举
    enum class ButtonStyle
    {
        Classic,        // 经典平面按钮
        Modern,         // 现代扁平按钮
        Round,          // 圆角按钮
        Pill,           // 药丸形按钮
        Outline,        // 边框按钮
        Ghost,          // 幽灵按钮（透明背景）
        Glossy,         // 光泽质感按钮
        Neon            // 霓虹发光按钮
    };

    class LsButton
    {
    private:
        // 几何属性
        int m_x, m_y;
        int m_width, m_height;
        int m_radius;           // 圆角半径

        // 文本属性
        std::wstring m_text;
        int m_fontSize;
        const wchar_t* m_fontFamily;

        // 交互状态
        bool m_isHover;
        bool m_isPressed;
        bool m_isEnabled;
        bool m_isChecked;       // 切换状态（用于开关按钮）
        bool m_checkable;       // 是否可切换

        // 颜色主题
        COLORREF m_bgColor;
        COLORREF m_hoverColor;
        COLORREF m_pressColor;
        COLORREF m_disabledColor;
        COLORREF m_textColor;
        COLORREF m_textHoverColor;
        COLORREF m_borderColor;
        COLORREF m_borderHoverColor;

        // 样式属性
        ButtonStyle m_style;
        int m_borderWidth;
        bool m_hasShadow;
        bool m_hasIcon;
        IMAGE* m_iconImage;
        int m_iconSize;

        // 发光效果（霓虹风格）
        bool m_hasGlow;
        COLORREF m_glowColor;
        int m_glowRadius;

        // 回调函数
        std::function<void()> m_onClick;
        std::function<void(bool)> m_onToggle;  // 切换按钮专用

        // 辅助方法
        void DrawClassic();
        void DrawModern();
        void DrawRound();
        void DrawPill();
        void DrawOutline();
        void DrawGhost();
        void DrawGlossy();
        void DrawNeon();
        void DrawShadow(int x, int y, int w, int h, int radius);
        void DrawGlow(int x, int y, int w, int h, int radius);

        int GetScaledX() const { return DPIManager::ScaleX(m_x); }
        int GetScaledY() const { return DPIManager::ScaleY(m_y); }
        int GetScaledWidth() const { return DPIManager::ScaleX(m_width); }
        int GetScaledHeight() const { return DPIManager::ScaleY(m_height); }
        int GetScaledRadius() const { return DPIManager::Scale(m_radius); }
        int GetScaledFontSize() const { return DPIManager::Scale(m_fontSize); }

    public:
        LsButton();
        ~LsButton() = default;

        // 创建按钮
        void Create(int x, int y, int width, int height,
            const wchar_t* text = L"Button",
            int fontSize = 16,
            const wchar_t* fontFamily = L"微软雅黑");

        // 设置样式
        void SetStyle(ButtonStyle style);
        void SetRadius(int radius);
        void SetBorderWidth(int width);
        void SetShadow(bool enable);
        void SetIcon(IMAGE* icon, int size = 24);

        // 霓虹风格特效
        void EnableGlow(COLORREF color = RGB(0, 255, 150), int radius = 10);
        void DisableGlow();

        // 切换按钮模式
        void SetCheckable(bool checkable);
        void SetChecked(bool checked);
        bool IsChecked() const { return m_isChecked; }

        // 交互
        bool OnMouseMove(int mouseX, int mouseY);
        bool OnMouseDown(int mouseX, int mouseY);
        void OnMouseUp();
        void OnMouseLeave();

        // 绘制
        void Draw();

        // 启用/禁用
        void SetEnabled(bool enabled);
        bool IsEnabled() const { return m_isEnabled; }

        // 设置文本
        void SetText(const wchar_t* text);

        // 设置颜色主题
        void SetTheme(COLORREF bg, COLORREF hover, COLORREF press,
            COLORREF text, COLORREF border);

        // 预设主题
        void SetDefaultTheme();     // 默认蓝灰色
        void SetDarkTheme();        // 暗黑主题
        void SetSuccessTheme();     // 成功绿色
        void SetDangerTheme();      // 危险红色
        void SetWarningTheme();     // 警告橙色
        void SetInfoTheme();        // 信息青色
        void SetNeonTheme();        // 霓虹主题

        // 回调注册
        void OnClick(std::function<void()> callback);
        void OnToggle(std::function<void(bool)> callback);

        // 获取边界
        RECT GetPixelRect() const;
        bool IsPointInside(int x, int y) const;
    };
}

#endif