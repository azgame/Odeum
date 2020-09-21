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

#define NAME_D3D_RESOURCES 1
#define SAFE_DELETE( x ) { if( x ) delete x; x = NULL; }
#define SAFE_DELETE_ARRAY( x ) { if( x ) delete[] x; x = NULL; }

// #define ALIGN(_alignment, _val) _alignment = (((_val + _alignment - 1) / _alignment) * _alignment);
#define __FILENAME__ (strrchr(__FILE__,'\\')+1)

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define D3D12_GPU_VIRTUAL_ADDRESS_NULL 0ull
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN ~0ull

#ifdef ASSERT
#undef ASSERT
#endif

#ifdef min
#undef min
#endif

#ifdef RELEASE
	#define ASSERT(expression,...) (void)(expression)
#else //--Debug

	#define ASSERT(expression,...) \
		if (!(bool)(expression)) { \
			Debug::AssertionError(3, __VA_ARGS__, __FILENAME__, __LINE__); \
			__debugbreak(); \
	}

#endif

template <typename T> bool isAligned(T value_, size_t alignment_)
{
	return 0 == ((size_t)value_ & (alignment_ - 1));
}

template <typename T> T Align(T value_, size_t alignment_)
{
	return ((value_ + alignment_ - 1) / alignment_) * alignment_;
}

#endif