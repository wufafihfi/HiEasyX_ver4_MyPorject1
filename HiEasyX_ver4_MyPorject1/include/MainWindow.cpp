#include "MainWindow.h"

#include "transparentWindow.h"
#include "../resource.h"

hiex::Window mainWindow_hwnd;
HMENU hMenu;
hiex::FPScontrol *_MainFPS;
bool close_flag = false;
float windowAlphaMW = 200;
Ls_UI::LsPlaneSelector TextSize_Slider;
Ls_UI::LsPlaneSelector TextAlpha_Slider;
Ls_UI::LsPlaneSelector TextR_Slider;
Ls_UI::LsPlaneSelector TextG_Slider;
Ls_UI::LsPlaneSelector TextB_Slider;
Ls_UI::LsPlaneSelector TextXY_Slider;
Ls_UI::LsButton theme_Button;

int TimeSettingWindowState = 0;
WNDPROC g_OriginWndProc = NULL;
Json::Value TimeConfigData;
void sliderDefiner();
void buttonDefiner();
void applyTheme();

int scaledWindowWidth = DPIManager::ScaleX(DEFAL_WINDOW_WIDTH);
int scaledWindowHeight = DPIManager::ScaleY(DEFAL_WINDOW_HEIGHT);

// 临时-主题切换
int count = 0;
// 主窗口背景
COLORREF backgroundColor = RGB(30,30,30);

// 自定义窗口过程，拦截关闭消息
LRESULT CALLBACK TrayWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
    {
        ShowWindow(hWnd, SW_HIDE);

        TimeSettingWindowState = 1;
        TimeConfigData["TimeSettingWindowState"] = TimeSettingWindowState;
        // 保存配置
        saveJsonFile(GetExePath() + "\\config\\TimeConfig.json", TimeConfigData);
        return 0;  // 阻止默认的关闭行为
    }

    case WM_GETMINMAXINFO:
    {
        // 获取 MINMAXINFO 结构体指针
        MINMAXINFO* pInfo = (MINMAXINFO*)lParam;
        // 设定窗口的最小宽度和高度（例如：400x300）
        pInfo->ptMinTrackSize.x = DPIManager::ScaleX(DEFAL_WINDOW_WIDTH + 16);
        pInfo->ptMinTrackSize.y = DPIManager::ScaleY(DEFAL_WINDOW_HEIGHT + 39);
        return 0;
    }

    case WM_MOUSEMOVE:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        // 传递给控件
        TextSize_Slider.OnMouseMove(x, y);
        TextAlpha_Slider.OnMouseMove(x, y);
        TextR_Slider.OnMouseMove(x, y);
        TextG_Slider.OnMouseMove(x, y);
        TextB_Slider.OnMouseMove(x, y);
        TextXY_Slider.OnMouseMove(x, y);

        theme_Button.OnMouseMove(x, y);

        break;
    }

    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        // 捕获鼠标，这样即使移出窗口也能收到 WM_LBUTTONUP
        SetCapture(hWnd);

        // 先让滑块尝试处理
        bool handled = false;
        handled = TextSize_Slider.OnMouseDown(x, y);
        if (!handled) {
            TextAlpha_Slider.OnMouseDown(x, y);
        }
        if (!handled) {
            TextR_Slider.OnMouseDown(x, y);
        }
        if (!handled) {
            TextG_Slider.OnMouseDown(x, y);
        }
        if (!handled) {
            TextB_Slider.OnMouseDown(x, y);
        }
        if (!handled) {
            TextXY_Slider.OnMouseDown(x, y);
        }

        // 如果没有滑块处理，可以执行其他逻辑
        if (!handled) {
            theme_Button.OnMouseDown(x, y);
        }
        break;
    }

    case WM_LBUTTONUP:
    {
        // 释放鼠标捕获
        ReleaseCapture();

        TextSize_Slider.OnMouseUp();
        TextAlpha_Slider.OnMouseUp();
        TextR_Slider.OnMouseUp();
        TextG_Slider.OnMouseUp();
        TextB_Slider.OnMouseUp();
        TextXY_Slider.OnMouseUp();

        theme_Button.OnMouseUp();

        // 保存配置
        saveJsonFile(GetExePath() + "\\config\\TimeConfig.json", TimeConfigData);
        break;
    }

    }
    // 其他消息交给原有的窗口过程处理
    return CallWindowProc(g_OriginWndProc, hWnd, uMsg, wParam, lParam);
}

