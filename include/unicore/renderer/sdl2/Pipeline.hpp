#pragma once
#include "unicore/Renderer.hpp"
#include "unicore/renderer/sdl2/PipelineState.hpp"
#include "unicore/renderer/sdl2/PipelineRender.hpp"

namespace unicore::sdl2
{
	class Pipeline : public Renderer, public PipelineState, public PipelineRender
	{
	public:
		virtual void clear(const Color4b& color) = 0;
	};
}