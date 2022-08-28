#include "D3D12RayTracerApp.h"

D3D12RayTracerApp::D3D12RayTracerApp(const HINSTANCE& hInstance, UINT ClientWidth, UINT ClientHeight) 
	: m_hInstance(hInstance), m_hWnd(NULL), m_clientWidth(ClientWidth), m_clientHeight(ClientHeight)
{}

D3D12RayTracerApp::~D3D12RayTracerApp() {}

#pragma region Life Cycle

void D3D12RayTracerApp::InitWindow()
{
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);                   
	wndClass.style = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;  // Window styles: redraw when horizontal or vertical resize occur
	wndClass.lpfnWndProc = WindowProc;             // Callback for the Windows Messages
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_hInstance;						// Application instance
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// Default application icon
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);			// Default cursor
	//wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   // White background 
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"D3D12RayTracerApp";
	wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);			// Default small icon

	// It's mandatory to register the class to create the window associated
	if (!RegisterClassEx(&wndClass))
	{
		throw std::exception("Cannot register window class in WindowApp::initWindow()");
	}

	DWORD style = WS_OVERLAPPEDWINDOW;                                      // Window styles
	DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;                     // Window extended styles
	RECT wr = { 0, 0, LONG(m_clientWidth), LONG(m_clientHeight) };          // Size of the window client area 
	AdjustWindowRectEx(&wr, style, FALSE, exStyle); // Compute the adjusted size of the window taking styles into account

	// Create the window
	m_hWnd = CreateWindowEx(
		exStyle,                        // Extended styles
		L"D3D12RayTracerApp",		       // Class name
		L"D3D12RayTracer",       		// Application name
		style,							// Window styles
		CW_USEDEFAULT,                  // Use defaults x coordinate
		CW_USEDEFAULT,	                // Use defaults y coordinate
		wr.right - wr.left,				// Width
		wr.bottom - wr.top,				// Height
		NULL,							// Handle to parent
		NULL,							// Handle to menu
		m_hInstance,				    // Application instance
		NULL);							// no extra parameters

	if (!m_hWnd)
	{
		throw std::exception("Cannot create the window");
	}

	// Save the pointer to the D3D12RayTracer class so it's could be used from in WindowMessageHanlder
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	LOG("Application Window initialized");
}


int D3D12RayTracerApp::Run()
{
	ShowWindow(m_hWnd, SW_MAXIMIZE);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	LOG("Entering application main loop");
	MSG msg = { WM_NULL };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) // Process window messages without blocking the loop
		{
			// Dispatch the message to the window callback procedure
			TranslateMessage(&msg);
			DispatchMessage(&msg);                
		}
		else
		{
			// Update and draw app
			OnUpdate();								
			OnDraw();
		}
	}
	OnDestroy();

	LOG("Leaving application main loop");
	return (int)msg.wParam;
}

#pragma endregion 

#pragma region Evemt Handlers

LRESULT D3D12RayTracerApp::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			return 0;
		}

		case WM_DESTROY:
		{
			return 0;
		}

		// The window has been closed
		case WM_CLOSE:          
		{
			PostQuitMessage(0);
			return 0;
		}

		// The user is dragging the window borders
		case WM_ENTERSIZEMOVE: 
		{
			return 0;
		}

		// The user finished dragging the window borders
		case WM_EXITSIZEMOVE:  
		{
			return 0;
		}

		case WM_SIZE:
		{
			break;
		}

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		{
			// wParam identifies which button has been pressed, while the macros GET_#_LPARAM get the coordinates from lParam
			return 0;
		}

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP: 
		{
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			return 0;
		}

		case WM_KEYDOWN:
		{
			return 0;
		}

		case WM_WINDOWPOSCHANGED:
		{
			return 0;
		}

		default:
		{
			break; 
		}
    }

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void D3D12RayTracerApp::OnEnterSizeMove()
{}

void D3D12RayTracerApp::OnExitSizeMove()
{}

void D3D12RayTracerApp::OnAppMinimized()
{}

void D3D12RayTracerApp::OnResize(UINT width, UINT height)
{}

void D3D12RayTracerApp::OnMouseMove(WPARAM btnState, int x, int y)
{}

void D3D12RayTracerApp::OnMouseDown(WPARAM btnState, int x, int y)
{}

void D3D12RayTracerApp::OnMouseUp(WPARAM btnState, int x, int y)
{}

void D3D12RayTracerApp::OnKeyDown(WPARAM wParam)
{}

void D3D12RayTracerApp::OnDestroy() 
{}

void D3D12RayTracerApp::OnUpdate()
{}

void D3D12RayTracerApp::OnDraw()
{}

#pragma region 