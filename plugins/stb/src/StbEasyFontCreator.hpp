#pragma once
#include "unicore/ResourceCreator.hpp"
#if defined(UNICORE_USE_STB_EASY_FONT)
#include "unicore/stb/StbEasyFont.hpp"

namespace unicore
{
	class StbEasyFontCreator : public ResourceCreatorData<StbEasyFont, int>
	{
		UC_OBJECT(StbEasyFontCreator, ResourceCreatorData)
	public:

	protected:
		Shared<StbEasyFont> create_from_data(const Options& context, const int& data) override;
	};
}
#endif