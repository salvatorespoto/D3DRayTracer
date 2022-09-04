#pragma once

#include "includes/common.h"

class D3D12RayTracer;

/**
 * Handle the life cycle of the application: 
 */
class D3D12RayTracerApp
{

public:
	D3D12RayTracerApp(const HINSTANCE& hInstance, UINT ClientWidth = 1280, UINT ClientHeight = 1024);
	~D3D12RayTracerApp();

#pragma region Life Cycle
public:
	/** Create the app windows */
	virtual void InitWindow();

	/** Create the ray tracer */
	void InitRayTracer();

	/** App running cycle */
	int Run();

protected:
	/** Window message handler */
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#pragma endregion 


#pragma region Evemt Handlers
public:
	virtual void OnEnterSizeMove();
	virtual void OnExitSizeMove();
	virtual void OnAppMinimized();
	virtual void OnResize(UINT width, UINT height);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);
	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	virtual void OnKeyDown(WPARAM btnState);
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnDraw();
#pragma endregion 

private:

	HINSTANCE m_hInstance;
	HWND m_hWnd;			// Handle to the main window

	// Window client dimensions
	UINT m_clientWidth;
	UINT m_clientHeight;

	std::unique_ptr<D3D12RayTracer> m_rayTracer;
};