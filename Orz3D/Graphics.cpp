#include "Graphics.h"
#include "DxErr/dxerr.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "GraphicsThrowMacros.h"

namespace dx = DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0; // ���������Ϊ0��ʱ��back buffer�Ŀ�߽�����Ϊ�ʹ���һ��
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

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr; // GFX_THROW_FAILED ����Ҫ��������麯�����õķ���ֵ

	// ���� device, front/back buffers, �Լ� swap chain �� rendering context
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		pSwap.GetAddressOf(),
		pDevice.GetAddressOf(),
		nullptr,
		pContext.GetAddressOf()));

	// ��ȡback buffer����back buffer bind��render target�ϣ������Ϳ���ͨ��render target������back buffer.
	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, pTarget.GetAddressOf()));

	// create depth stensil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, pDSState.GetAddressOf()));

	// bind depth state
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// create depth stensil texture
	ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800u;
	descDepth.Height = 600u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, pDepthStencil.GetAddressOf()));

	// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, pDSV.GetAddressOf()));

	// bind depth stensil view to OM
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width    = 800.0f;
	vp.Height   = 600.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);
	
	SetupDearImGui(hWnd);
	//m_cap.open("d:/repos/cvdx/cvdx/111.mp4");
}

void Graphics::SetupDearImGui(HWND hWnd) const noexcept
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}


void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	GFX_THROW_INFO_ONLY(pContext->ClearRenderTargetView(pTarget.Get(), color));
	GFX_THROW_INFO_ONLY(pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u));
}

void Graphics::DrawIndexed(UINT count) noexcept(!IS_DEBUG)
{
	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0u, 0u));
}

