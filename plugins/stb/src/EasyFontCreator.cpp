#include "EasyFontCreator.hpp"
#if defined(UNICORE_USE_STB_EASY_FONT)
namespace unicore
{
	Shared<EasyFont> EasyFontCreator::create_from_data(const Options& context, const int& data)
	{
		return std::make_shared<EasyFont>();
	}
}
#endif