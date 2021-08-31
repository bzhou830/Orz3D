#pragma once
#include "../Graphics.h"
#include <DirectXMath.h>
#include "../Bindable/IndexBuffer.h"
#include "../Bindable/Texture.h"
class Bindable;

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	virtual void Update(float dt) noexcept = 0;
	void AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;
	void AddTexture(std::unique_ptr<Texture> tbuf) noexcept;
	virtual ~Drawable() = default;
private:
	const IndexBuffer* pIndexBuffer = nullptr;
	const Texture* pTexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};
