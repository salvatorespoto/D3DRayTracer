#include "D3D12RayTracer.h"

#include "DXDeviceResources.h"

D3D12RayTracer::D3D12RayTracer()
{}

D3D12RayTracer::~D3D12RayTracer() 
{}

void D3D12RayTracer::Init(HWND hWnd, unsigned int width, unsigned int height)
{
	m_deviceResources = std::make_unique<DXDeviceResources>(
		D3D_FEATURE_LEVEL_12_1, 
		DXDeviceResources::c_AllowTearingFlag,
		/* backBufferCount */ 3);
	
	m_deviceResources->Init();

	LOG("Ray tracer initialized.")
}