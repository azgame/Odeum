#ifndef _PCH_H_
#define _PCH_H_

#include <Windows.h>

#include <d3d12.h>
#include "Engine/Utilities/d3dx12.h"
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <dwrite_3.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <xaudio2.h>
#include <Xinput.h>

#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <list>
#include <string>
#include <string.h>

#include "Engine/Core/Debug.h"

#define NAME_D3D_RESOURCES 1
#define SAFE_DELETE( x ) { if( x ) delete x; x = NULL; }
#define SAFE_DELETE_ARRAY( x ) { if( x ) delete[] x; x = NULL; }

#define ALIGN(_alignment, _val) (((_val + _alignment - 1) / _alignment) * _alignment)
#define __FILENAME__ (strrchr(__FILE__,'\\')+1)

#endif // !_PCH_H_