#ifndef LsUI_Slider 

#pragma once
#include <graphics.h>
#include <functional>
#include "LsDPIManager.h"

namespace Ls_UI
{
    class LsPlaneSelector{
    private:
        // 几何属性（逻辑坐标）
        int m_x, m_y;           // 控件左上角坐标
        int m_width, m_height;  // 控件区域宽高
        int m_thumbRadius;      // 拖拽圆点半径
        int m_expand_w, m_expand_h;     // 背景矩形拓宽
        int m_ellipseWH;        // 圆角矩形角

        // 数值属性
        int m_minX, m_maxX;
        int m_minY, m_maxY;
        int m_currentX, m_currentY;

        // 其他属性
        bool drawGrid_flag;         // 是否使用网格
        bool roundrect_flag;        // 是否使用圆角矩形

        // 自动对齐
        bool m_IsSnap;              // 是否使用自动对齐
        int m_TriggerDist;          // 自动对齐距离
        int m_snapReleaseDist;    // 释放距离
        int m_snapTarget_Px, m_snapTarget_Py;   // 对齐目标(自动对齐点所在的横纵线)

        // 文本
        // 是否显示TOOLTIP(自绘)
        bool ShowValueToolTip;
        // 是否启用滑块旁数值显示
        bool ShowValueBeside;
        int ShowValue_count;        // 显示的数据个数 0全显示 1显示X 2显示Y
        int ValueTextDirection;     // 数值显示位置 4个方向
        int TextPositionOffset;     // 文本显示位置偏移
        bool TextBorder_flag;       // 文本框是否显示
        int m_TextBorderEllipseWH;  // 文本框圆角矩形角

        // 交互状态
        bool m_isDragging;
        bool m_isHover;
        bool m_isSnapped_X,m_isSnapped_Y;
        int m_thumbPixelX, m_thumbPixelY;  // 圆点当前像素位置

        // 颜色主题
        COLORREF m_bgColor;           // 背景色
        COLORREF m_gridColor;         // 网格线颜色
        COLORREF m_thumbColor;        // 圆点颜色
        COLORREF m_thumbCenterCircleColor;      // 圆点中心圆颜色
        COLORREF m_thumbHoverColor;   // 圆点悬停颜色
        COLORREF m_borderColor;       // 边框颜色
        COLORREF m_crossColor;        // 十字线颜色
        //文本
        COLORREF m_text_bgColor;           // 背景色
        COLORREF m_text_borderColor;       // 边框颜色
        COLORREF m_text_color;             // 文本颜色
        const wchar_t* m_text_font;            // 字体
        const wchar_t* m_text_sliderName;      // 滑块名称

        // 回调函数
        std::function<void(int, int)> m_onValueChanged;

        // 启用标志
        bool m_enabled;

        // 辅助方法
        void UpdateThumbPixelPosition();
        void ValueToPixel(int x, int y, int& outX, int& outY) const;
        void PixelToValue(int pixelX, int pixelY, int& outX, int& outY) const;
        bool IsPointOnThumb(int mouseX, int mouseY) const;
        bool IsPointInArea(int mouseX, int mouseY) const;

        // 缩放辅助
        int GetScaledX() const { return DPIManager::ScaleX(m_x); }
        int GetScaledY() const { return DPIManager::ScaleY(m_y); }
        int GetScaledWidth() const { return DPIManager::ScaleX(m_width); }          // 获取选择点实际移动范围 W
        int GetScaledHeight() const { return DPIManager::ScaleY(m_height); }        // 获取选择点实际移动范围 H
        int GetExpandWidth() const { return DPIManager::Scale(m_expand_w); }
        int GetExpandHeight() const { return DPIManager::Scale(m_expand_h); }
        int GetScaledRadius() const { return DPIManager::Scale(m_thumbRadius); }
        int GetExpandEllipseWH() const { return DPIManager::Scale(m_ellipseWH); }
        int GetExpandTextBorderEllipseWH() const { return DPIManager::Scale(m_TextBorderEllipseWH); }

        // 功能方法
        void autoSnap(int mouseX, int mouseY, int& outX, int& outY);

    public:
        LsPlaneSelector();
        ~LsPlaneSelector() = default;

        // 初始化
        void Create(int x, int y, int width, int height,
            int minX = 0, int maxX = 100,
            int minY = 0, int maxY = 100,
            int defaultX = 50, int defaultY = 50,
            int thumbRadius = 8,
            int expand_w = 5,
            int expand_h = 5,
            bool drawGrid = true,
            bool roundrect = true,
            int ellipseWH = 10
        );

        // 文本样式设置
        void SetValueText(bool showValueToolTip = true,
            bool showValueBeside = true,
            int showValue_count = 0,
            int valueTextDirection = 0,
            int textPositionOffset = 10,
            int TextBorderEllipseWH = 5,
            bool textBorder_flag = true,
            const wchar_t* text_font = L"微软雅黑",
            const wchar_t* text_sliderName = L"滑块"
        );

        // 自动对齐功能
        void SetSnap(bool IsSnap = false,
            int TriggerDist = 5,
            int snapReleaseDist = 10,
            int snapTarget_Px = 0, 
            int snapTarget_Py = 0
            );

        // 缩放辅助函数 公共扩展
        int GetWidth() { return m_width; }
        int GetHeight() { return m_height; }

        // 绘制
        void Draw();

        // 交互处理
        bool OnMouseMove(int mouseX, int mouseY);
        bool OnMouseDown(int mouseX, int mouseY);
        void OnMouseUp();

        // 获取/设置数值
        int GetX() const { return m_currentX; }
        int GetY() const { return m_currentY; }
        void GetValue(int& x, int& y) const { x = m_currentX; y = m_currentY; }
        void SetValue(int x, int y, bool triggerCallback = true);

        // 设置范围
        void SetRangeX(int minVal, int maxVal);
        void SetRangeY(int minVal, int maxVal);
        void SetRange(int minX, int maxX, int minY, int maxY);

        // 颜色主题设置
        void SetTheme(COLORREF bg, COLORREF grid, COLORREF thumb, COLORREF thumbCenterCircleColor,
            COLORREF thumbHover, COLORREF border, COLORREF cross,
            COLORREF text_bgColor, COLORREF text_borderColor, COLORREF text_color);
        // AI太好用了!
        void SetDefaultTheme();      // 默认主题（蓝灰经典）
        void SetDarkTheme();         // 暗黑主题
        void SetHeatmapTheme();      // 热力图主题
        void SetDeepSpaceTheme();    // 深空主题（深蓝科技感）
        void SetSakuraTheme();       // 樱花主题（粉嫩可爱）
        void SetForestTheme();       // 森林主题（清新自然）
        void SetSunsetTheme();       // 日落主题（温暖橙黄）
        void SetOceanTheme();        // 海洋主题（清爽海洋）
        void SetVioletTheme();       // 紫罗兰主题（优雅紫色）
        void SetGraphiteTheme();     // 石墨主题（工业简约）
        void SetNeonTheme();         // 荧光主题（炫酷荧光）
        void SetCandyTheme();        // 糖果主题（甜美糖果）

        // 设置回调
        void OnValueChanged(std::function<void(int, int)> callback) { m_onValueChanged = callback; }

        // 启用/禁用
        void SetEnabled(bool enabled) { m_enabled = enabled; }
        bool IsEnabled() const { return m_enabled; }

        // 获取边界矩形
        RECT GetPixelRect() const;
    };
}

#endif