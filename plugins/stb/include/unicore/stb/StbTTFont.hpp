#pragma once
#include "unicore/Font.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/Rect.hpp"
#include <stb_truetype.h>

namespace unicore
{
	class StbTTFont : public TexturedFont
	{
		UC_OBJECT(StbTTFont, TexturedFont)
	public:
		using CharInfo = Dictionary<uint32_t, stbtt_bakedchar>;

		UC_NODISCARD const Shared<Texture>& texture() const { return _texture; }

		struct ConstructionParams
		{
			CharInfo infos;
			Shared<Texture> texture;
			float height;
			float space_width;
		};

		explicit StbTTFont(const ConstructionParams& params)
			: _infos(params.infos)
			, _texture(params.texture)
			, _height(params.height)
			, _space_width(params.space_width)
		{
		}

		UC_NODISCARD size_t get_system_memory_use() const override;
		UC_NODISCARD size_t get_used_resources(Set<Shared<Resource>>& resources) override;

		UC_NODISCARD float get_height() const override;
		UC_NODISCARD float calc_width(StringView32 text) const override;

		Shared<Texture> get_char_print_info(uint32_t code, Vector2f& pos, Rectf* rect, Rectf* uv_rect) const;

		void generate(const Vector2f& position, StringView32 text, const Color4b& color,
			Dictionary<Shared<Texture>, List<VertexTexColorQuad2>>& quad_dict) override;

	protected:
		const CharInfo _infos;
		Shared<Texture> _texture;
		float _height;
		float _space_width;
	};
}
#endif