// 菜单响应函数
void OnTray(UINT id)
{
    BEGIN_TASK();

    switch (id)
    {
    case IDC_A:
        MainWindow();
        MainWindowDraw();
        TimeSettingWindowState = 0;
        TimeConfigData["TimeSettingWindowState"] = TimeSettingWindowState;
        // 保存配置
        saveJsonFile(GetExePath() + "\\config\\TimeConfig.json", TimeConfigData);
        break;

    case IDC_B:
        close_flag = true;
        break;

    case IDC_C:
        transparentWindow();
        break;
    }

    END_TASK();
    REDRAW_WINDOW();
}

void init_MainWindow(hiex::FPScontrol* _MainFPS_) {
    mainWindow_hwnd.Create(
        DPIManager::ScaleX(DEFAL_WINDOW_WIDTH),
        DPIManager::ScaleY(DEFAL_WINDOW_HEIGHT),
        0, L"𝒃𝒛𝒘𝒇'𝒔 𝒔𝒄𝒓𝒆𝒆𝒏𝑻𝒐𝒐𝒍𝒔 v1.0 - 主窗口(目前)");

    DisableResizing(mainWindow_hwnd.GetHandle(), true);
    SetWindowPos(mainWindow_hwnd.GetHandle(), HWND_TOP, GetSystemMetrics(SM_CXSCREEN) / 2.0f, GetSystemMetrics(SM_CYSCREEN) / 2.0f,
        DPIManager::ScaleX(DEFAL_WINDOW_WIDTH), DPIManager::ScaleY(DEFAL_WINDOW_HEIGHT), SWP_NOSIZE);

    g_OriginWndProc = (WNDPROC)GetWindowLongPtr(mainWindow_hwnd.GetHandle(), GWLP_WNDPROC);
    SetWindowLongPtr(mainWindow_hwnd.GetHandle(), GWLP_WNDPROC, (LONG_PTR)TrayWndProc);

    mainWindow_hwnd.CreateTray(L"屏幕时钟-托盘");
    hMenu = CreatePopupMenu();
    HINSTANCE hInst = GetModuleHandle(NULL);
    HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MENU_TITLE));
    AppendMenu(hMenu, MF_BITMAP, IDB_MENU_TITLE, (LPCTSTR)hBitmap);
    AppendMenu(hMenu, MF_STRING, IDC_A, L"打开主窗口(时间显示设置)");
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenu, MF_STRING, IDC_C, L"刷新");
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenu, MF_STRING, IDC_B, L"退出");
    mainWindow_hwnd.SetTrayMenu(hMenu);					// 设置菜单
    mainWindow_hwnd.SetTrayMenuProcFunc(OnTray);		// 设置菜单响应函数

    // 帧率应用
    _MainFPS = _MainFPS_;

    // 配置文件读取
    TimeConfigData["timeRGBA"]["Alpha"] = 200;
    TimeConfigData["timeRGBA"]["Red"] = 0;
    TimeConfigData["timeRGBA"]["Green"] = 255;
    TimeConfigData["timeRGBA"]["Blue"] = 0;

    TimeConfigData["timePosition"]["x"] = 500;
    TimeConfigData["timePosition"]["y"] = 200;

    TimeConfigData["timeTextSize"] = 80;
    TimeConfigData["uiTheme"] = 0;

    TimeConfigData["TimeSettingWindowState"] = 0;

    if (!readJsonFile(GetExePath() + "\\config\\TimeConfig.json", TimeConfigData)) {
        MessageBoxW(NULL, _T("错误码:114\nBZD_CONFIGERRO_READFAILURE\n配置文件读取失败,将创建新文件"), _T("错误"), MB_OK | MB_ICONERROR);
    }

    windowAlpha = TimeConfigData["timeRGBA"]["Alpha"].asFloat();
    text_r = TimeConfigData["timeRGBA"]["Red"].asInt();
    text_g = TimeConfigData["timeRGBA"]["Green"].asInt();
    text_b = TimeConfigData["timeRGBA"]["Blue"].asInt();

    text_x = TimeConfigData["timePosition"]["x"].asInt();
    text_y = TimeConfigData["timePosition"]["y"].asInt();

    textSzie = TimeConfigData["timeTextSize"].asInt();

    TimeSettingWindowState = TimeConfigData["TimeSettingWindowState"].asInt();

    if (TimeSettingWindowState == 1) {
        ShowWindow(mainWindow_hwnd.GetHandle(), SW_HIDE);
    }

    // 滑块
    sliderDefiner();
    buttonDefiner();
    // 先初始化控件后应用保存的风格
    count = TimeConfigData["uiTheme"].asInt();
    applyTheme();
}

