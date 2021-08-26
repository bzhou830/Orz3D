#pragma once
#include "BzWin.h"
#include <d3d11.h>
#include "BzException.h"
#include "DxgiInfoManager.h"

// ʹ��ComPtr�����Com�ӿں����Ĺ���
#include <wrl.h>
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class Graphics
{
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
	Graphics(const Graphics&) = delete;            //�������캯��
	Graphics& operator=(const Graphics&) = delete; //��ֵ���캯��
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawTestTriangle(float angle);
	void EndFrame();
private:
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pTarget;
};

