#include "HiFPS.h"

#include "HiFunc.h"

#include <time.h>
#include <thread>

namespace HiEasyX
{
    // 新
    FPScontrol::FPScontrol()
    {
        _FPS = 120;
        _frameTimeMs = 1000.0 / _FPS;
    }
    FPScontrol::FPScontrol(int setFPS) {
        _FPS = setFPS;
        _frameTimeMs = 1000.0 / _FPS;
    }
    void FPScontrol::set_FPS(int setFPS) {
        if(_FPS != setFPS)
        {
            _FPS = setFPS;
            _frameTimeMs = 1000.0 / _FPS;
        }
    }
    double  FPScontrol::time_Sleep() {
        auto now = std::chrono::steady_clock::now();

        if (_first) {
            _lastTime = now;
            _first = false;
            return 0;
        }

        double elapsed = std::chrono::duration<double, std::milli>(now - _lastTime).count();

        if (elapsed < _frameTimeMs) {
            int sleepMs = (int)(_frameTimeMs - elapsed);
            if (sleepMs > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long>(sleepMs)));
                //Sleep(sleepMs);
            }
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));

        _lastTime = now;
        return elapsed;
    }
    bool FPScontrol::time_Sleep_bool() {
        auto now = std::chrono::steady_clock::now();

        if (_first) {
            _lastTime = now;
            _first = false;
            return true;  // 第一次，需要绘制
        }

        double elapsed = std::chrono::duration<double, std::milli>(now - _lastTime).count();

        if (elapsed >= _frameTimeMs) {
            _lastTime = now;
            return true;  // 到达帧时间，需要绘制
        }

        return false;  // 未到帧时间，不需要绘制
    }

    // 旧
	clock_t tRecord = 0;
	
    void DelayFPS(int fps, bool wait_long)
    {
        if (wait_long) {
            Sleep(500);
            return;
        }

        static LARGE_INTEGER lastTime = { 0 };
        static LARGE_INTEGER frequency;
        static bool init = false;

        if (!init) {
            QueryPerformanceFrequency(&frequency);
            init = true;
        }

        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);

        if (lastTime.QuadPart != 0) {
            double elapsed = (double)(now.QuadPart - lastTime.QuadPart) / frequency.QuadPart * 1000.0;
            int delay = (int)(1000.0 / fps - elapsed);

            if (delay > 0) {
                if (delay > 10) {
                    Sleep(delay - 5);  // 保留一些余量
                }
                else if (delay > 1) {
                    Sleep(1);
                }
                // delay <= 1时，直接让出时间片
                Sleep(0);
            }
        }
        lastTime = now;
    }
}

