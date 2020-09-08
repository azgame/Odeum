#ifndef _PCH_H_
#define _PCH_H_

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

// Windows
#include <Windows.h>

// DirectX
#include <d3d12.h>
#include "Engine/Utilities/d3dx12.h"
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <dwrite_3.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// C/C++ Libraries
#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>

#include <memory>
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

#define ALIGN(_alignment, _val) (((_val + _alignment - 1) / _alignment) * _alignment)
#define __FILENAME__ (strrchr(__FILE__,'\\')+1)

#ifdef ASSERT
#undef ASSERT
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

// Move to utilities file
std::wstring MakeWString(const std::string & string_)
{
	return std::wstring(string_.begin(), string_.end());
}

#endif