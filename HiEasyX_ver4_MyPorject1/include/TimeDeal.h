#pragma once
#include "Ls/LsUI/LsDPIManager.h"

#include "HiEasyX.h"

#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <string>

inline std::tm getTime_Chrono() {
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();

    // 转换为time_t类型
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // 转换为本地时间结构体
    std::tm local_time;
    localtime_s(&local_time, &now_time);

    return local_time;
}

inline void TimeShow(int fontSize, int x, int y) {
    static time_t lastTimeCheck = 0;
    static wchar_t cachedTimeStr[64];

    time_t now = time(nullptr);

    // 只在秒数变化时更新时间字符串
    if (now != lastTimeCheck) {
        lastTimeCheck = now;
        struct tm local_time;
        localtime_s(&local_time, &now);

        wcsftime(cachedTimeStr, 64, L"%Y-%m-%d %H:%M:%S", &local_time);
    }

    // 设置文字样式
    settextstyle(DPIManager::ScaleX(fontSize), 0, L"微软雅黑");
    setbkmode(TRANSPARENT);

    int textWidth = textwidth(cachedTimeStr);
    int textHeight = textheight(cachedTimeStr);

    outtextxy(x - textWidth / 2.0f, y - textHeight / 2.0f, cachedTimeStr);
}