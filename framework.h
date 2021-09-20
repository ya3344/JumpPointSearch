#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <windowsx.h>
#include <timeapi.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <random>
// STL
#include <vector>
#include <stack>
#include <list>
#include <algorithm>
using namespace std;


// Memory Leack Check
#define CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )                                            
#endif

#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "winmm.lib") 

// CONOSP:E
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif