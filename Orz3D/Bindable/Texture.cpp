#include "Texture.h"
#include "../GraphicsThrowMacros.h"

namespace wrl = Microsoft::WRL;

Texture::Texture(Graphics& gfx, std::shared_ptr<class Surface> s)
{
	INFOMAN(gfx);

	// create texture resource
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = 800;
	textureDesc.Height = 600;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = s->GetBufferPtr();
	sd.SysMemPitch = s->GetWidth() * sizeof(DWORD);
	
	GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, &sd, &pTexture));

	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView));

	surface = s;
}

void Texture::Bind(Graphics& gfx) noexcept
{
	Update(gfx);
	GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}


void Texture::Update(Graphics& gfx) noexcept
{
	UINT subResource = ::D3D11CalcSubresource(0, 0, 1);
	D3D11_MAPPED_SUBRESOURCE mappedTex;
	GetContext(gfx)->Map(pTexture.Get(), subResource, D3D11_MAP_WRITE_DISCARD, 0, &mappedTex);

	surface->Update();
	memcpy(mappedTex.pData, surface->GetBufferPtr(), surface->GetHeight() * surface->GetWidth() * sizeof(DWORD));

	GetContext(gfx)->Unmap(pTexture.Get(), subResource);
}

