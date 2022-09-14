#pragma once
#include "unicore/ResourceCreator.hpp"
#if defined(UNICORE_USE_STB_EASY_FONT)
#include "EasyFont.hpp"

namespace unicore
{
	class EasyFontCreator : public ResourceCreatorData<EasyFont, int>
	{
		UC_OBJECT(EasyFontCreator, ResourceCreatorData)
	public:

	protected:
		Shared<EasyFont> create_from_data(const Options& context, const int& data) override;
	};
}
#endif