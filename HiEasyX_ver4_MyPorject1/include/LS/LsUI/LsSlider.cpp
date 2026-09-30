#include "LsSlider.h"
#include <algorithm>
#include <cmath>

namespace Ls_UI
{
    LsPlaneSelector::LsPlaneSelector()
        : m_x(0), m_y(0), m_width(300), m_height(300), m_thumbRadius(8)
        , m_minX(0), m_maxX(100), m_minY(0), m_maxY(100)
        , m_currentX(50), m_currentY(50)
        , m_isDragging(false), m_isHover(false), m_isSnapped_X(false), m_isSnapped_Y(false)
        , m_thumbPixelX(0), m_thumbPixelY(0)
        , m_bgColor(RGB(240, 240, 240))
        , m_gridColor(RGB(200, 200, 200))
        , m_thumbColor(RGB(70, 130, 220))
        , m_thumbHoverColor(RGB(100, 160, 250))
        , m_borderColor(RGB(128, 128, 128))
        , m_crossColor(RGB(150, 150, 150))
        , m_enabled(true)
        , m_onValueChanged(nullptr)
    {
    }

    void LsPlaneSelector::Create(int x, int y, int width, int height,
        int minX, int maxX, int minY, int maxY,
        int defaultX, int defaultY, int thumbRadius,
        int expand_w, int expand_h,
        bool drawGrid,
        bool roundrect,
        int ellipseWH
    )
    {
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;
        m_thumbRadius = thumbRadius;

        m_minX = minX;
        m_maxX = maxX;
        m_minY = minY;
        m_maxY = maxY;

        m_currentX = std::clamp(defaultX, minX, maxX);
        m_currentY = std::clamp(defaultY, minY, maxY);

        m_expand_w = expand_w;
        m_expand_h = expand_h;

        drawGrid_flag = drawGrid;
        roundrect_flag = roundrect;

        m_ellipseWH = ellipseWH;

        SetValueText();

        SetSnap();

        UpdateThumbPixelPosition();
    }

    // 文本设置
    void LsPlaneSelector::SetValueText(bool showValueToolTip,
        bool showValueBeside,
        int showValue_count,
        int valueTextDirection,
        int textPositionOffset,
        int TextBorderEllipseWH,
        bool textBorder_flag,
        const wchar_t* text_font,
        const wchar_t* text_sliderName
    )
    {
        ShowValueToolTip = showValueToolTip;
        ShowValueBeside = showValueBeside;

        ShowValue_count = showValue_count;
        ValueTextDirection = valueTextDirection;
        TextPositionOffset = textPositionOffset;
        m_TextBorderEllipseWH = TextBorderEllipseWH;

        TextBorder_flag = textBorder_flag;

        m_text_font = text_font;
        m_text_sliderName = text_sliderName;

    }

    void LsPlaneSelector::SetSnap(bool IsSnap,
        int TriggerDist,
        int snapReleaseDist,
        int snapTarget_Px,
        int snapTarget_Py
    )
    {
        m_IsSnap = IsSnap;

        m_TriggerDist = DPIManager::Scale(TriggerDist);
        m_snapReleaseDist = DPIManager::Scale(snapReleaseDist);

        m_snapTarget_Px = DPIManager::Scale(snapTarget_Px);
        m_snapTarget_Py = DPIManager::Scale(snapTarget_Py);
    }
    // 自动对齐实现
    void LsPlaneSelector::autoSnap(int mouseX, int mouseY, int& outX, int& outY) {
        float distX = std::abs(mouseX - (GetScaledX() + m_snapTarget_Px));
        float distY = std::abs(mouseY - (GetScaledY() + m_snapTarget_Py));

        if (!m_isSnapped_X)
        {
            if (distX <= m_TriggerDist) {
                m_isSnapped_X = true;
            }
        }
        else
        {
            if (distX >= m_snapReleaseDist) {
                m_isSnapped_X = false;
            }
        }
        if (!m_isSnapped_Y)
        {
            if (distY <= m_TriggerDist) {
                m_isSnapped_Y = true;
            }
        }
        else
        {
            if (distY >= m_snapReleaseDist) {
                m_isSnapped_Y = false;
            }
        }

        if (m_isSnapped_X) {
            outX = GetScaledX() + m_snapTarget_Px;
        }
        else {
            outX = mouseX;
        }
        if (m_isSnapped_Y) {
            outY = GetScaledY() + m_snapTarget_Py;
        }
        else {
            outY = mouseY;
        }
    }