//void Graphics::DrawTestTriangle(float angle, float x, float y)
//{
//	HRESULT hr = S_OK;
//	struct Vertex
//	{
//		struct  
//		{
//			float x;
//			float y;
//			float z;
//		} position;
//		struct 
//		{
//			float u;
//			float v;
//		} texCoord;
//	};
//
//	// create vertex buffer (1 2d triangle at center of screen)
//	const Vertex vertices[] =
//	{
//		{ -1.0f, -1.0f, -1.0f  , 0.0f, 1.0f},
//		{ 1.0f,-1.0f,-1.0f     , 1.0f, 1.0f},
//		{ -1.0f,1.0f,-1.0f     , 0.0f, 0.0f},
//		{ 1.0f,1.0f,-1.0f      , 1.0f, 0.0f},
//		{ -1.0f,-1.0f,1.0f     , 0.0f, 0.0f},
//		{ 1.0f,-1.0f,1.0f      , 1.0f, 0.0f},
//		{ -1.0f,1.0f,1.0f      , 0.0f, 1.0f},
//		{ 1.0f,1.0f,1.0f       , 1.0f, 1.0f},
//	};
//	
//	D3D11_BUFFER_DESC bd = { 0 };
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.CPUAccessFlags = 0u;
//	bd.MiscFlags = 0u;
//	bd.ByteWidth = sizeof(vertices);
//	bd.StructureByteStride = sizeof(Vertex);
//
//	D3D11_SUBRESOURCE_DATA sd = { 0 };
//	sd.pSysMem = vertices;
//	
//	ComPtr<ID3D11Buffer> pVertexBuffer;
//	GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, pVertexBuffer.GetAddressOf()));
//
//	// Bind vertex buffer to pipeline
//	const UINT stride = sizeof(Vertex);
//	const UINT offset = 0u;
//	GFX_THROW_INFO_ONLY(pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset));
//
//	// create index buffer
//	const unsigned short indices[] = {
//		0,2,1, 2,3,1,
//		1,3,5, 3,7,5,
//		2,6,3, 3,6,7,
//		4,5,7, 4,7,6,
//		0,4,2, 2,4,6,
//		0,1,4, 1,5,4
//	};
//	ComPtr<ID3D11Buffer> pIndexBuffer;
//	D3D11_BUFFER_DESC ibd = { 0 };
//	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	ibd.Usage = D3D11_USAGE_DEFAULT;
//	ibd.CPUAccessFlags = 0u;
//	ibd.MiscFlags = 0u;
//	ibd.ByteWidth = sizeof(indices);
//	ibd.StructureByteStride = sizeof(unsigned short);
//	D3D11_SUBRESOURCE_DATA isd = { 0 };
//	isd.pSysMem = indices;
//	GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, pIndexBuffer.GetAddressOf()));
//
//	// bind index buffer
//	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
//
//	// create constant buffer for transformation matrix
//	struct ConstantBuffer
//	{
//		dx::XMMATRIX transformation;
//	};
//	const ConstantBuffer cb = {
//		dx::XMMatrixTranspose(
//				dx::XMMatrixRotationX(angle) *
//				dx::XMMatrixTranslation(0, 0, 4.0f) *
//			/*dx::XMMatrixTranslation(0, 0, 4.0f) **/
//				dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f))
//	};
//	ComPtr<ID3D11Buffer> pConstantBuffer;
//	D3D11_BUFFER_DESC cbd = { 0 };
//	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	cbd.Usage = D3D11_USAGE_DYNAMIC;
//	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	cbd.MiscFlags = 0u;
//	cbd.ByteWidth = sizeof(cb);
//	cbd.StructureByteStride = 0u;
//	D3D11_SUBRESOURCE_DATA csd = { 0 };
//	csd.pSysMem = &cb;
//	GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, pConstantBuffer.GetAddressOf()));
//
//	// bind constant buffer to vertex shader
//	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
//
//	// create vertex shader
//	ComPtr<ID3D11VertexShader> pVertexShader;
//	ComPtr<ID3DBlob> pBlob;
//	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", pBlob.GetAddressOf()));
//	GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, pVertexShader.GetAddressOf()));
//	// bind vertex shader
//	GFX_THROW_INFO_ONLY(pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u));
//
//	// input (vertex) layout (2d position only)
//	ComPtr<ID3D11InputLayout> pInputLayout;
//	const D3D11_INPUT_ELEMENT_DESC ied[] =
//	{
//		// "Position" ��������vs�е�����������һһ��Ӧ��
//		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vertex::position), D3D11_INPUT_PER_VERTEX_DATA, 0},
//		//{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, sizeof(Vertex::position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	};
//	GFX_THROW_INFO(pDevice->CreateInputLayout(
//		ied, (UINT)std::size(ied),
//		pBlob->GetBufferPointer(),
//		pBlob->GetBufferSize(),
//		pInputLayout.GetAddressOf()));
//
//	// bind vertex layout
//	GFX_THROW_INFO_ONLY(pContext->IASetInputLayout(pInputLayout.Get()));
//
//	//���ļ��ж�ȡ��ͼ�����ݣ�ʹ��ͼ�����ݴ���texture
//	
//	// create texture resource
//	D3D11_TEXTURE2D_DESC textureDesc = {};
//	textureDesc.Width = 800u;
//	textureDesc.Height = 600u;
//	textureDesc.MipLevels = 1;
//	textureDesc.ArraySize = 1;
//	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//	textureDesc.SampleDesc.Count = 1;
//	textureDesc.SampleDesc.Quality = 0;
//	textureDesc.Usage = D3D11_USAGE_DEFAULT;
//	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//	textureDesc.CPUAccessFlags = 0;
//	textureDesc.MiscFlags = 0;
//
//	if (!m_cap.read(m_frame_bgr))
//		throw GFX_EXCEPT(hr);
//
//	cv::cvtColor(m_frame_bgr, m_frame_rgba, cv::COLOR_RGB2RGBA);
//	cv::resize(m_frame_rgba, m_frame_rgba, cv::Size(800, 600));
//
//	sd.pSysMem = m_frame_rgba.data;
//	sd.SysMemPitch = 800 * sizeof(DWORD);
//	ComPtr<ID3D11Texture2D> pTexture;
//	GFX_THROW_INFO(pDevice->CreateTexture2D(&textureDesc, &sd, &pTexture));
//
//	ComPtr<ID3D11ShaderResourceView> pTextureView;
//	// create the resource view on the texture
//	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//	srvDesc.Format = textureDesc.Format;
//	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MostDetailedMip = 0;
//	srvDesc.Texture2D.MipLevels = 1;
//	GFX_THROW_INFO(pDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView));
//
//	// create pixel shader
//	ComPtr<ID3D11PixelShader> pPixelShader;
//	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", pBlob.GetAddressOf()));
//	GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, pPixelShader.GetAddressOf()));
//	// bind pixel shader
//	GFX_THROW_INFO_ONLY(pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u));
//
//	GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf()));
//
//	D3D11_SAMPLER_DESC samplerDesc = {};
//	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//
//	GFX_THROW_INFO(pDevice->CreateSamplerState(&samplerDesc, &pSampler));
//	pContext->PSSetSamplers(0, 1, pSampler.GetAddressOf());
//
//	// Set primitive topology to triangle list (groups of 3 vertices)
//	GFX_THROW_INFO_ONLY(pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
//
//	GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
//}

void Graphics::EndFrame()
{
	// imgui frame end
	if (imguiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif
	if (FAILED(hr = pSwap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}

// Graphics exception�쳣������ش���
Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	: Exception(line, file), hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}


const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	: Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}


