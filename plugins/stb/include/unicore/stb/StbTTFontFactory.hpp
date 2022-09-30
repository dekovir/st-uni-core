#pragma once
#include "unicore/Font.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/Memory.hpp"
#include <stb_truetype.h>

namespace unicore
{
	class BinaryData;

	class StbTTFontFactory : public TTFontFactory
	{
		UC_OBJECT(StbTTFontFactory, TTFontFactory)
	public:
		StbTTFontFactory(Renderer& renderer, const Shared<BinaryData>& data);

		UC_NODISCARD bool valid() const { return _valid; }

		UC_NODISCARD const stbtt_fontinfo* info() const { return &_font_info; }

		UC_NODISCARD size_t get_system_memory_use() const override;

		void get_font_metrics(int* ascent, int* descent, int* line_gap) const;

		void get_codepoint_metrics(WChar c, int* advance_width, int* left_side_bearing) const;
		MemoryChunk get_codepoint_bitmap(WChar c,
			const Vector2f& scale, Vector2i& size, Vector2i* offset = nullptr) const;

		UC_NODISCARD Shared<TexturedFont> create(const TTFontOptions& options, Logger* logger) override;

	protected:
		Renderer& _renderer;
		Shared<BinaryData> _data;
		stbtt_fontinfo _font_info{};
		bool _valid;
	};
}
#endif