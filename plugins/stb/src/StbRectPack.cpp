#include "unicore/stb/StbRectPack.hpp"
#if defined(UNICORE_USE_STB_RECT_PACK)
#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

namespace unicore
{
	bool StbRectPack::pack(const Vector2i& size, stbrp_rect* rects, int num_rects)
	{
		if (num_rects <= 0 || size.x <= 0 || size.y <= 0) return false;

		const auto num_nodes = Math::max<int>(size.x * 2, _nodes.size());

		_nodes.resize(num_nodes);
		stbrp_init_target(&_context, size.x, size.y, _nodes.data(), num_nodes);

		return !_nodes.empty() && stbrp_pack_rects(&_context, rects, num_rects) == 1;
	}

	bool StbRectPack::pack(const Vector2i& size, const List<Vector2i>& items, List<Recti>& packed)
	{
		if (items.empty() || size.x <= 0 || size.y <= 0) return false;

		List<stbrp_rect> rects;
		rects.resize(items.size());

		for (unsigned i = 0; i < items.size(); i++)
		{
			rects[i].id = i;
			rects[i].w = items[i].x;
			rects[i].h = items[i].y;
		}

		const auto result = pack(size, rects.data(), rects.size());
		packed.resize(items.size());

		for (unsigned i = 0; i < items.size(); i++)
		{
			const auto& rect = rects[i];
			if (rect.was_packed)
				packed[i] = { rect.x, rect.y, rect.w, rect.h };
			else packed[i] = RectConsti::Zero;
		}

		return result;
	}

	Vector2i StbRectPack::calc_start_size(stbrp_rect* rects, int num_rects)
	{
		int side = 64;
		while (!pack({ side, side }, rects, num_rects))
			side += 64;
		return { side, side };
	}

	Vector2i StbRectPack::calc_start_size(const List<Vector2i>& items)
	{
		List<stbrp_rect> rects;
		rects.resize(items.size());

		for (unsigned i = 0; i < items.size(); i++)
		{
			rects[i].id = i;
			rects[i].w = items[i].x;
			rects[i].h = items[i].y;
		}

		return calc_start_size(rects.data(), rects.size());
	}

	bool StbRectPack::pack_optimize(stbrp_rect* rects, int num_rects,
		Vector2i& size, const OptimizeOptions& options)
	{
		Vector2i last_size = VectorConst2i::Zero;
		int last_area = std::numeric_limits<int>::max();

		switch (options.method)
		{
		case OptimizeMethod::Squared:
			for (auto side = options.start_size.min_value(); side >= 0; side -= options.step)
			{
				size = Vector2i(side);
				if (pack(size, rects, num_rects))
					last_size = size;
				else break;
			}
			break;

		case OptimizeMethod::RectWidth:
			for (auto w = options.start_size.x; w > 0; w -= options.step)
				for (auto h = options.start_size.y; h > 0; h -= options.step)
				{
					size = Vector2i(w, h);
					if (pack(size, rects, num_rects))
					{
						const int area = size.area();
						if (area < last_area)
						{
							last_size = size;
							last_area = area;
						}
					}
					else break;
				}
			break;

		case OptimizeMethod::RectHeight:
			for (auto h = options.start_size.y; h > 0; h -= options.step)
				for (auto w = options.start_size.x; w > 0; w -= options.step)
				{
					size = Vector2i(w, h);
					if (pack(size, rects, num_rects))
					{
						const int area = size.area();
						if (area < last_area)
						{
							last_size = size;
							last_area = area;
						}
					}
					else break;
				}
			break;
		}

		if (last_size.x > 0 && pack(last_size, rects, num_rects))
		{
			size = last_size;
			return true;
		}

		return false;
	}

	bool StbRectPack::pack_optimize(const List<Vector2i>& items,
		List<Recti>& packed, Vector2i& size, const OptimizeOptions& options)
	{
		if (items.empty()) return false;

		List<stbrp_rect> rects;
		rects.resize(items.size());

		for (unsigned i = 0; i < items.size(); i++)
		{
			rects[i].id = i;
			rects[i].w = items[i].x;
			rects[i].h = items[i].y;
		}

		const auto result = pack_optimize(rects.data(), rects.size(), size, options);
		packed.resize(items.size());

		for (unsigned i = 0; i < items.size(); i++)
		{
			const auto& rect = rects[i];
			if (rect.was_packed)
				packed[i] = { rect.x, rect.y, rect.w, rect.h };
			else packed[i] = RectConsti::Zero;
		}

		return result;
	}
}
#endif