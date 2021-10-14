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

namespace dx = DirectX;

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
	void BeginFrame(float red, float green, float blue) noexcept;
	void EndFrame();
	void DrawIndexed(UINT count) noexcept(!IS_DEBUG);
	
	void SetProjection(dx::FXMMATRIX proj) noexcept;
	dx::XMMATRIX GetProjection() const noexcept;
	void SetCamera(dx::FXMMATRIX cam) noexcept;
	dx::XMMATRIX GetCamera() const noexcept;
	
#if ENABLE_RENDERDOC
	RENDERDOC_API_1_1_2* rdoc_api;
#endif
private:
	void SetupDearImGui(HWND hWnd) const noexcept;
	bool imguiEnabled = true;
	dx::XMMATRIX projection;
	dx::XMMATRIX camera;
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