    void LsPlaneSelector::UpdateThumbPixelPosition()
    {
        ValueToPixel(m_currentX, m_currentY, m_thumbPixelX, m_thumbPixelY);
    }

    void LsPlaneSelector::ValueToPixel(int x, int y, int& outX, int& outY) const
    {
        float ratioX = (x - m_minX) / (float)(m_maxX - m_minX);
        float ratioY = (y - m_minY) / (float)(m_maxY - m_minY);

        outX = GetScaledX() + (int)(ratioX * GetScaledWidth());
        outY = GetScaledY() + (int)(ratioY * GetScaledHeight());
    }

    void LsPlaneSelector::PixelToValue(int pixelX, int pixelY, int& outX, int& outY) const
    {
        pixelX = std::clamp(pixelX, GetScaledX(), GetScaledX() + GetScaledWidth());
        pixelY = std::clamp(pixelY, GetScaledY(), GetScaledY() + GetScaledHeight());

        float ratioX = (pixelX - GetScaledX()) / (float)GetScaledWidth();
        float ratioY = (pixelY - GetScaledY()) / (float)GetScaledHeight();

        outX = m_minX + (int)(ratioX * (m_maxX - m_minX));
        outY = m_minY + (int)(ratioY * (m_maxY - m_minY));
    }

    bool LsPlaneSelector::IsPointOnThumb(int mouseX, int mouseY) const
    {
        int dx = mouseX - m_thumbPixelX;
        int dy = mouseY - m_thumbPixelY;
        int radius = GetScaledRadius();
        return ((dx * dx + dy * dy) <= (radius * radius));
    }

    bool LsPlaneSelector::IsPointInArea(int mouseX, int mouseY) const
    {
        return (mouseX >= GetScaledX() - GetExpandWidth()
            && mouseX <= GetScaledX() + GetScaledWidth() + GetExpandWidth()
            && mouseY >= GetScaledY() - GetExpandHeight()
            && mouseY <= GetScaledY() + GetScaledHeight() + GetExpandHeight());
    }

