#pragma once
#include <windows.h>

class DPIManager {
private:
    static float m_scaleX;
    static float m_scaleY;
    static int m_dpi;

public:
    static void InitializeFromDC();
    static float GetScaleX() { return m_scaleX; }
    static float GetScaleY() { return m_scaleY; }
    static int GetDPI() { return m_dpi; }
    static int ScaleX(int x) { return (int)(x * m_scaleX); }
    static int ScaleY(int y) { return (int)(y * m_scaleY); }
    static int Scale(int value) { return (int)(value * m_scaleX); }
};