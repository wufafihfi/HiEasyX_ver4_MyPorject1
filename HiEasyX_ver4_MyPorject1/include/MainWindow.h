#pragma once
#define _WIN32_WINNT 0x0600 

#include "HiEasyX.h"
#include "TimeDeal.h"
#include "Ls/LsUI/LsDPIManager.h"
#include "Ls/LsUI/LsSlider.h"
#include "Ls/LsUI/LsButton.h"
#include "JsonManager.h"

#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <json/json.h>
#include <windows.h>
#include <winreg.h>

#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

#define IDC_A	101
#define IDC_B	102
#define IDC_C	103

#define DEFAL_WINDOW_WIDTH 400
#define DEFAL_WINDOW_HEIGHT 300

extern hiex::Window mainWindow_hwnd;
extern HMENU hMenu;
extern bool close_flag;
extern float windowAlphaMW;
extern hiex::FPScontrol *_MainFPS;

// 暂时不用
/*
inline void OptimizeProgramStartup(const std::wstring& appName, bool enableAutoStart)
{
    // 1. 设置高优先级
    SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

    // 2. 开机自启（根据参数决定）
    if (enableAutoStart)
    {
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(NULL, exePath, MAX_PATH);

        HKEY hKey;
        if (RegOpenKeyExW(HKEY_CURRENT_USER,
            L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
            0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
        {
            std::wstring value = L"\"" + std::wstring(exePath) + L"\"";
            RegSetValueExW(hKey, appName.c_str(), 0, REG_SZ,
                (const BYTE*)value.c_str(),
                (DWORD)((value.size() + 1) * sizeof(wchar_t)));
            RegCloseKey(hKey);
        }
        else {
            // 删除自启动项
            RegDeleteKeyValueW(HKEY_CURRENT_USER,
                L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                appName.c_str());
        }
    }
}
*/


void OnTray(UINT id);

void init_MainWindow(hiex::FPScontrol* _MainFPS_);

void MainWindow();

void MainWindowDraw();

bool GetCloseFlag();