    void LsPlaneSelector::Draw()
    {
        if (!m_enabled) return;

        int x = GetScaledX();
        int y = GetScaledY();
        int w = GetScaledWidth();
        int h = GetScaledHeight();
        int radius = GetScaledRadius();

        int expand_w = GetExpandWidth();
        int expand_h = GetExpandHeight();

        int ellipseWH = GetExpandEllipseWH();
        int textEllipseWH = GetExpandTextBorderEllipseWH();

        // 1. 绘制背景
        setfillcolor(m_bgColor);
        setlinecolor(m_borderColor);
        setlinestyle(PS_SOLID, 1);
        if (roundrect_flag) {
            fillroundrect(x - expand_w, y - expand_h, x + w + expand_w, y + h + expand_h, ellipseWH, ellipseWH);
            roundrect(x, y, x + w, y + h, ellipseWH, ellipseWH);
        }
        else
        {
            solidrectangle(x - expand_w, y - expand_h, x + w + expand_w, y + h + expand_h);
            rectangle(x, y, x + w, y + h);
        }

        if (drawGrid_flag)
        {
            // 2. 绘制网格线（根据区域大小动态调整网格密度）
            int gridStepsX = max(4, w / 50);
            int gridStepsY = max(4, h / 50);
            setlinecolor(m_gridColor);
            setlinestyle(PS_SOLID, 1);

            for (int i = 1; i <= gridStepsX; i++) {
                int gridX = x + w * i / gridStepsX;
                line(gridX, y, gridX, y + h);
            }
            for (int i = 1; i <= gridStepsY; i++) {
                int gridY = y + h * i / gridStepsY;
                line(x, gridY, x + w, gridY);
            }

            // 3. 绘制十字线（跟随圆点位置）
            setlinecolor(m_crossColor);
            setlinestyle(PS_SOLID, 1);
            line(x, m_thumbPixelY, x + w, m_thumbPixelY);
            line(m_thumbPixelX, y, m_thumbPixelX, y + h);
        }

        // 绘制自动对齐线
        if (m_IsSnap) {
            setlinecolor(m_thumbColor);
            setlinestyle(PS_SOLID, 2);
            if(m_isSnapped_X)
            {
                line(x + m_snapTarget_Px, y, x + m_snapTarget_Px, y + h);
            }
            if (m_isSnapped_Y)
            {
                line(x, y + m_snapTarget_Py, x + w, y + m_snapTarget_Py);
            }
        }

        // 旁侧文本显示
        if (ShowValueBeside) {
            wchar_t buf[64];
            if (ShowValue_count == 0)
            {
                swprintf(buf, 64, L"%s %d,%d", m_text_sliderName, m_currentX, m_currentY);
            }
            else if (ShowValue_count == 1) {
                swprintf(buf, 64, L"%s %d", m_text_sliderName, m_currentX);
            }
            else {
                swprintf(buf, 64, L"%s %d", m_text_sliderName, m_currentY);
            }

            int fontSize = DPIManager::Scale(20);
            settextstyle(fontSize, 0, m_text_font);
            settextcolor(m_text_color);
            setbkmode(TRANSPARENT);

            int textWidth = textwidth(buf);
            int textHeight = textheight(buf);

            int scaledOffset = DPIManager::Scale(TextPositionOffset);

            // 初始位置为左边
            int textX = x - expand_w - textWidth - scaledOffset;
            int textY = y - textHeight / 2.0f;
            if (ValueTextDirection == 1) {  // 上边
                textX = x + w / 2.0f - textWidth / 2.0f;
                textY = y - textHeight - scaledOffset;
            }
            else if (ValueTextDirection == 2) {  // 右边
                textX = x + w + expand_w + scaledOffset;
                textY = y - textHeight / 2.0f;
            }
            else if (ValueTextDirection == 3) {  // 下边
                textX = x + w / 2.0f - textWidth / 2.0f;
                textY = y + h + scaledOffset;
            }

            // 绘制文字背景
            setfillcolor(m_text_bgColor);
            setlinecolor(m_text_borderColor);
            setlinestyle(PS_SOLID, 1);
            if (TextBorder_flag)
            {
                if (roundrect_flag) {
                    fillroundrect(textX - DPIManager::Scale(3), textY - DPIManager::Scale(2),
                        textX + textWidth + DPIManager::Scale(3), textY + textHeight + DPIManager::Scale(2),
                        textEllipseWH, textEllipseWH
                    );
                }
                else
                {
                    fillrectangle(textX - DPIManager::Scale(3), textY - DPIManager::Scale(2),
                        textX + textWidth + DPIManager::Scale(3), textY + textHeight + DPIManager::Scale(2));
                }
            }

            outtextxy(textX, textY, buf);
        }

        // 绘制圆点
        COLORREF thumbColor = m_isHover ? m_thumbHoverColor : m_thumbColor;
        setfillcolor(thumbColor);
        setlinestyle(PS_SOLID, 1);
        solidcircle(m_thumbPixelX, m_thumbPixelY, radius);

        // 圆点中的小圆
        int lightCircleRadius = m_isDragging ? (radius / 9.0f * 7.0f) : (radius / 3.0f * 2.0f);
        setfillcolor(m_thumbCenterCircleColor);
        solidcircle(m_thumbPixelX, m_thumbPixelY, lightCircleRadius);

        // 悬停时显示坐标值
        if ((m_isHover || m_isDragging) && ShowValueToolTip) {
            wchar_t buf[64];
            if (ShowValue_count == 0)
            {
                swprintf(buf, 64, L"%d,%d", m_currentX, m_currentY);
            }
            else if (ShowValue_count == 1) {
                swprintf(buf, 64, L"%d", m_currentX);
            }
            else {
                swprintf(buf, 64, L"%d", m_currentY);
            }

            int fontSize = DPIManager::Scale(16);
            settextstyle(fontSize, 0, m_text_font);
            settextcolor(m_text_color);
            setbkmode(TRANSPARENT);

            int textWidth = textwidth(buf);
            int textHeight = textheight(buf);

            // 文字显示在圆点上方
            int textX = m_thumbPixelX - textWidth / 2;
            int textY = y - textHeight - radius - DPIManager::Scale(10);

            // 如果上方空间不足，显示在下方
            if (textY < 0) {
                textY = y + h + radius + DPIManager::Scale(8);
            }

            // 绘制文字背景
            setfillcolor(m_text_bgColor);
            setlinecolor(m_text_borderColor);
            setlinestyle(PS_SOLID, 1);
            if (roundrect_flag) {
                fillroundrect(textX - DPIManager::Scale(3), textY - DPIManager::Scale(2),
                    textX + textWidth + DPIManager::Scale(3), textY + textHeight + DPIManager::Scale(2),
                    textEllipseWH, textEllipseWH
                );
            }
            else
            {
                fillrectangle(textX - DPIManager::Scale(3), textY - DPIManager::Scale(2),
                    textX + textWidth + DPIManager::Scale(3), textY + textHeight + DPIManager::Scale(2));
            }

            outtextxy(textX, textY, buf);
        }
    }

