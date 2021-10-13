#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:
	vcbuf(gfx),
	parent(parent)
{}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	vcbuf.Update(gfx, dx::XMMatrixTranspose(
			parent.GetTransformXM() * 
			gfx.GetCamera()*
			gfx.GetProjection()
		)
	);
	vcbuf.Bind(gfx);
}
