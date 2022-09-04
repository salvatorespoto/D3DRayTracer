#include "DXDeviceResources.h"

using std::vector;
using Microsoft::WRL::ComPtr;
using Utils::ThrowIfFailed;


DXDeviceResources::DXDeviceResources()
	: m_D3DMinFeatureLevel(D3D_FEATURE_LEVEL_12_1)
	, m_dxgiFactory(nullptr)
	, m_adapterDescription(nullptr)
	, m_adapterID(0)
	, m_optionsFlags(c_AllowTearingFlag)
{}

DXDeviceResources::DXDeviceResources(D3D_FEATURE_LEVEL minFeatureLevel, UINT flags)
	: m_D3DMinFeatureLevel(minFeatureLevel)
	, m_optionsFlags(flags)
	, m_adapterDescription(nullptr)
	, m_adapterID(0)
{}

void DXDeviceResources::Init()
{
#if defined(_DEBUG)
	EnableDebugLayer();
#endif

	CheckTearingSupport();
	InitializeDXGI();

#if defined(_DEBUG)
	PrintAdaptersInfo();
#endif

	CreateDeviceAdapter();
}

void DXDeviceResources::EnableDebugLayer()
{
	ComPtr<ID3D12Debug> debugController;
	// IID_PPV_ARGS macro is used to retrieve an interface pointer, supplying the IID value of the requested interface 
	// automatically based on the type of the interface pointer used. 
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
	else
	{
		LOG("WARNING: Direct3D Debug Device is not available\n");
	}

	ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiInfoQueue))))
	{
		ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_dxgiFactory)), "Cannot create DXGIFactory");

		dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
		dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
	}
}

void DXDeviceResources::CheckTearingSupport()
{
	// Variable refresh rate displays require TEARING to be enabled, this is also known as "vsync-off" support.
	if (m_optionsFlags & (c_AllowTearingFlag | c_RequireTearingSupportFlag))
	{
		BOOL allowTearing = FALSE;

		// Query the IDXGIFactory5 interface
		ComPtr<IDXGIFactory5> factory5;
		HRESULT hr = m_dxgiFactory.As(&factory5);

		if (SUCCEEDED(hr))
		{
			hr = factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
		}
		
		if (FAILED(hr) || !allowTearing)
		{
			LOG("WARNING: Variable refresh rate displays are not supported.\n");
			if (m_optionsFlags & c_RequireTearingSupportFlag)
			{
				ThrowIfFailed(hr, "Error: Sample must be run on an OS with tearing support.");
			}
			m_optionsFlags &= ~c_AllowTearingFlag;
		}
	}
}

void DXDeviceResources::InitializeDXGI()
{
	bool debugDXGI = false;

	if (!debugDXGI)
	{
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)), "Cannot Create DXGIFactory");
	}
}

#pragma region Adapters

vector<ComPtr<IDXGIAdapter1>> DXDeviceResources::EnumerateAdapters()
{
	UINT i = 0;
	UINT createFactoryFlags = 0;				// = DXGI_CREATE_FACTORY_DEBUG can be used here
	ComPtr<IDXGIFactory6> dxgiFactory;			// Com smart pointer to IDXGIFactory5
	ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)), "Cannot create DXGIFactory");

	ComPtr<IDXGIAdapter1> adapter;
	vector<ComPtr<IDXGIAdapter1>> adapterList;
	while (dxgiFactory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND)		// Get the i-th adapter
	{
		adapterList.push_back(adapter);
		++i;
	}

	return adapterList;
}

vector<ComPtr<IDXGIOutput>>  DXDeviceResources::EnumerateAdaptersOutputs(IDXGIAdapter* adapter)
{
	UINT i = 0;
	ComPtr <IDXGIOutput> output = nullptr;
	vector<ComPtr<IDXGIOutput>> outputList;
	while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		outputList.push_back(output);
		++i;
	}
	return outputList;
}

vector<DXGI_MODE_DESC> DXDeviceResources::EnumerateAdapterOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;		// This flag specifies modes to include in the list, 0 = 

	output->GetDisplayModeList(format, flags, &count, nullptr);	// Get display modes count
	vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]); // Get display modes list

	return modeList;
}

void DXDeviceResources::PrintAdaptersInfo()
{
	// Iterate over ADAPTERS and print info
	for (ComPtr<IDXGIAdapter1> adapter : EnumerateAdapters())
	{
		DXGI_ADAPTER_DESC aDesc;	// Struct that holds the adapter descrdiption
		adapter->GetDesc(&aDesc);	// Get the adapter description
		LOG("--- Adapter: " << aDesc.Description)

			// Iterate over ADAPTER OUTPUTS and print info
			for (ComPtr<IDXGIOutput> output : EnumerateAdaptersOutputs(adapter.Get()))
			{
				DXGI_OUTPUT_DESC oDesc;		// Struct that holds the output description
				output->GetDesc(&oDesc);	// Get the output description
				LOG("  Output: " << " Device name " << oDesc.DeviceName)
					// Iterate over ADAPTER OUTPUT DISPLAY MODES and print info
					for (DXGI_MODE_DESC displayMode : EnumerateAdapterOutputDisplayModes(output.Get(), DXGI_FORMAT_B8G8R8A8_UNORM))
					{
						LOG("    Display mode: " << " Width " << displayMode.Width << " Height " << displayMode.Height)
					}
			}
	}
}

void DXDeviceResources::CreateDeviceAdapter()
{
	ComPtr<IDXGIAdapter1> adapter;
	ComPtr<IDXGIFactory6> factory6;
	ThrowIfFailed(m_dxgiFactory.As(&factory6), ("DXGI 1.6 not supported"));

	// Choose the GPU that offers the maximum performances
	for (UINT adapterID = 0; DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(adapterID, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)); ++adapterID)
	{
		DXGI_ADAPTER_DESC1 desc;
		ThrowIfFailed(adapter->GetDesc1(&desc), "Cannot get adapter description");

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Exclude the Basic Render Driver adapter.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), m_D3DMinFeatureLevel, _uuidof(ID3D12Device), nullptr)))
		{
			m_adapterID = adapterID;
			m_adapterDescription = desc.Description;
			LOG("Direct3D Adapter (" << adapterID << "): VID: " << desc.VendorId << ", PID : " << desc.DeviceId << " - " << desc.Description);
			break;
		}
	}

	// Fall back on WARP software adapter
	if (!adapter)
	{
		// Try WARP12 instead
		ThrowIfFailed(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&adapter)), "WARP12 not available. Enable the 'Graphics Tools' optional feature");
		LOG("Direct3D Adapter - WARP12");
	}

	m_adapter = adapter.Detach();
}

#pragma endregion