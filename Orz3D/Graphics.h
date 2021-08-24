#pragma once
#include "BzWin.h"
#include <d3d11.h>

// 使用ComPtr方便对Com接口函数的管理
#include <wrl.h>
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;            //拷贝构造函数
	Graphics& operator=(const Graphics&) = delete; //赋值构造函数
	~Graphics();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void EndFrame();
private:
	
	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pTarget;
};

