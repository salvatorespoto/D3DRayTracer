#pragma once

#include "includes/common.h"  


/**
    * Handle all the DirectX resources
    */
class DXDeviceResources
{

private:
	DXDeviceResources();

public:
	DXDeviceResources(D3D_FEATURE_LEVEL minFeatureLevel, UINT flags);
	~DXDeviceResources() {};

#pragma region Initilize resources
public:
	void Init();
	void EnableDebugLayer();
	void CheckTearingSupport();
	void InitializeDXGI();

private:
	D3D_FEATURE_LEVEL					  m_D3DMinFeatureLevel; // Min feature level required from this app
	Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
#pragma endregion

#pragma region Adapters
public:
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter1>> EnumerateAdapters();
	std::vector<Microsoft::WRL::ComPtr<IDXGIOutput>>  EnumerateAdaptersOutputs(IDXGIAdapter* adapter);
	std::vector<DXGI_MODE_DESC> EnumerateAdapterOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);
	void PrintAdaptersInfo();

	/* Acquire the first high performance hardware adapter that supports Direct3D 12.
	   If no such adapter can be found, try WARP. Otherwise throw an exception. */
	void InitializeAdapter();
	
private:
	UINT								  m_adapterID;
	LPCWSTR                               m_adapterDescription;
	Microsoft::WRL::ComPtr<IDXGIAdapter1> m_adapter;
	std::vector<DXGI_MODE_DESC>           m_displayModes;
#pragma endregion

#pragma region Getters
	IDXGIFactory4* GetDXGIFactory() const { return m_dxgiFactory.Get(); }
#pragma endregion

#pragma region Option Flags
public:	
	// Variable refresh rate displays require tearing to be enabled, this is also known as "vsync-off" support.
	static const unsigned int c_AllowTearingFlag = 0x1;
	static const unsigned int c_RequireTearingSupportFlag = 0x2;

private:
	// Holds the options setting flags
	UINT m_optionsFlags;
#pragma endregion

};
