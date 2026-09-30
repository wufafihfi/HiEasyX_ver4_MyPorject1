#include "LsButton.h"
#include <algorithm>

namespace Ls_UI
{
    LsButton::LsButton()
        : m_x(0), m_y(0), m_width(100), m_height(36), m_radius(6)
        , m_text(L"Button"), m_fontSize(16), m_fontFamily(L"微软雅黑")
        , m_isHover(false), m_isPressed(false), m_isEnabled(true)
        , m_isChecked(false), m_checkable(false)
        , m_bgColor(RGB(70, 130, 220))
        , m_hoverColor(RGB(100, 160, 250))
        , m_pressColor(RGB(50, 100, 200))
        , m_disabledColor(RGB(180, 180, 180))
        , m_textColor(RGB(255, 255, 255))
        , m_textHoverColor(RGB(255, 255, 255))
        , m_borderColor(RGB(70, 130, 220))
        , m_borderHoverColor(RGB(100, 160, 250))
        , m_style(ButtonStyle::Modern)
        , m_borderWidth(1)
        , m_hasShadow(true)
        , m_hasIcon(false)
        , m_iconImage(nullptr)
        , m_iconSize(24)
        , m_hasGlow(false)
        , m_glowColor(RGB(0, 255, 150))
        , m_glowRadius(10)
        , m_onClick(nullptr)
        , m_onToggle(nullptr)
    {
    }

    void LsButton::Create(int x, int y, int width, int height,
        const wchar_t* text, int fontSize, const wchar_t* fontFamily)
    {
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;
        m_text = text;
        m_fontSize = fontSize;
        m_fontFamily = fontFamily;
    }

    void LsButton::SetStyle(ButtonStyle style)
    {
        m_style = style;

        // 根据样式调整默认属性
        switch (style)
        {
        case ButtonStyle::Round:
            m_radius = m_height / 2;
            break;
        case ButtonStyle::Pill:
            m_radius = m_height;
            break;
        case ButtonStyle::Outline:
        case ButtonStyle::Ghost:
            m_borderWidth = 2;
            break;
        default:
            break;
        }
    }

    void LsButton::SetRadius(int radius)
    {
        m_radius = radius;
    }

    void LsButton::SetBorderWidth(int width)
    {
        m_borderWidth = width;
    }

    void LsButton::SetShadow(bool enable)
    {
        m_hasShadow = enable;
    }

    void LsButton::SetIcon(IMAGE* icon, int size)
    {
        m_hasIcon = (icon != nullptr);
        m_iconImage = icon;
        m_iconSize = size;
    }

    void LsButton::EnableGlow(COLORREF color, int radius)
    {
        m_hasGlow = true;
        m_glowColor = color;
        m_glowRadius = DPIManager::ScaleX(radius);
    }

    void LsButton::DisableGlow()
    {
        m_hasGlow = false;
    }

    void LsButton::SetCheckable(bool checkable)
    {
        m_checkable = checkable;
        if (!checkable) m_isChecked = false;
    }

    void LsButton::SetChecked(bool checked)
    {
        if (m_checkable)
        {
            m_isChecked = checked;
            if (m_onToggle) m_onToggle(m_isChecked);
        }
    }

    bool LsButton::OnMouseMove(int mouseX, int mouseY)
    {
        if (!m_isEnabled) return false;

        bool wasHover = m_isHover;
        m_isHover = IsPointInside(mouseX, mouseY);

        return m_isHover;
    }

    bool LsButton::OnMouseDown(int mouseX, int mouseY)
    {
        if (!m_isEnabled) return false;

        if (IsPointInside(mouseX, mouseY))
        {
            m_isPressed = true;
            return true;
        }
        return false;
    }

    void LsButton::OnMouseUp()
    {
        if (m_isPressed && m_isEnabled)
        {
            if (m_checkable)
            {
                m_isChecked = !m_isChecked;
                if (m_onToggle) m_onToggle(m_isChecked);
            }
            if (m_onClick) m_onClick();
        }
        m_isPressed = false;
    }

    void LsButton::OnMouseLeave()
    {
        m_isHover = false;
        m_isPressed = false;
    }

