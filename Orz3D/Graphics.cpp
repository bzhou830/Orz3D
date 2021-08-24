#include "Graphics.h"
#pragma comment(lib,"d3d11.lib")

Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
	// create device and front/back buffers, and swap chain and rendering context
	D3D11CreateDeviceAndSwapChain(nullptr,
								  D3D_DRIVER_TYPE_HARDWARE,
								  nullptr,
							      0,
							      nullptr,
								  0,
								  D3D11_SDK_VERSION,
								  &sd,
								  pSwap.GetAddressOf(),
								  pDevice.GetAddressOf(),
								  nullptr,
								  pContext.GetAddressOf());

	// 获取back buffer，将back buffer bind到render target上，这样就可以通过render target来操作back buffer.
	ComPtr<ID3D11Resource> pBackBuffer;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(pBackBuffer.GetAddressOf()));
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, pTarget.GetAddressOf());
}

Graphics::~Graphics()
{
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}
