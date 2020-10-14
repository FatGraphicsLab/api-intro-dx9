#pragma once

#include "Framework/D3DHeaders.h"
#include "Framework/D3DTimer.h"
#include <string>

class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance, std::wstring winCaption, D3DDEVTYPE devType = D3DDEVTYPE_HAL, DWORD requestedVP = D3DCREATE_HARDWARE_VERTEXPROCESSING);
	virtual ~D3DApp();

	HINSTANCE GetAppInst();
	HWND      GetMainWnd();

	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.
	virtual bool CheckDeviceCaps() { return true; }
	virtual void OnLostDevice() {}
	virtual void OnResetDevice() {}
	virtual void UpdateScene(float dt) {}
	virtual void DrawScene() {}

	// Override these methods only if you do not like the default window creation,
	// direct3D device creation, window procedure, or message loop.  In general,
	// for the sample programs of this book, we will not need to modify these.
	virtual void InitMainWindow();
	virtual void InitDirect3D();
	virtual int Run();
	virtual LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void EnableFullScreenMode(bool enable);
	bool IsDeviceLost();

protected:
	void CalculateFrameStats();

protected:
	// Derived client class can modify these data members in the constructor to
	// customize the application.
	std::wstring mMainWndCaption;
	D3DDEVTYPE   mDevType;
	DWORD        mRequestedVP;

	GameTimer mTimer;

	HINSTANCE mhAppInst;
	HWND mhMainWnd;
	IDirect3D9* md3dObject;
	bool mAppPaused;
	D3DPRESENT_PARAMETERS md3dPP;
};

extern D3DApp* GApp;
