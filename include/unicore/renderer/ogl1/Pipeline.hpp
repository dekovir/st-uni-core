#pragma once
#include "unicore/system/EnumFlag.hpp"
#include "unicore/math/Rect.hpp"
#include "unicore/renderer/Color4.hpp"
#include "unicore/renderer/Texture.hpp"
#include "unicore/renderer/ogl1/PipelineState.hpp"
#include "unicore/renderer/ogl1/Geometry.hpp"

namespace unicore::ogl1
{
	enum class ClearBit : uint8_t
	{
		Color = 1 << 0,
		Depth = 1 << 1,
		Stencil = 1 << 2,
	};
	UNICORE_ENUM_FLAGS(ClearBit, ClearMask);

	class Pipeline : public PipelineState, public Geometry
	{
	public:
		virtual void clear_color(Float r, Float g, Float b, Float a) = 0;
		virtual void clear_color(const Color4f& color) { clear_color(color.r, color.g, color.b, color.a); }

		virtual void clear_depth(Float depth) = 0;
		virtual void clear_stencil(Int s) = 0;

		virtual void clear(ClearMask mask) = 0;

		virtual void scissor(Int x, Int y, Int width, Int height) = 0;
		virtual void scissor(const Recti& rect) { scissor(rect.x, rect.y, rect.w, rect.h); }

		virtual void viewport(Int x, Int y, Int width, Int height) = 0;
		virtual void viewport(const Recti& rect) { viewport(rect.x, rect.y, rect.w, rect.h); }

		virtual void bind_texture(const Shared<Texture>& texture) = 0;

		virtual void draw_elements(RenderMode mode, size_t count, const UInt16* indices) = 0;
	};
}