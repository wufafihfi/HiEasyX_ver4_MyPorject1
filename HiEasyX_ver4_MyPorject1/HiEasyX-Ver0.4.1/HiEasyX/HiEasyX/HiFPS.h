/**
 * @file	HiFPS.h
 * @brief	HiEasyX 库的帧率模块
 * @author	huidong
*/

#pragma once
#include <chrono>

namespace HiEasyX
{
    //新
    class FPScontrol {
    private:
        int _FPS = 120;
        double _frameTimeMs = 8.333;  // 1000/120
        std::chrono::steady_clock::time_point _lastTime;
        bool _first = true;

    public:
        FPScontrol();
        FPScontrol(int setFPS);

        void set_FPS(int setFPS);

        double time_Sleep();
        bool time_Sleep_bool();
    };

    // 旧
	/**
	 * @brief 根据目标帧率延时
	 * @param[in] fps			帧率
	 * @param[in] wait_long		是否长等待（降低占用）
	*/
	void DelayFPS(int fps, bool wait_long = false);
};
