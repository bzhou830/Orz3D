#include "PointLight.h"

PointLight::PointLight(Graphics& gfx, float radius) :
	mesh(gfx, radius), 	cbuf(gfx)
{
}

void PointLight::SpawnControlWindow()
{
}

void PointLight::Reset()
{
	pos = { 0.0f, 0.0f, 0.0f, 0.0f };
}

void PointLight::Draw(Graphics& gfx)
{
	mesh.SetPos(pos);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx)
{
	cbuf.Update(gfx, PointLightCBuf{ pos });
}
