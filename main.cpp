#include "D3D12RayTracerApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{	
	D3D12RayTracerApp App(hInstance);
	App.Run();
	return 0;
}