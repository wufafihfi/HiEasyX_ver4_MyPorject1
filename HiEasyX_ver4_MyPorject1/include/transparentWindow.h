#pragma once

#include "HiEasyX.h"
#include "LS/LsWinodwStyle.h"
#include "MainWindow.h"
#include "TimeDeal.h"
#include "Ls/LsUI/LsDPIManager.h"
#include "HiEasyX/HiIcon.h"

#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <string>

extern hiex::Window transparentWindow_hwnd;

extern float windowAlpha;
extern int textSzie;
extern int text_r, text_g, text_b;
extern int text_x, text_y;

void init_transparentWindow();

void transparentWindow();

void transparentWindowDraw();

void transparentWindowLogic(float newAlpha);