    void LsButton::DrawShadow(int x, int y, int w, int h, int radius)
    {
        if (!m_hasShadow || !m_isEnabled) return;

        COLORREF shadowColor = RGB(0, 0, 0);
        setlinecolor(shadowColor);
        setfillcolor(shadowColor);

        for (int i = 1; i <= 3; i++)
        {
            int alpha = 20 - i * 5;
            setfillcolor(RGB(0, 0, 0));
            fillroundrect(x + i, y + i, x + w + i, y + h + i, radius, radius);
        }
    }

    void LsButton::DrawGlow(int x, int y, int w, int h, int radius)
    {
        if (!m_hasGlow || !m_isHover) return;

        // 保存当前绘图状态
        int oldLineColor = getlinecolor();
        LINESTYLE oldLineStyle;
        getlinestyle(&oldLineStyle);  // 正确的用法

        // 绘制外发光（从外向内，亮度递增）
        for (int i = m_glowRadius; i >= 1; i--)
        {
            // 计算亮度：外层暗，内层亮
            int brightness = 50 + (m_glowRadius - i) * 150 / m_glowRadius;
            brightness = min(200, brightness);

            int r = (GetRValue(m_glowColor) * brightness) / 255;
            int g = (GetGValue(m_glowColor) * brightness) / 255;
            int b = (GetBValue(m_glowColor) * brightness) / 255;

            setlinecolor(RGB(r, g, b));
            setlinestyle(PS_SOLID, max(1, (m_glowRadius - i + 1) / 2));
            roundrect(x - i, y - i, x + w + i, y + h + i, radius + i, radius + i);
        }

        // 恢复绘图状态
        setlinecolor(oldLineColor);
        setlinestyle(&oldLineStyle);  // 恢复线型
    }

    void LsButton::DrawClassic()
    {
        int x = GetScaledX();
        int y = GetScaledY();
        int w = GetScaledWidth();
        int h = GetScaledHeight();
        int r = GetScaledRadius();

        COLORREF bgColor = m_isEnabled ?
            (m_isPressed ? m_pressColor : (m_isHover ? m_hoverColor : m_bgColor))
            : m_disabledColor;

        setfillcolor(bgColor);
        setlinecolor(RGB(80, 80, 80));
        fillroundrect(x, y, x + w, y + h, r, r);

        // 添加内边框效果
        setlinecolor(RGB(255, 255, 255));
        setlinestyle(PS_SOLID, 1);
        roundrect(x + 2, y + 2, x + w - 2, y + h - 2, r - 2, r - 2);
    }

    void LsButton::DrawModern()
    {
        int x = GetScaledX();
        int y = GetScaledY();
        int w = GetScaledWidth();
        int h = GetScaledHeight();
        int r = GetScaledRadius();

        COLORREF bgColor = m_isEnabled ?
            (m_isPressed ? m_pressColor : (m_isHover ? m_hoverColor : m_bgColor))
            : m_disabledColor;

        // 阴影
        DrawShadow(x, y, w, h, r);

        // 主体
        setfillcolor(bgColor);
        setlinecolor(bgColor);
        fillroundrect(x, y, x + w, y + h, r, r);
    }

    void LsButton::DrawRound()
    {
        int x = GetScaledX();
        int y = GetScaledY();
        int w = GetScaledWidth();
        int h = GetScaledHeight();
        int r = GetScaledRadius();

        COLORREF bgColor = m_isEnabled ?
            (m_isPressed ? m_pressColor : (m_isHover ? m_hoverColor : m_bgColor))
            : m_disabledColor;

        DrawShadow(x, y, w, h, r);
        setfillcolor(bgColor);
        setlinecolor(bgColor);
        fillroundrect(x, y, x + w, y + h, r, r);
    }

    void LsButton::DrawPill()
    {
        int x = GetScaledX();
        int y = GetScaledY();
        int w = GetScaledWidth();
        int h = GetScaledHeight();
        int r = h / 2;

        COLORREF bgColor = m_isEnabled ?
            (m_isPressed ? m_pressColor : (m_isHover ? m_hoverColor : m_bgColor))
            : m_disabledColor;

        DrawShadow(x, y, w, h, r);
        setfillcolor(bgColor);
        setlinecolor(bgColor);
        fillroundrect(x, y, x + w, y + h, r, r);
    }

