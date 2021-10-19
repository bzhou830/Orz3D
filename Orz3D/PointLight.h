#pragma once
#include "Graphics.h"
#include "./Bindable/ConstantBuffers.h"

class PointLight
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow();
	void Reset();
	void Draw(Graphics& gfx);
	void Bind(Graphics& gfx);
private:
	struct PointLightCBuf
	{
		dx::XMFLOAT4 pos;
	};
	dx::XMFLOAT4 pos = { 0.0f, 0.0f, 0.0f, 0.0f };
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};

