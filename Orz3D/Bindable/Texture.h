#pragma once
#include "Bindable.h"
#include "../Surface.h"

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, std::shared_ptr<class Surface> s);
	void Bind(Graphics& gfx) noexcept override;
	void Update(Graphics& gfx) noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView1;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture1;

	std::shared_ptr<class Surface> surface;
};
