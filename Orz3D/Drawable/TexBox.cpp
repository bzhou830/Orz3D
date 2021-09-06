#include "TexBox.h"
#include "../Bindable/BindableBase.h"
#include "../GraphicsThrowMacros.h"
#include "../Surface.h"

TexBox::TexBox(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
		struct
		{
			float u;
			float v;
		} texCoord;
	};

	const std::vector<Vertex> vertices =
	{
		{ -1.0f, -1.0f, -1.0f  , 0.0f, 1.0f},
		{  1.0f, -1.0f, -1.0f  , 1.0f, 1.0f},
		{ -1.0f,  1.0f, -1.0f  , 0.0f, 0.0f},
		{  1.0f,  1.0f, -1.0f  , 1.0f, 0.0f},
		{ -1.0f, -1.0f,  1.0f  , 0.0f, 0.0f},
		{  1.0f, -1.0f,  1.0f  , 1.0f, 0.0f},
		{ -1.0f,  1.0f,  1.0f  , 0.0f, 1.0f},
		{  1.0f,  1.0f,  1.0f  , 1.0f, 1.0f},
	};

	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShaderTex.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"PixelShaderTex.cso"));

	AddBind(std::make_unique<Sampler>(gfx));

	std::shared_ptr<Surface> surface = std::make_shared<Surface>("D:/111.mp4");
		
	AddTexture(std::make_unique<Texture>(gfx, surface));
	
	const std::vector<unsigned short> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vertex::pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void TexBox::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}


DirectX::XMMATRIX TexBox::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