void MainWindow() {// 召唤窗口
    // 恢复正常帧率
    _MainFPS->set_FPS(-1);

    if (IsIconic(mainWindow_hwnd.GetHandle())) {
        ShowWindow(mainWindow_hwnd.GetHandle(), SW_RESTORE);
    }
    ShowWindow(mainWindow_hwnd.GetHandle(), SW_SHOW);
    SetForegroundWindow(mainWindow_hwnd.GetHandle());

    InvalidateRect(mainWindow_hwnd.GetHandle(), NULL, TRUE);
    UpdateWindow(mainWindow_hwnd.GetHandle());
}

void MainWindowDraw() {
    if (IsWindow(mainWindow_hwnd.GetHandle()) && 
        IsWindowVisible(mainWindow_hwnd.GetHandle())) 
    {
        // 恢复正常帧率
        _MainFPS->set_FPS(-1);

        BEGIN_TASK_WND(mainWindow_hwnd.GetHandle());
        setbkmode(TRANSPARENT);
        setbkcolor(backgroundColor);
        clearcliprgn();

        static wchar_t buf_title[] = L"ᚱᚢᚾᛖᛋ:ᛒᛉᚹᚠ'ᛋ ᛋᚲᚱᛖᛖᚾᛏᛟᛟᛚᛋ";
        settextstyle(DPIManager::ScaleX(30), 0, L"微软雅黑");
        settextcolor(RGB(text_r, text_g, text_b));
        setbkmode(TRANSPARENT);
        static int textWidth_title = textwidth(buf_title);
        static int textHeight_title = textheight(buf_title);
        outtextxy(DPIManager::ScaleX(scaledWindowWidth / 2.0f) - textWidth_title / 2.0f,
            DPIManager::ScaleX(10) - textHeight_title / 2.0f,
            buf_title);

        settextcolor(RGB(text_r, text_g, text_b));
        TimeShow(40, DPIManager::ScaleX(scaledWindowWidth / 2.0f), DPIManager::ScaleX(70));

        // 控件绘制
        TextSize_Slider.Draw();
        TextAlpha_Slider.Draw();
        TextR_Slider.Draw();
        TextG_Slider.Draw();
        TextB_Slider.Draw();
        TextXY_Slider.Draw();

        theme_Button.Draw();

        END_TASK();
        REDRAW_WINDOW();
    }
    else
    {
        // 主窗口不可见大降帧率
        _MainFPS->set_FPS(2);
    }
}

bool GetCloseFlag() {
    return close_flag;
}

