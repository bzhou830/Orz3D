#pragma once
#include "BzWin.h"
#include <d3d11.h>

// ʹ��ComPtr�����Com�ӿں����Ĺ���
#include <wrl.h>
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;            //�������캯��
	Graphics& operator=(const Graphics&) = delete; //��ֵ���캯��
	~Graphics();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void EndFrame();
private:
	
	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pTarget;
};

