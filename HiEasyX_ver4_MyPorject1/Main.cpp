#include <windows.h>

#include "HiEasyX.h"
#include "resource.h"

#include "include/includeALL.hpp"

//https://github.com/zouhuidong/HiEasyX/tree/main

bool IsWindowsVistaOrLater()
{
    // 方法1：使用 VerifyVersionInfo（推荐）
    OSVERSIONINFOEXW osvi = { sizeof(osvi) };
    osvi.dwMajorVersion = 6;
    osvi.dwMinorVersion = 0;

    DWORDLONG mask = 0;
    VER_SET_CONDITION(mask, VER_MAJORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(mask, VER_MINORVERSION, VER_GREATER_EQUAL);

    return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, mask) != FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    if (!IsWindowsVistaOrLater())
    {
        MessageBoxW(NULL,
            L"不兼容的操作系统\n\n"
            L"此程序需要 Windows Vista 或更高版本。\n"
            L"Windows XP 及更早版本不受支持。\n\n"
            L"程序将退出。",
            L"系统要求不满足",
            MB_OK | MB_ICONSTOP | MB_APPLMODAL);
        exit(1);
    }

    hiex::SetCustomIcon(MAKEINTRESOURCE(IDI_ICON1), MAKEINTRESOURCE(IDI_ICON1));

    // 互斥体创建
    HANDLE hMutex = CreateMutex(NULL, TRUE,
        L"Lcy#z15-bzhf-screenTools-912ef77d9b981a76");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBoxW(NULL, _T("错误码:114514100867891\nBZF_EXEERRO_REOPEN\n程序正在运行,无法重复打开此程序\n请在系统托盘中打开窗口"), _T("不对啊"), MB_OK | MB_ICONERROR);
        if (hMutex)
        {
            CloseHandle(hMutex);
        }
        return 0;
    }

    SetProcessDPIAware();
    DPIManager::InitializeFromDC();
    hiex::FPScontrol MainFPS(-1);
    hiex::FPScontrol renderFPS(60);

    init_MainWindow(&MainFPS);
    MainWindowDraw();
    
    init_transparentWindow();
    transparentWindowDraw();

    ExMessage msg;

    // 主循环
    while (1) {
        transparentWindowDraw();
        if (renderFPS.time_Sleep_bool())
        {
            if (peekmessage(&msg, -1,true, mainWindow_hwnd.GetHandle())) {
                MainWindowDraw();
            }
        }

        if (GetCloseFlag()) { break; }
        MainFPS.time_Sleep();
    }

    if (IsMenu(hMenu)) {DestroyMenu(hMenu);}
    closegraph();
    // 释放互斥句柄
    if (hMutex)
    {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
        hMutex = NULL;
    }
    return 0;
}