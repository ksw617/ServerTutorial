#pragma once

#define WIN32_LEAN_AND_MEAN     

#ifdef _DEBUG
#pragma comment(lib, "Debug\\ServerCore.lib") // Debug ����϶�
#else
#pragma comment(lib, "Release\\ServerCore.lib") // Release ����϶�
#endif

#include <CorePch.h>