    bool LsPlaneSelector::OnMouseMove(int mouseX, int mouseY)
    {
        if (!m_enabled) return false;

        //bool oldHover = m_isHover;
        m_isHover = IsPointInArea(mouseX, mouseY) || IsPointOnThumb(mouseX, mouseY);

        if (m_isDragging) {
            int newX, newY;
            if (m_IsSnap) {
                autoSnap(mouseX, mouseY, newX, newY);
                PixelToValue(newX, newY, newX, newY);
            }
            else
            {
                PixelToValue(mouseX, mouseY, newX, newY);
            }

            if (newX != m_currentX || newY != m_currentY) {
                SetValue(newX, newY, true);
                UpdateThumbPixelPosition();

                if (m_onValueChanged) {
                    m_onValueChanged(m_currentX, m_currentY);
                }
            }
            return true;
        }

        return m_isHover;
    }

    bool LsPlaneSelector::OnMouseDown(int mouseX, int mouseY)
    {
        if (!m_enabled) return false;

        if (IsPointOnThumb(mouseX, mouseY)) {
            m_isDragging = true;
            return true;
        }
        else if (IsPointInArea(mouseX, mouseY)) {
            // 点击区域任意位置直接跳转
            int newX, newY;
            PixelToValue(mouseX, mouseY, newX, newY);

            if (newX != m_currentX || newY != m_currentY) {
                SetValue(newX, newY, true);
                UpdateThumbPixelPosition();

                if (m_onValueChanged) {
                    m_onValueChanged(m_currentX, m_currentY);
                }
            }

            m_isDragging = true;
            return true;
        }

        return false;
    }

    void LsPlaneSelector::OnMouseUp()
    {
        m_isDragging = false;
    }

    void LsPlaneSelector::SetValue(int x, int y, bool triggerCallback)
    {
        x = std::clamp(x, m_minX, m_maxX);
        y = std::clamp(y, m_minY, m_maxY);

        if (x != m_currentX || y != m_currentY) {
            m_currentX = x;
            m_currentY = y;
            UpdateThumbPixelPosition();

            if (triggerCallback && m_onValueChanged) {
                m_onValueChanged(m_currentX, m_currentY);
            }
        }
    }

    void LsPlaneSelector::SetRangeX(int minVal, int maxVal)
    {
        m_minX = minVal;
        m_maxX = maxVal;
        SetValue(m_currentX, m_currentY, false);
    }

    void LsPlaneSelector::SetRangeY(int minVal, int maxVal)
    {
        m_minY = minVal;
        m_maxY = maxVal;
        SetValue(m_currentX, m_currentY, false);
    }

    void LsPlaneSelector::SetRange(int minX, int maxX, int minY, int maxY)
    {
        m_minX = minX;
        m_maxX = maxX;
        m_minY = minY;
        m_maxY = maxY;
        SetValue(m_currentX, m_currentY, false);
    }

    void LsPlaneSelector::SetTheme(COLORREF bg, COLORREF grid, COLORREF thumb, COLORREF thumbCenterCircleColor,
        COLORREF thumbHover, COLORREF border, COLORREF cross,
        COLORREF text_bgColor, COLORREF text_borderColor, COLORREF text_color)
    {
        m_bgColor = bg;
        m_gridColor = grid;
        m_thumbColor = thumb;
        m_thumbCenterCircleColor = thumbCenterCircleColor;
        m_thumbHoverColor = thumbHover;
        m_borderColor = border;
        m_crossColor = cross;

        m_text_bgColor = text_bgColor;
        m_text_borderColor = text_borderColor;
        m_text_color = text_color;
    }

    // 1. 默认主题（你已有的）
    void LsPlaneSelector::SetDefaultTheme()
    {
        SetTheme(
            RGB(240, 240, 240),  // 背景浅灰
            RGB(200, 200, 200),  // 网格浅灰
            RGB(70, 130, 220),   // 圆点蓝色
            RGB(240, 240, 240),  // 圆点中心圆灰色
            RGB(100, 160, 250),  // 悬停浅蓝
            RGB(128, 128, 128),  // 边框灰色
            RGB(180, 180, 180),  // 十字线浅灰
            RGB(240, 240, 240),  // 文本框背景浅灰
            RGB(128, 128, 128),  // 文本框灰色
            RGB(70, 130, 220)    // 文本蓝色
        );
    }

    // 2. 暗黑主题（你已有的）
    void LsPlaneSelector::SetDarkTheme()
    {
        SetTheme(
            RGB(50, 50, 50),     // 背景深灰
            RGB(80, 80, 80),     // 网格深灰
            RGB(0, 120, 215),    // 圆点蓝色
            RGB(50, 50, 50),     // 圆点中心圆灰色
            RGB(0, 160, 255),    // 悬停亮蓝
            RGB(100, 100, 100),  // 边框
            RGB(100, 100, 100),  // 十字线
            RGB(50, 50, 50),     // 文本框背景深灰
            RGB(100, 100, 100),  // 文本框
            RGB(0, 120, 215)     // 文本蓝色
        );
    }

    // 3. 热力图主题（你已有的）
    void LsPlaneSelector::SetHeatmapTheme()
    {
        SetTheme(
            RGB(255, 245, 235),  // 背景暖白
            RGB(255, 200, 150),  // 网格橙色
            RGB(255, 80, 40),    // 圆点红色
            RGB(255, 255, 255),  // 圆点中心圆白色
            RGB(255, 120, 80),   // 悬停橙红
            RGB(200, 100, 50),   // 边框
            RGB(255, 180, 130),  // 十字线
            RGB(255, 245, 235),  // 文本框背景暖白
            RGB(200, 100, 50),   // 文本框
            RGB(255, 80, 40)     // 文本红色
        );
    }

    // 4. 深空主题（Deep Space）
    void LsPlaneSelector::SetDeepSpaceTheme()
    {
        SetTheme(
            RGB(20, 25, 45),     // 背景深蓝黑
            RGB(60, 70, 100),    // 网格暗蓝
            RGB(100, 180, 250),  // 圆点亮蓝
            RGB(40, 50, 70),     // 圆点中心暗蓝
            RGB(130, 200, 255),  // 悬停亮蓝
            RGB(80, 90, 120),    // 边框
            RGB(70, 80, 110),    // 十字线
            RGB(20, 25, 45),     // 文本框背景
            RGB(80, 90, 120),    // 文本框
            RGB(100, 180, 250)   // 文本亮蓝
        );
    }

    // 5. 樱花主题（Sakura）
    void LsPlaneSelector::SetSakuraTheme()
    {
        SetTheme(
            RGB(255, 240, 245),  // 背景樱花粉
            RGB(255, 200, 210),  // 网格粉红
            RGB(255, 100, 150),  // 圆点粉色
            RGB(255, 220, 230),  // 圆点中心浅粉
            RGB(255, 130, 170),  // 悬停粉红
            RGB(230, 150, 170),  // 边框
            RGB(255, 200, 220),  // 十字线
            RGB(255, 240, 245),  // 文本框背景
            RGB(230, 150, 170),  // 文本框
            RGB(255, 100, 150)   // 文本粉色
        );
    }

    // 6. 森林主题（Forest）
    void LsPlaneSelector::SetForestTheme()
    {
        SetTheme(
            RGB(230, 245, 230),  // 背景浅绿
            RGB(180, 210, 170),  // 网格绿色
            RGB(60, 140, 60),    // 圆点深绿
            RGB(210, 235, 210),  // 圆点中心浅绿
            RGB(80, 170, 80),    // 悬停绿
            RGB(120, 150, 110),  // 边框
            RGB(170, 200, 160),  // 十字线
            RGB(230, 245, 230),  // 文本框背景
            RGB(120, 150, 110),  // 文本框
            RGB(60, 140, 60)     // 文本绿色
        );
    }

