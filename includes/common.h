#pragma once

// Common includes
#include <Windows.h>
#include <windowsx.h>           // Windows helper functions
#include <wrl.h>                // ComPtr smart pointers for COM interfaces

#include <sstream>

// Exclude rarely-used stuff from Windows headers.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             
#endif

// Statically link the required DirectX libraries
#pragma comment(lib, "D3d12.lib")
#pragma comment(lib, "dxgi.lib")       
#pragma comment(lib, "d3dcompiler.lib")

namespace Utils
{
    /**  Log debug message to the Visual Studio output window */
    #define LOG(msg) { std::wstringstream wss; wss << msg << std::endl; OutputDebugString(wss.str().c_str()); }
}