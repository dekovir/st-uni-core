#include "unicore/ui/UITemplate.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	static constexpr StringView32 TokenBegin = U"::";
	static constexpr StringView32 TokenEnd = U"::";

	static const Dictionary<StringView32, TemplateParser::TokenType> ParamTokens =
	{
		{U"foreach", TemplateParser::TokenType::Loop},
		{U"if", TemplateParser::TokenType::If}
	};
	static const Dictionary<StringView32, TemplateParser::TokenType> SingleTokens =
	{
		{U"else", TemplateParser::TokenType::Else},
		{U"end", TemplateParser::TokenType::End}
	};

	bool TemplateParser::parse_token(StringView32 str, Token& info, Logger* logger)
	{
		if (const auto pos = str.find_first_of(U' '); pos != StringView32::npos)
		{
			const auto key = str.substr(0, pos);
			const auto value = str.substr(pos + 1);

			if (const auto it = ParamTokens.find(key); it != ParamTokens.end())
			{
				info = { it->second, value };
				return true;
			}

			UC_LOG_ERROR(logger) << "Invalid param token '" << str << "'";
			return false;
		}

		if (const auto it = SingleTokens.find(str); it != SingleTokens.end())
		{
			info = { it->second, U"" };
			return true;
		}

		info = { TokenType::Variable, str };
		return true;
	}

	TemplateParser::Data TemplateParser::parse(StringView32 source, Logger* logger)
	{
		constexpr size_t npos = StringView32::npos;
		constexpr size_t tag_begin_size = TokenBegin.size();

		size_t offset = 0;

		Data data{ String32(source), {} };

		while (true)
		{
			const auto start = source.find_first_of(TokenBegin, offset);
			if (start == npos)
				break;

			const auto end = source.find_first_of(TokenEnd, start + tag_begin_size);
			if (end == npos)
				return {};

			const auto before = source.substr(offset, start - offset);
			if (!before.empty())
				data.tokens.push_back({ TokenType::None, before });

			const auto tag = source.substr(start + tag_begin_size, end - start - tag_begin_size);
			Token token;
			if (parse_token(tag, token, logger))
				data.tokens.push_back(token);
			else return {};

			offset = end + TokenEnd.size();
		}

		const auto after = source.substr(offset);
		if (!after.empty())
			data.tokens.push_back({ TokenType::None, after });

		return data;
	}

	Template::Template(StringView32 str, Logger* logger)
		: _logger(logger), _data(TemplateParser::parse(str, _logger))
	{
	}

	String32 Template::execute(const DataType& data) const
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
			case TemplateParser::TokenType::None:
				builder << token.str;
				break;

			case TemplateParser::TokenType::Variable:
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
						else
							static_assert(false, "non-exhaustive visitor!");
						}, it->second);
				}
				else builder << StringBuilder::format(U"[{}]", token.str);
				break;
			}
		}

		return Unicode::to_utf32(builder.data);
	}
}