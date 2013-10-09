﻿// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから使用されていない部分を除外します。
#define _WIN32_DCOM 

#pragma warning(disable : 4995)
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")

#ifdef _DEBUG
#pragma comment(lib, "strmbasd.lib")
#else
#pragma comment(lib, "strmbase.lib")
#endif

#include <streams.h>
#include <commctrl.h>
#include <initguid.h>

// TODO: reference additional headers your program requires here
#include "MySourceExFilter.h"
#include "resource.h"
