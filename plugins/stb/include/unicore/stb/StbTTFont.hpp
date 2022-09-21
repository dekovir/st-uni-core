#pragma once
#include "unicore/Font.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include <stb_truetype.h>

namespace unicore
{
	class StbTTFont : public TexturedFont
	{
		UC_OBJECT(StbTTFont, TexturedFont)
	public:
		using CharInfo = Dictionary<uint32_t, stbtt_bakedchar>;

		StbTTFont(const KerningDictionary& kerning, uint8_t space_width,
			CharInfo infos, const Shared<Texture>& texture)
			: TexturedFont(kerning, space_width),
			_infos(std::move(infos)),
			_texture(texture)
		{
		}

		UC_NODISCARD size_t get_system_memory_use() const override;
		UC_NODISCARD size_t get_used_resources(Set<Shared<Resource>>& resources) override;

		UC_NODISCARD float get_height() const override;
		UC_NODISCARD Vector2f calc_size(StringView text) const override;

		Shared<Texture> get_char_print_info(uint32_t code, Vector2f& pos, Rectf* rect, Rectf* uv_rect) const override;

	protected:
		const CharInfo _infos;
		Shared<Texture> _texture;
	};
}
#endif