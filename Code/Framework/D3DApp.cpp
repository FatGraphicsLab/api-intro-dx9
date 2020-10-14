#include "Framework/D3DApp.h"

extern D3DApp* GApp = nullptr;

static LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (GApp)
		return GApp->MsgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance, std::wstring winCaption, D3DDEVTYPE devType, DWORD requestedVP)
{
	mMainWndCaption = winCaption;
	mDevType = devType;
	mRequestedVP = requestedVP;

	mhAppInst = hInstance;
	mhMainWnd = 0;
	md3dObject = 0;
	mAppPaused = false;
	ZeroMemory(&md3dPP, sizeof(md3dPP));

	InitMainWindow();
	InitDirect3D();
}

D3DApp::~D3DApp()
{
	SafeRelease(md3dObject);
	SafeRelease(GD3DDevice);
}

HINSTANCE D3DApp::GetAppInst()
{
	return mhAppInst;
}

HWND D3DApp::GetMainWnd()
{
	return mhMainWnd;
}

void D3DApp::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"D3D9WndClassName";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}

	RECT R = { 0, 0, 800, 600 };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	mhMainWnd = CreateWindow(L"D3D9WndClassName", mMainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, 100, 100, R.right, R.bottom, 0, 0, mhAppInst, 0);

	if (!mhMainWnd)
	{
		MessageBox(0, L"CreateWindow() FAILED", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);
}

void D3DApp::InitDirect3D()
{
	// Step 1: Create the IDirect3D9 object
	md3dObject = Direct3DCreate9(D3D_SDK_VERSION);
	if (!md3dObject)
	{
		MessageBox(0, L"Direct3DCreate9() FAILED", 0, 0);
		PostQuitMessage(0);
	}

	// Step 2: Verify hardware support for specified formats in windowed and full screen modes
	D3DDISPLAYMODE mode;
	md3dObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
	HR(md3dObject->CheckDeviceType(D3DADAPTER_DEFAULT, mDevType, mode.Format, mode.Format, true));
	HR(md3dObject->CheckDeviceType(D3DADAPTER_DEFAULT, mDevType, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false));

	// Step 3: Check for requested vertex processing and pure device.
	D3DCAPS9 caps;
	HR(md3dObject->GetDeviceCaps(D3DADAPTER_DEFAULT, mDevType, &caps));

	DWORD devBehaviorFlags = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		devBehaviorFlags |= mRequestedVP;
	else
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// if pure device and HW T&L supported
	if ((caps.DevCaps & D3DDEVCAPS_PUREDEVICE) &&
		(devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING))
	{
		devBehaviorFlags |= D3DCREATE_PUREDEVICE;
	}

	// Step 4: Fill out the D3DPRESENT_PARAMETERS structure.
	md3dPP.BackBufferWidth = 0;
	md3dPP.BackBufferHeight = 0;
	md3dPP.BackBufferFormat = D3DFMT_UNKNOWN;
	md3dPP.BackBufferCount = 1;
	md3dPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	md3dPP.MultiSampleQuality = 0;
	md3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	md3dPP.hDeviceWindow = mhMainWnd;
	md3dPP.Windowed = true;
	md3dPP.EnableAutoDepthStencil = true;
	md3dPP.AutoDepthStencilFormat = D3DFMT_D24S8;
	md3dPP.Flags = 0;
	md3dPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	md3dPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 5: Create the device
	HR(md3dObject->CreateDevice(
		D3DADAPTER_DEFAULT, // primay adapter
		mDevType,           // device type
		mhMainWnd,          // window associated with device
		devBehaviorFlags,   // vertex processing
		&md3dPP,            // present parameters
		&GD3DDevice));      // return created device
}

void D3DApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::wstring fpsStr = std::to_wstring(frameCnt);
		std::wstring msStr = std::to_wstring(mspf);

		std::wstring windowText = mMainWndCaption +
			L"    fps: " + fpsStr +
			L"    ms: " + msStr;

		SetWindowText(mhMainWnd, windowText.c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

int D3DApp::Run()
{
	MSG msg;
	msg.message = WM_NULL;

	mTimer.Reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			mTimer.Tick();

			// If the application is paused then free some CPU
			// cycles to other applications and then continue on
			// to the next frame
			if (mAppPaused)
			{
				Sleep(20);
				continue;
			}

			if (!IsDeviceLost())
			{
				CalculateFrameStats();
				UpdateScene(mTimer.DeltaTime());
				DrawScene();
			}
		}
	}

	return (int)msg.wParam;
}

