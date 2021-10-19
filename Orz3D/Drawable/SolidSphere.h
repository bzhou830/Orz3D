#pragma once
#include "DrawableBase.h"

class SolidSphere : public DrawableBase<SolidSphere>
{
public:
	SolidSphere(Graphics& gfx, float radius);
	void Update(float dt) noexcept override;
	void SetPos(dx::XMFLOAT4 pos);
	dx::XMMATRIX GetTransformXM() const noexcept;
private:
	dx::XMFLOAT4 pos = {1.0f, 1.0f, 1.0f, 0.f};
};