#include "LsDPIManager.h"

// 静态成员变量
float DPIManager::m_scaleX = 1.0f;
float DPIManager::m_scaleY = 1.0f;
int DPIManager::m_dpi = 96;

void DPIManager::InitializeFromDC() {
    HDC hdc = GetDC(NULL);
    int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
    int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(NULL, hdc);

    m_scaleX = dpiX / 96.0f;
    m_scaleY = dpiY / 96.0f;
    m_dpi = dpiX;
}