    // 7. 日落主题（Sunset）
    void LsPlaneSelector::SetSunsetTheme()
    {
        SetTheme(
            RGB(255, 235, 210),  // 背景橙黄
            RGB(255, 180, 120),  // 网格橙色
            RGB(255, 100, 50),   // 圆点橙色
            RGB(255, 215, 180),  // 圆点中心浅橙
            RGB(255, 130, 80),   // 悬停橙红
            RGB(200, 130, 80),   // 边框
            RGB(255, 200, 150),  // 十字线
            RGB(255, 235, 210),  // 文本框背景
            RGB(200, 130, 80),   // 文本框
            RGB(255, 100, 50)    // 文本橙色
        );
    }

    // 8. 海洋主题（Ocean）
    void LsPlaneSelector::SetOceanTheme()
    {
        SetTheme(
            RGB(220, 240, 255),  // 背景浅蓝
            RGB(170, 200, 230),  // 网格蓝色
            RGB(30, 100, 180),   // 圆点深蓝
            RGB(200, 225, 245),  // 圆点中心浅蓝
            RGB(50, 130, 200),   // 悬停蓝
            RGB(100, 140, 180),  // 边框
            RGB(180, 210, 240),  // 十字线
            RGB(220, 240, 255),  // 文本框背景
            RGB(100, 140, 180),  // 文本框
            RGB(30, 100, 180)    // 文本深蓝
        );
    }

    // 9. 紫罗兰主题（Violet）
    void LsPlaneSelector::SetVioletTheme()
    {
        SetTheme(
            RGB(245, 235, 255),  // 背景浅紫
            RGB(200, 170, 220),  // 网格紫色
            RGB(130, 70, 180),   // 圆点紫色
            RGB(225, 210, 240),  // 圆点中心浅紫
            RGB(150, 90, 200),   // 悬停紫
            RGB(160, 120, 190),  // 边框
            RGB(210, 190, 230),  // 十字线
            RGB(245, 235, 255),  // 文本框背景
            RGB(160, 120, 190),  // 文本框
            RGB(130, 70, 180)    // 文本紫色
        );
    }

    // 10. 石墨主题（Graphite）- 工业风
    void LsPlaneSelector::SetGraphiteTheme()
    {
        SetTheme(
            RGB(45, 45, 48),     // 背景深灰
            RGB(80, 80, 85),     // 网格灰
            RGB(0, 150, 200),    // 圆点青色
            RGB(60, 60, 65),     // 圆点中心深灰
            RGB(0, 180, 230),    // 悬停青色
            RGB(100, 100, 110),  // 边框
            RGB(90, 90, 95),     // 十字线
            RGB(45, 45, 48),     // 文本框背景
            RGB(100, 100, 110),  // 文本框
            RGB(0, 150, 200)     // 文本青色
        );
    }

    // 11. 荧光主题（Neon）
    void LsPlaneSelector::SetNeonTheme()
    {
        SetTheme(
            RGB(10, 10, 20),     // 背景深黑
            RGB(0, 50, 80),      // 网格深青
            RGB(0, 255, 150),    // 圆点霓虹绿
            RGB(20, 30, 40),     // 圆点中心暗色
            RGB(50, 255, 180),   // 悬停亮绿
            RGB(0, 100, 120),    // 边框
            RGB(0, 80, 100),     // 十字线
            RGB(10, 10, 20),     // 文本框背景
            RGB(0, 100, 120),    // 文本框
            RGB(0, 255, 150)     // 文本霓虹绿
        );
    }

    // 12. 糖果主题（Candy）
    void LsPlaneSelector::SetCandyTheme()
    {
        SetTheme(
            RGB(255, 245, 250),  // 背景粉白
            RGB(255, 180, 200),  // 网格粉红
            RGB(255, 80, 120),   // 圆点红
            RGB(255, 220, 230),  // 圆点中心浅粉
            RGB(255, 110, 150),  // 悬停粉红
            RGB(220, 130, 150),  // 边框
            RGB(255, 200, 220),  // 十字线
            RGB(255, 245, 250),  // 文本框背景
            RGB(220, 130, 150),  // 文本框
            RGB(255, 80, 120)    // 文本红色
        );
    }

    RECT LsPlaneSelector::GetPixelRect() const
    {
        int radius = GetScaledRadius();
        return {
            GetScaledX() - radius,
            GetScaledY() - radius,
            GetScaledX() + GetScaledWidth() + radius,
            GetScaledY() + GetScaledHeight() + radius
        };
    }
}