// 创建滑块
void sliderDefiner() {
    int baseX = scaledWindowWidth / 2.0f - 250 / 2.0f + 40;
    int baseY = 120;
    int Ydelta = 30;
    // 字体大小
    TextSize_Slider.Create(baseX, baseY, 250, 0,
        20, 500,   // X 范围
        0, 0,   // Y 范围
        textSzie, 0, // 默认位置
        10,     // 圆点半径
        5, 5,    // 背景矩形拓展
        false,   // 关闭网格显示
        true,    // 使用圆角矩形
        10
    );
    TextSize_Slider.SetValueText(
        true,
        true,
        1, 0, 10, 5,
        true,
        L"微软雅黑",
        L"字体大小:"
    );
    TextSize_Slider.SetDeepSpaceTheme();
    TextSize_Slider.OnValueChanged([](int x, int y) {
        textSzie = x;
        TimeConfigData["timeTextSize"] = x;
        });

    baseY += Ydelta;
    // 透明度
    TextAlpha_Slider.Create(baseX, baseY, 250, 0,
        0, 255,   // X 范围
        0, 0,   // Y 范围
        windowAlpha, 0, // 默认位置
        10,     // 圆点半径
        5, 5,    // 背景矩形拓展
        false,   // 关闭网格显示
        true,    // 使用圆角矩形
        10
    );
    TextAlpha_Slider.SetValueText(
        true,
        true,
        1, 0, 10, 5,
        true,
        L"微软雅黑",
        L"透明度:"
    );
    TextAlpha_Slider.SetDeepSpaceTheme();
    TextAlpha_Slider.OnValueChanged([](int x, int y) {
        transparentWindowLogic(x);
        TimeConfigData["timeRGBA"]["Alpha"] = x;
        });

    baseY += Ydelta;
    // 颜色
    // R
    TextR_Slider.Create(baseX, baseY, 100, 0,
        0, 255,   // X 范围
        0, 0,   // Y 范围
        text_r, 0, // 默认位置
        10,     // 圆点半径
        5, 5,    // 背景矩形拓展
        false,   // 关闭网格显示
        true,    // 使用圆角矩形
        10
    );
    TextR_Slider.SetValueText(
        true,
        true,
        1, 0, 10, 5,
        true,
        L"微软雅黑",
        L"红:"
    );
    TextR_Slider.SetDeepSpaceTheme();
    TextR_Slider.OnValueChanged([](int x, int y) {
        text_r = x;
        TimeConfigData["timeRGBA"]["Red"] = x;
        });
    baseY += Ydelta;
    //G
    TextG_Slider.Create(baseX, baseY, 100, 0,
        0, 255,   // X 范围
        0, 0,   // Y 范围
        text_g, 0, // 默认位置
        10,     // 圆点半径
        5, 5,    // 背景矩形拓展
        false,   // 关闭网格显示
        true,    // 使用圆角矩形
        10
    );
    TextG_Slider.SetValueText(
        true,
        true,
        1, 0, 10, 5,
        true,
        L"微软雅黑",
        L"绿:"
    );
    TextG_Slider.SetDeepSpaceTheme();
    TextG_Slider.OnValueChanged([](int x, int y) {
        text_g = x;
        TimeConfigData["timeRGBA"]["Green"] = x;
        });
    baseY += Ydelta;
    //B
    TextB_Slider.Create(baseX, baseY, 100, 0,
        0, 255,   // X 范围
        0, 0,   // Y 范围
        text_b, 0, // 默认位置
        10,     // 圆点半径
        5, 5,    // 背景矩形拓展
        false,   // 关闭网格显示
        true,    // 使用圆角矩形
        10
    );
    TextB_Slider.SetValueText(
        true,
        true,
        1, 0, 10, 5,
        true,
        L"微软雅黑",
        L"蓝:"
    );
    TextB_Slider.SetDeepSpaceTheme();
    TextB_Slider.OnValueChanged([](int x, int y) {
        text_b = x;
        TimeConfigData["timeRGBA"]["Blue"] = x;
        });

    baseY += Ydelta;
    // 文本位置
    TextXY_Slider.Create(baseX + 120, baseY - Ydelta * 3, 130, 80,
        0, GetSystemMetrics(SM_CXSCREEN),   // X 范围
        0, GetSystemMetrics(SM_CYSCREEN),   // Y 范围
        text_x, text_y, // 默认位置
        10,     // 圆点半径
        5, 5,    // 背景矩形拓展
        true,   // 网格显示
        true,    // 使用圆角矩形
        10
    );
    TextXY_Slider.SetValueText(
        true,
        true,
        0, 3, 10, 5,
        true,
        L"微软雅黑",
        L"显示位置:"
    );
    TextXY_Slider.SetSnap(true, 2, 5, TextXY_Slider.GetWidth() / 2.0f, TextXY_Slider.GetHeight() / 2.0f);
    TextXY_Slider.SetDeepSpaceTheme();
    TextXY_Slider.OnValueChanged([](int x, int y) {
        text_x = x;
        TimeConfigData["timePosition"]["x"] = x;
        text_y = y;
        TimeConfigData["timePosition"]["y"] = y;
        });

}
// 创建按钮
void buttonDefiner() {
    theme_Button.Create(50, 260, 80, 25, L"主题切换", 20);
    theme_Button.SetStyle(Ls_UI::ButtonStyle::Round);
    theme_Button.SetNeonTheme();
    theme_Button.SetShadow(false);
    theme_Button.OnClick([]() {
        count += 1;
        if (count > 11) {
            count = 0;
        }
        applyTheme();
        TimeConfigData["uiTheme"] = count;
        // 保存配置
        saveJsonFile(GetExePath() + "\\config\\TimeConfig.json", TimeConfigData);
        });
}

