#include "Framework/D3DApp.h"
#include <string.h>

class InitDX9App : public D3DApp
{
public:
	InitDX9App(HINSTANCE hInstance, std::wstring winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	virtual ~InitDX9App();

	virtual bool CheckDeviceCaps() override;
	virtual void OnLostDevice() override;
	virtual void OnResetDevice() override;
	virtual void UpdateScene(float dt) override;
	virtual void DrawScene() override;

private:
	ID3DXLine* mLine;
};

InitDX9App::InitDX9App(HINSTANCE hInstance, std::wstring winCaption, D3DDEVTYPE devType, DWORD requestedVP)
	: D3DApp(hInstance, winCaption, devType, requestedVP)
{
	srand(time_t(0));

	HR(D3DXCreateLine(GD3DDevice, &mLine));
}

InitDX9App::~InitDX9App()
{
	SafeRelease(mLine);
}

bool InitDX9App::CheckDeviceCaps()
{
	return true;
}

void InitDX9App::OnLostDevice()
{
	HR(mLine->OnLostDevice());
}

void InitDX9App::OnResetDevice()
{
	HR(mLine->OnResetDevice());
}

void InitDX9App::UpdateScene(float dt)
{
}

void InitDX9App::DrawScene()
{
	HR(GD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 0), 1.0f, 0));

	RECT formatRect;
	GetClientRect(mhMainWnd, &formatRect);

	HR(GD3DDevice->BeginScene());

	D3DXVECTOR2 line[] = {
		D3DXVECTOR2(0,0),
		D3DXVECTOR2(100.0f, 100.0f),
		D3DXVECTOR2(100.0f, 200.0f),
	};
	mLine->Begin();
	mLine->Draw(line, 3, D3DCOLOR_XRGB(255, 0, 0));
	mLine->End();

	HR(GD3DDevice->EndScene());
	HR(GD3DDevice->Present(0, 0, 0, 0));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	InitDX9App app(hInstance, L"Hello Direct3D 9", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	GApp = &app;
	return GApp->Run();
}

int main()
{
	char cmdLine[1];
	WinMain(GetModuleHandle(NULL), NULL, cmdLine, SW_SHOW);
	void *p = malloc(10);
	return 0;
}
