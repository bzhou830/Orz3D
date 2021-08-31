#pragma once
#include "Bindable.h"
#include "../Surface.h"

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, class Surface& s);
	void Bind(Graphics& gfx) noexcept override;
	void Update(Graphics& gfx) noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	Surface& surface;
};
