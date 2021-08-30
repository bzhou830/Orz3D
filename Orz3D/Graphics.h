#pragma once
#include "BzWin.h"
#include <d3d11.h>
#include "BzException.h"
#include "DxgiInfoManager.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>

#include "opencv2/core.hpp"
#include "opencv2/core/directx.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

// 使用ComPtr方便对Com接口函数的管理
#include <wrl.h>
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

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
	void DrawIndexed(UINT count) noexcept(!IS_DEBUG);
	void SetProjection(DirectX::FXMMATRIX proj) noexcept
	{
		projection = proj;
	}
	DirectX::XMMATRIX GetProjection() const noexcept
	{
		return projection;
	}
	void DrawTestTriangle(float angle, float x, float y);
	void EndFrame();
private:
	void SetupDearImGui(HWND hWnd) const noexcept;
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

	cv::VideoCapture   m_cap;
	cv::Mat            m_frame_bgr;
	cv::Mat            m_frame_rgba;
};

