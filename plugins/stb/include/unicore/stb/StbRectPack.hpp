#pragma once
#include "unicore/Rect.hpp"
#if defined(UNICORE_USE_STB_RECT_PACK)
#include <stb_rect_pack.h>
#include "unicore/EnumFlag.hpp"

namespace unicore
{
	class StbRectPack
	{
	public:
		StbRectPack() = default;

		// TODO: Replace (rects, num_rects) with span
		bool pack(const Vector2i& size, stbrp_rect* rects, int num_rects);
		bool pack(const Vector2i& size, const List<Vector2i>& items, List<Recti>& packed);

		enum class OptimizeMethod
		{
			Squared,
			RectWidth,
			RectHeight,
		};

		struct OptimizeOptions
		{
			Vector2i start_size;
			int step;
			OptimizeMethod method = OptimizeMethod::RectWidth;
		};

		// TODO: Replace (rects, num_rects) with span
		bool pack_optimize(stbrp_rect* rects, int num_rects,
			Vector2i& size, const OptimizeOptions& options);
		bool pack_optimize(const List<Vector2i>& items, List<Recti>& packed,
			Vector2i& size, const OptimizeOptions& options);

	protected:
		stbrp_context _context;
		List<stbrp_node> _nodes;
	};
}
#endif