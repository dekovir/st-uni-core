#include "unicore/pattern/Pattern.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	Pattern::Pattern(StringView32 str, Logger* logger)
		: _logger(logger), _data(PatternParser::parse(str, _logger))
	{
	}

	String32 Pattern::execute(const DataType& data) const
	{
		if (_data.tokens.empty())
		{
			UC_LOG_ERROR(_logger) << "Trying to execute empty template";
			return _data.source;
		}

		// TODO: Optimize
		StringBuilder builder;
		for (const auto& token : _data.tokens)
		{
			switch (token.type)
			{
			case PatternParser::TokenType::None:
				builder << token.str;
				break;

			case PatternParser::TokenType::Variable:
				const String32 key(token.str);
				if (auto it = data.find(key); it != data.end())
				{
					std::visit([&](auto&& arg) {
						using T = std::decay_t<decltype(arg)>;
						if constexpr (std::is_same_v<T, Int>)
							builder << arg;
						else if constexpr (std::is_same_v<T, Int64>)
							builder << arg;
						else if constexpr (std::is_same_v<T, Double>)
							builder << arg;
						else if constexpr (std::is_same_v<T, String>)
							builder << arg;
						else if constexpr (std::is_same_v<T, String32>)
							builder << arg;
						//else  static_assert(false, "non-exhaustive visitor!");
						}, it->second);
				}
				else builder << StringBuilder::format(U"[{}]", token.str);
				break;
			}
		}

		return Unicode::to_utf32(builder.data);
	}
}