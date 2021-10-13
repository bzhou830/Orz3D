#pragma once
#include "BzWin.h"
#include <d3d11.h>
#include "BzException.h"
#include "DxgiInfoManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>
#include "renderdoc_app.h"
// 使用ComPtr方便对Com接口函数的管理
#include <wrl.h>
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#define ENABLE_RENDERDOC 0

class Graphics
{
	friend class Bindable;
public:
	class Exception : public BzException
	{
		using BzException::BzException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
		HRESULT hr;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};
public:
	Graphics(HWND hWnd);
	~Graphics() {};
	Graphics(const Graphics&) = delete;            //拷贝构造函数
	Graphics& operator=(const Graphics&) = delete; //赋值构造函数
	void ClearBuffer(float red, float green, float blue);
	void BeginFrame(float red, float green, float blue) noexcept
	{
		// imgui begin frame
		if (imguiEnabled)
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
		}

		const float color[] = { red,green,blue,1.0f };
		pContext->ClearRenderTargetView(pTarget.Get(), color);
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}
	void DrawIndexed(UINT count) noexcept(!IS_DEBUG);
	void SetProjection(DirectX::FXMMATRIX proj) noexcept
	{
		projection = proj;
	}
	DirectX::XMMATRIX GetProjection() const noexcept
	{
		return projection;
	}
	void EndFrame();
	RENDERDOC_API_1_1_2* rdoc_api;
private:
	void SetupDearImGui(HWND hWnd) const noexcept;
	bool imguiEnabled = true;
	DirectX::XMMATRIX projection;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pTarget;
	ComPtr<ID3D11DepthStencilView> pDSV;
	ComPtr<ID3D11Resource> pBackBuffer;
	ComPtr<ID3D11SamplerState> pSampler;
};

