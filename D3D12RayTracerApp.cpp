#include "D3D12RayTracerApp.h"

void ViewerApp::InitWindow()
{
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);                   // Size of the struct
	wndClass.style = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;  // Styles: here set redraw when horizontal or vertical resize occurs
	wndClass.lpfnWndProc = wndMsgCallback;                  // Handle to the procedure that will handle the window messages   
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_hInstance;                   // Application instance
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // Default application icon
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);     // Default cursor
	//wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   // White background 
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"ViewerApp";
	wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);     // Default small icon

	// It's mandatory to register the class to create the window associated
	if (!RegisterClassEx(&wndClass)) throw std::exception("Cannot register window class in WindowApp::initWindow()");

	DWORD style = WS_OVERLAPPEDWINDOW;                                      // Window styles
	DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;                     // Window extended styles
	RECT wr = { 0, 0, LONG(m_clientWidth), LONG(m_clientHeight) };          // Size of the window client area 
	AdjustWindowRectEx(&wr, style, FALSE, exStyle); // Compute the adjusted size of the window taking styles into account

	// Create the window
	m_hWnd = CreateWindowEx(
		exStyle,                        // Extended styles
		L"ViewerApp",		            // Class name
		L"GLTF Viewer",       			// Application name
		style,							// Window styles
		CW_USEDEFAULT,                  // Use defaults x coordinate
		CW_USEDEFAULT,	                // Use defaults y coordinate
		wr.right - wr.left,				// Width
		wr.bottom - wr.top,				// Height
		NULL,							// Handle to parent
		NULL,							// Handle to menu
		m_hInstance,				    // Application instance
		NULL);							// no extra parameters

	if (!m_hWnd) throw std::exception("Cannot create the window");

	// Save the pointer to this class so it's could be used from wndMsgCallback
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	m_appState.isAppWindowed = true;
	DEBUG_LOG("Main window initialized");
}
