#include "transparentWindow.h"

hiex::Window transparentWindow_hwnd;
float windowAlpha = 200;
int textSzie = 80;
int text_r = 0, text_g = 255, text_b = 0;
int text_x = 700, text_y = 300;

time_t lastTimeCheck = 0;

void init_transparentWindow() {
    transparentWindow_hwnd.Create(640, 480, 0, L"悬浮内容显示窗口");

    LsWS::setWindowTransparent_Tool(transparentWindow_hwnd.GetHandle(),
        RGB(0, 0, 0),
        windowAlpha);// 设置透明全屏无边框鼠标穿透
}

void transparentWindow() {// 召唤窗口
    //if (IsIconic(transparentWindow_hwnd.GetHandle())) {
        //ShowWindow(transparentWindow_hwnd.GetHandle(), SW_RESTORE);
    //}
    //ShowWindow(transparentWindow_hwnd.GetHandle(), SW_SHOW);
    SetForegroundWindow(transparentWindow_hwnd.GetHandle());

    //InvalidateRect(transparentWindow_hwnd.GetHandle(), NULL, TRUE);
    //UpdateWindow(transparentWindow_hwnd.GetHandle());
}

void transparentWindowDraw() {
    if (IsWindow(transparentWindow_hwnd.GetHandle()) &&
        IsWindowVisible(transparentWindow_hwnd.GetHandle())) 
    {
        time_t now = time(nullptr);
        if (now == lastTimeCheck) { return; }

        BEGIN_TASK_WND(transparentWindow_hwnd.GetHandle());
        setbkmode(TRANSPARENT);
        setbkcolor(RGB(0, 0, 0));
        clearcliprgn();

        //putimage(500,100,HiEasyX::GetIconImage());

        settextcolor(RGB(text_r, text_g, text_b));
        TimeShow(textSzie, text_x, text_y);

        END_TASK();
        REDRAW_WINDOW();
    }
}

void transparentWindowLogic(float newAlpha) {
    if(newAlpha != windowAlpha)
    {
        windowAlpha = newAlpha;
        SetLayeredWindowAttributes(transparentWindow_hwnd.GetHandle(), 0, windowAlpha, LWA_ALPHA | LWA_COLORKEY);
    }
}