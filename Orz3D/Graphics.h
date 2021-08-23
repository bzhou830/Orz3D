#pragma once
#include "BzWin.h"
#include <d3d11.h>
#include <wrl.h>

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();
private:
	
	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11DeviceContext> pContext;
};

