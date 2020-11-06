#ifndef _PCH_H_
#define _PCH_H_

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

// Windows
#include <Windows.h>

// C/C++ Libraries
#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>

#include <memory>
#include <thread>
#include <mutex>

#include <vector>

#include <string>
#include <string.h>
#include <sstream>
#include <iomanip>

// Engine
#include "Engine/Core/Debug.h"
#include "Engine/Core/EngineProfiling.h"
#include "Engine/Core/Utility.h"
#include "Engine/Math/MathUtility.h"

// Imgui
#include "Engine/ImGui/imgui.h"
#include "Engine/ImGui/imgui_impl_win32.h"
#include "Engine/ImGui/imgui_impl_dx12.h"

#define NAME_D3D_RESOURCES 1
#define SAFE_DELETE( x ) { if( x ) delete x; x = NULL; }
#define SAFE_DELETE_ARRAY( x ) { if( x ) delete[] x; x = NULL; }

#define __FILENAME__ (strrchr(__FILE__,'\\')+1)

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define D3D12_GPU_VIRTUAL_ADDRESS_NULL 0ull
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN ~0ull

#ifdef ASSERT
#undef ASSERT
#endif

#ifdef LOG
#undef LOG
#endif

#ifdef ERROR
#undef ERROR
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifdef RELEASE
	#define ASSERT(expression,...) (void)(expression)
	#define LOG(...) (void)()
	#define ERROR(...) (void)()
	#define BREAKPOINT() (void)()
	#define SCOPEDTIMER(name) (void)(name)
#else //--Debug

	#define ASSERT(expression,...) \
		if (!(bool)(expression)) { \
			Debug::AssertionError(3, __VA_ARGS__, __FILENAME__, __LINE__); \
			__debugbreak(); \
	}

	#define LOG(...) \
		Debug::Info(__VA_ARGS__, __FILENAME__, __LINE__);

	#define ERROR(...) \
		Debug::Error(__VA_ARGS__, __FILENAME__, __LINE__);

	#define BREAKPOINT() __debugbreak();

	#define SCOPEDTIMER(name) ScopedTimer name(__FUNCTION__, __FILENAME__, __LINE__);

#endif

#endif