LRESULT D3DApp::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool minOrMaxed = false;

	RECT clientRect = { 0, 0, 0, 0 };
	switch (msg)
	{
	case WM_ACTIVATE:
		mAppPaused = (LOWORD(wParam) == WA_INACTIVE);
		return 0;

	case WM_SIZE:
		if (GD3DDevice)
		{
			md3dPP.BackBufferWidth = LOWORD(lParam);
			md3dPP.BackBufferHeight = HIWORD(lParam);
			if (wParam == SIZE_MINIMIZED)
			{
				mAppPaused = true;
				minOrMaxed = true;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mAppPaused = false;
				minOrMaxed = true;
				OnLostDevice();
				HR(GD3DDevice->Reset(&md3dPP));
				OnResetDevice();
			}
			else if (wParam == SIZE_RESTORED)
			{
				// Restored is any resize that is not a minimize or maximize.
				// For example, restoring the window to its default size
				// after a minimize or maximize, or from dragging the resize
				// bars.
				mAppPaused = false;

				// Are we restoring from a minimized or maximized state,
				// and are in windowed mode? Do not execute this code if
				// we restoring to full screen mode.
				if (minOrMaxed && md3dPP.Windowed)
				{
					OnLostDevice();
					HR(GD3DDevice->Reset(&md3dPP));
					OnResetDevice();
				}
				else
				{
					// No, which implies the user is resizing by dragging
					// the resize bars. However, we do not reset the device
					// here because as the user continuously drags the resize
					// bars, a stream of WM_SIZE messages is sent to the window,
					// and it would be pointless (and slow) to reset for each
					// WM_SIZE message received from dragging the resize bars.
					// So instead, we reset after the user is done resizing the
					// window and releases the resize bars, which sends a
					// WM_EXITSIZEMOVE messages.
				}
				minOrMaxed = false;
			}
		}
		return 0;

	case WM_EXITSIZEMOVE:
		GetClientRect(mhMainWnd, &clientRect);
		md3dPP.BackBufferWidth = clientRect.right;
		md3dPP.BackBufferHeight = clientRect.bottom;
		OnLostDevice();
		HR(GD3DDevice->Reset(&md3dPP));
		OnResetDevice();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			EnableFullScreenMode(false);
		else if (wParam == 'F')
			EnableFullScreenMode(true);
		return 0;
	}

	return DefWindowProc(mhMainWnd, msg, wParam, lParam);
}

void D3DApp::EnableFullScreenMode(bool enable)
{
	if (enable)
	{
		if (!md3dPP.Windowed)
			return;

		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		md3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
		md3dPP.BackBufferWidth = width;
		md3dPP.BackBufferHeight = height;
		md3dPP.Windowed = false;

		// Change the window style to a more fullscreen friendly style.
		SetWindowLongPtr(mhMainWnd, GWL_STYLE, WS_POPUP);

		// If we call SetWindowLongPtr, MSDN states that we need to call
		// SetWindowPos for the change to take effect.  In addition, we
		// need to call this function anyway to update the window dimensions.
		SetWindowPos(mhMainWnd, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	else
	{
		if (md3dPP.Windowed)
			return;

		RECT R = { 0, 0, 800, 600 };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		md3dPP.BackBufferFormat = D3DFMT_UNKNOWN;
		md3dPP.BackBufferWidth = 800;
		md3dPP.BackBufferHeight = 600;
		md3dPP.Windowed = true;

		SetWindowLongPtr(mhMainWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(mhMainWnd, HWND_TOP, 100, 100, R.right, R.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
	}

	OnLostDevice();
	HR(GD3DDevice->Reset(&md3dPP));
	OnResetDevice();
}

bool D3DApp::IsDeviceLost()
{
	HRESULT hr = GD3DDevice->TestCooperativeLevel();

	if (hr == D3DERR_DEVICELOST)
	{
		// If the device is lost and cannot be reset yet then
		// sleep for a bit and we'll try again on the next
		// message loop cycle.
		Sleep(20);
		return true;
	}
	else if (hr == D3DERR_DRIVERINTERNALERROR)
	{
		MessageBox(0, L"Internal Driver Error, Exiting", 0, 0);
		PostQuitMessage(0);
		return true;
	}
	else if (hr == D3DERR_DEVICENOTRESET)
	{
		// The device is lost but we can reset and restore it
		OnLostDevice();
		HR(GD3DDevice->Reset(&md3dPP));
		OnResetDevice();
		return false;
	}
	else
		return false;
}
