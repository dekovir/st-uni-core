#pragma once
#include "unicore/Defs.hpp"

namespace unicore::ogl1
{
	enum class StateName
	{
	};

	enum class Capability
	{
		AlphaTest,
		Blend,
		// ...
		CullFace,
		DepthTest,
		// ...
		ScissorTest,
		StencilTest,
		Texture2D,
	};

	class PipelineState
	{
	public:
		virtual ~PipelineState() = default;

		virtual void get_boolv(StateName name, Bool* params) = 0;
		virtual void get_intv(StateName name, Int* params) = 0;
		virtual void get_floatv(StateName name, Float* params) = 0;

		virtual void enable(Capability cap) = 0;
		virtual void disable(Capability cap) = 0;
		virtual Bool is_enabled(Capability cap) = 0;
	};
}