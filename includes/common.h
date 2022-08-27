#pragma once

// Common includes
#include <Windows.h>

// Exclude rarely-used stuff from Windows headers.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             
#endif

// Statically link the required DirectX libraries
#pragma comment(lib, "D3d12.lib")
#pragma comment(lib, "dxgi.lib")       
#pragma comment(lib, "d3dcompiler.lib")
