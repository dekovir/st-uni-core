#pragma once
#include "unicore/Font.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include <stb_truetype.h>

namespace unicore
{
	struct TTFOptions
	{
		int height;
		const WChar* chars = CharTable::Ascii.c_str();
	};

	class StbTTFont : public TexturedFont
	{
		UC_OBJECT(StbTTFont, TexturedFont)
	public:
		using CharInfo = Dictionary<uint32_t, stbtt_bakedchar>;

		struct ConstructionParams
		{
			CharInfo infos;
			Shared<Texture> texture;
			float height;
		};

		explicit StbTTFont(const ConstructionParams& params)
			: _infos(params.infos)
			, _texture(params.texture)
			, _height(params.height)
		{
		}

		UC_NODISCARD size_t get_system_memory_use() const override;
		UC_NODISCARD size_t get_used_resources(Set<Shared<Resource>>& resources) override;

		UC_NODISCARD float get_height() const override;
		UC_NODISCARD Vector2f calc_size(StringView text) const override;

		Shared<Texture> get_char_print_info(uint32_t code, Vector2f& pos, Rectf* rect, Rectf* uv_rect) const;

		void generate(const Vector2f& position, StringView text,
			Dictionary<Shared<Texture>, List<VertexTexColorQuad2>>& quad_dict) override;

	protected:
		const CharInfo _infos;
		Shared<Texture> _texture;
		float _height;
	};
}
#endif