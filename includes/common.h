#pragma once

// Common includes
#include <Windows.h>
#include <windowsx.h>           // Windows helper functions
#include <wrl.h>                // ComPtr smart pointers for COM interfaces

// Direct3D includes
#include <d3d12.h>

// Include for Microsoft DirectX Graphics Infrastructure (DXGI)
// The Microsoft DirectX Graphics Infrastructure(DXGI) manages low - level tasks that can be independent of the 
// Direct3D graphics runtime like enumeration of hardware devices, presenting rendered frames to an output, 
// controlling gamma, and managing a full-screen transition were included in the Direct3D runtime
#include <dxgi1_6.h>			

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

#include <sstream>
#include <format>
#include <vector>

// Exclude rarely-used stuff from Windows headers.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             
#endif

// Statically link the required DirectX libraries
#pragma comment(lib, "D3d12.lib")
#pragma comment(lib, "dxgi.lib")       
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

namespace Utils
{
	/**  Log debug message to the Visual Studio output window */
	#define LOG(msg) { std::wstringstream wss; wss << msg << std::endl; OutputDebugString(wss.str().c_str()); }
	
	/** Throw exception on failure with a given message */
	inline void ThrowIfFailed(HRESULT hr, std::string errorMsg)
	{
		if (FAILED(hr))
		{
			errorMsg = "Error:" + errorMsg + "[file: " + __FILE__ + " line: " + std::to_string(__LINE__) + " ]";
			throw std::exception(errorMsg.c_str());
		}
	}
}