void applyTheme() {
    switch (count)
    {
    case 0:  // 默认主题 (蓝灰经典)
        backgroundColor = RGB(240, 240, 240);  // 浅灰背景，与控件一致
        TextSize_Slider.SetDefaultTheme();
        TextAlpha_Slider.SetDefaultTheme();
        TextR_Slider.SetDefaultTheme();
        TextG_Slider.SetDefaultTheme();
        TextB_Slider.SetDefaultTheme();
        TextXY_Slider.SetDefaultTheme();
        theme_Button.SetDefaultTheme();
        break;

    case 1:  // 暗黑主题
        backgroundColor = RGB(35, 35, 35);     // 深灰背景，比控件稍深形成层次
        TextSize_Slider.SetDarkTheme();
        TextAlpha_Slider.SetDarkTheme();
        TextR_Slider.SetDarkTheme();
        TextG_Slider.SetDarkTheme();
        TextB_Slider.SetDarkTheme();
        TextXY_Slider.SetDarkTheme();
        theme_Button.SetDarkTheme();
        break;

    case 2:  // 热力图主题
        backgroundColor = RGB(255, 235, 215);  // 暖白背景，匹配暖色系
        TextSize_Slider.SetHeatmapTheme();
        TextAlpha_Slider.SetHeatmapTheme();
        TextR_Slider.SetHeatmapTheme();
        TextG_Slider.SetHeatmapTheme();
        TextB_Slider.SetHeatmapTheme();
        TextXY_Slider.SetHeatmapTheme();
        theme_Button.SetWarningTheme();
        break;

    case 3:  // 深空主题
        backgroundColor = RGB(15, 20, 35);     // 深蓝黑背景，匹配科技感
        TextSize_Slider.SetDeepSpaceTheme();
        TextAlpha_Slider.SetDeepSpaceTheme();
        TextR_Slider.SetDeepSpaceTheme();
        TextG_Slider.SetDeepSpaceTheme();
        TextB_Slider.SetDeepSpaceTheme();
        TextXY_Slider.SetDeepSpaceTheme();
        theme_Button.SetInfoTheme();
        break;

    case 4:  // 樱花主题
        backgroundColor = RGB(255, 235, 240);  // 樱花粉背景，柔和可爱
        TextSize_Slider.SetSakuraTheme();
        TextAlpha_Slider.SetSakuraTheme();
        TextR_Slider.SetSakuraTheme();
        TextG_Slider.SetSakuraTheme();
        TextB_Slider.SetSakuraTheme();
        TextXY_Slider.SetSakuraTheme();
        theme_Button.SetDangerTheme();
        break;

    case 5:  // 森林主题
        backgroundColor = RGB(225, 240, 225);  // 浅绿背景，清新自然
        TextSize_Slider.SetForestTheme();
        TextAlpha_Slider.SetForestTheme();
        TextR_Slider.SetForestTheme();
        TextG_Slider.SetForestTheme();
        TextB_Slider.SetForestTheme();
        TextXY_Slider.SetForestTheme();
        theme_Button.SetSuccessTheme();
        break;

    case 6:  // 日落主题
        backgroundColor = RGB(255, 220, 180);  // 暖橙背景，温暖黄昏感
        TextSize_Slider.SetSunsetTheme();
        TextAlpha_Slider.SetSunsetTheme();
        TextR_Slider.SetSunsetTheme();
        TextG_Slider.SetSunsetTheme();
        TextB_Slider.SetSunsetTheme();
        TextXY_Slider.SetSunsetTheme();
        theme_Button.SetWarningTheme();
        break;

    case 7:  // 海洋主题
        backgroundColor = RGB(200, 225, 240);  // 海洋蓝背景，清爽
        TextSize_Slider.SetOceanTheme();
        TextAlpha_Slider.SetOceanTheme();
        TextR_Slider.SetOceanTheme();
        TextG_Slider.SetOceanTheme();
        TextB_Slider.SetOceanTheme();
        TextXY_Slider.SetOceanTheme();
        theme_Button.SetDefaultTheme();
        break;

    case 8:  // 紫罗兰主题
        backgroundColor = RGB(240, 230, 250);  // 浅紫背景，优雅
        TextSize_Slider.SetVioletTheme();
        TextAlpha_Slider.SetVioletTheme();
        TextR_Slider.SetVioletTheme();
        TextG_Slider.SetVioletTheme();
        TextB_Slider.SetVioletTheme();
        TextXY_Slider.SetVioletTheme();
        theme_Button.SetWarningTheme();
        break;

    case 9:  // 石墨主题
        backgroundColor = RGB(30, 30, 35);     // 深灰背景，工业感
        TextSize_Slider.SetGraphiteTheme();
        TextAlpha_Slider.SetGraphiteTheme();
        TextR_Slider.SetGraphiteTheme();
        TextG_Slider.SetGraphiteTheme();
        TextB_Slider.SetGraphiteTheme();
        TextXY_Slider.SetGraphiteTheme();
        theme_Button.SetInfoTheme();
        break;

    case 10:  // 荧光主题
        backgroundColor = RGB(10, 10, 20);     // 极深背景，突出荧光效果
        TextSize_Slider.SetNeonTheme();
        TextAlpha_Slider.SetNeonTheme();
        TextR_Slider.SetNeonTheme();
        TextG_Slider.SetNeonTheme();
        TextB_Slider.SetNeonTheme();
        TextXY_Slider.SetNeonTheme();
        theme_Button.SetNeonTheme();
        break;

    case 11:  // 糖果主题
        backgroundColor = RGB(255, 240, 245);  // 糖果粉背景，甜美
        TextSize_Slider.SetCandyTheme();
        TextAlpha_Slider.SetCandyTheme();
        TextR_Slider.SetCandyTheme();
        TextG_Slider.SetCandyTheme();
        TextB_Slider.SetCandyTheme();
        TextXY_Slider.SetCandyTheme();
        theme_Button.SetDangerTheme();
        break;
    }
}