#include "StbEasyFontCreator.hpp"
#if defined(UNICORE_USE_STB_EASY_FONT)
namespace unicore
{
	Shared<StbEasyFont> StbEasyFontCreator::create_from_data(const Options& context, const int& data)
	{
		return std::make_shared<StbEasyFont>(data);
	}
}
#endif