    void LsButton::DrawOutline()
    {
        int x = GetScaledX();
        int y = GetScaledY();
        int w = GetScaledWidth();
        int h = GetScaledHeight();
        int r = GetScaledRadius();

        COLORREF bgColor = m_isEnabled ?
            (m_isHover ? m_hoverColor : RGB(255, 255, 255))
            : RGB(240, 240, 240);
        COLORREF borderColor = m_isEnabled ?
            (m_isHover ? m_borderHoverColor : m_borderColor)
            : RGB(200, 200, 200);

        setfillcolor(bgColor);
        setlinecolor(borderColor);
        setlinestyle(PS_SOLID, m_borderWidth);
        fillroundrect(x, y, x + w, y + h, r, r);
    }

    void LsButton::DrawGhost()
    {
        int x = GetScaledX();
        int y = GetScaledY();
        int w = GetScaledWidth();
        int h = GetScaledHeight();
        int r = GetScaledRadius();

        COLORREF borderColor = m_isEnabled ?
            (m_isHover ? m_borderHoverColor : m_borderColor)
            : RGB(200, 200, 200);

        setfillcolor(RGB(0, 0, 0));
        setlinestyle(PS_SOLID, m_borderWidth);
        setlinecolor(borderColor);
        fillroundrect(x, y, x + w, y + h, r, r);
        setbkcolor(RGB(0, 0, 0));
    }

    void LsButton::DrawGlossy()
    {
        int x = GetScaledX();
        int y = GetScaledY();
        int w = GetScaledWidth();
        int h = GetScaledHeight();
        int r = GetScaledRadius();

        COLORREF bgColor = m_isEnabled ?
            (m_isPressed ? m_pressColor : (m_isHover ? m_hoverColor : m_bgColor))
            : m_disabledColor;

        DrawShadow(x, y, w, h, r);
        setfillcolor(bgColor);
        setlinecolor(bgColor);
        fillroundrect(x, y, x + w, y + h, r, r);

        // 光泽效果
        setfillcolor(RGB(255, 255, 255));
        setlinestyle(PS_SOLID, 1);
        fillroundrect(x + 3, y + 3, x + w - 3, y + h / 2, r - 3, r - 3);
    }

    void LsButton::DrawNeon()
    {
        int x = GetScaledX();
        int y = GetScaledY();
        int w = GetScaledWidth();
        int h = GetScaledHeight();
        int r = GetScaledRadius();

        DrawGlow(x, y, w, h, r);

        COLORREF bgColor = m_isEnabled ?
            (m_isPressed ? m_pressColor : (m_isHover ? m_hoverColor : m_bgColor))
            : m_disabledColor;

        setfillcolor(bgColor);
        setlinecolor(m_glowColor);
        setlinestyle(PS_SOLID, 2);
        fillroundrect(x, y, x + w, y + h, r, r);
    }

    void LsButton::Draw()
    {
        if (!m_isEnabled && m_style != ButtonStyle::Ghost)
        {
            int x = GetScaledX();
            int y = GetScaledY();
            int w = GetScaledWidth();
            int h = GetScaledHeight();
            int r = GetScaledRadius();

            setfillcolor(m_disabledColor);
            setlinecolor(m_disabledColor);
            fillroundrect(x, y, x + w, y + h, r, r);
        }
        else
        {
            switch (m_style)
            {
            case ButtonStyle::Classic: DrawClassic(); break;
            case ButtonStyle::Modern: DrawModern(); break;
            case ButtonStyle::Round: DrawRound(); break;
            case ButtonStyle::Pill: DrawPill(); break;
            case ButtonStyle::Outline: DrawOutline(); break;
            case ButtonStyle::Ghost: DrawGhost(); break;
            case ButtonStyle::Glossy: DrawGlossy(); break;
            case ButtonStyle::Neon: DrawNeon(); break;
            }
        }

        // 绘制文本
        int x = GetScaledX();
        int y = GetScaledY();
        int w = GetScaledWidth();
        int h = GetScaledHeight();

        settextstyle(GetScaledFontSize(), 0, m_fontFamily);
        settextcolor(m_isEnabled ? m_textColor : RGB(150, 150, 150));
        setbkmode(TRANSPARENT);

        int textW = textwidth(m_text.c_str());
        int textH = textheight(m_text.c_str());

        int textX = x + (w - textW) / 2;
        int textY = y + (h - textH) / 2;

        // 如果有图标，调整文本位置
        if (m_hasIcon && m_iconImage)
        {
            int iconX = x + (w - m_iconSize - textW - 10) / 2;
            int iconY = y + (h - m_iconSize) / 2;
            putimage(iconX, iconY, m_iconSize, m_iconSize, m_iconImage, 0, 0);
            textX = iconX + m_iconSize + 10;
        }

        outtextxy(textX, textY, m_text.c_str());
    }

