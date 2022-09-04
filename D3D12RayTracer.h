#pragma once

#include "includes/common.h"

class DXDeviceResources;

/**
 * This class implements the ray tracer.
 */
class D3D12RayTracer
{
public:
	D3D12RayTracer(); 
	~D3D12RayTracer();

#pragma region Life Cycle
	void Init(HWND hWnd, unsigned int width, unsigned int height);
#pragma endregion

private:
	std::unique_ptr<DXDeviceResources> m_deviceResources;
};