    void LsButton::SetEnabled(bool enabled)
    {
        m_isEnabled = enabled;
        if (!enabled)
        {
            m_isHover = false;
            m_isPressed = false;
        }
    }

    void LsButton::SetText(const wchar_t* text)
    {
        m_text = text;
    }

    void LsButton::SetTheme(COLORREF bg, COLORREF hover, COLORREF press,
        COLORREF text, COLORREF border)
    {
        m_bgColor = bg;
        m_hoverColor = hover;
        m_pressColor = press;
        m_textColor = text;
        m_borderColor = border;
        m_borderHoverColor = hover;
    }

    void LsButton::SetDefaultTheme()
    {
        DisableGlow();  // 关闭发光
        SetTheme(RGB(70, 130, 220), RGB(100, 160, 250), RGB(50, 100, 200),
            RGB(255, 255, 255), RGB(70, 130, 220));
        SetStyle(ButtonStyle::Modern);
    }

    void LsButton::SetDarkTheme()
    {
        DisableGlow();  // 关闭发光
        SetTheme(RGB(60, 60, 65), RGB(80, 80, 85), RGB(45, 45, 50),
            RGB(255, 255, 255), RGB(100, 100, 110));
        SetStyle(ButtonStyle::Modern);
    }

    void LsButton::SetSuccessTheme()
    {
        DisableGlow();  // 关闭发光
        SetTheme(RGB(50, 180, 80), RGB(70, 210, 100), RGB(40, 150, 60),
            RGB(255, 255, 255), RGB(50, 180, 80));
        SetStyle(ButtonStyle::Modern);
    }

    void LsButton::SetDangerTheme()
    {
        DisableGlow();  // 关闭发光
        SetTheme(RGB(220, 60, 50), RGB(240, 80, 70), RGB(190, 45, 35),
            RGB(255, 255, 255), RGB(220, 60, 50));
        SetStyle(ButtonStyle::Modern);
    }

    void LsButton::SetWarningTheme()
    {
        DisableGlow();  // 关闭发光
        SetTheme(RGB(240, 150, 30), RGB(255, 170, 50), RGB(210, 120, 20),
            RGB(255, 255, 255), RGB(240, 150, 30));
        SetStyle(ButtonStyle::Modern);
    }

    void LsButton::SetInfoTheme()
    {
        DisableGlow();  // 关闭发光
        SetTheme(RGB(30, 180, 200), RGB(50, 210, 230), RGB(20, 150, 170),
            RGB(255, 255, 255), RGB(30, 180, 200));
        SetStyle(ButtonStyle::Modern);
    }

    void LsButton::SetNeonTheme()
    {
        // 霓虹主题：不关闭发光，反而要启用
        SetTheme(RGB(10, 10, 20), RGB(20, 20, 40), RGB(5, 5, 15),
            RGB(0, 255, 150), RGB(0, 255, 150));
        EnableGlow(RGB(0, 255, 150), 5);
        SetStyle(ButtonStyle::Neon);
    }

    void LsButton::OnClick(std::function<void()> callback)
    {
        m_onClick = callback;
    }

    void LsButton::OnToggle(std::function<void(bool)> callback)
    {
        m_onToggle = callback;
    }

    RECT LsButton::GetPixelRect() const
    {
        return { GetScaledX(), GetScaledY(),
                 GetScaledX() + GetScaledWidth(),
                 GetScaledY() + GetScaledHeight() };
    }

    bool LsButton::IsPointInside(int x, int y) const
    {
        return (x >= GetScaledX() && x <= GetScaledX() + GetScaledWidth() &&
            y >= GetScaledY() && y <= GetScaledY() + GetScaledHeight());
    }
}