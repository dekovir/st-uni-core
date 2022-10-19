#pragma once
#include "unicore/math/Rect.hpp"

namespace unicore
{
	class TemplateParser
	{
	public:
		enum class TokenType
		{
			None,
			Variable,
			If,
			Else,
			Loop,
			End,
		};

		struct Token
		{
			TokenType type;
			StringView32 str;
		};

		struct Data
		{
			String32 source;
			List<Token> tokens;

			~Data() = default;

			UC_TYPE_DEFAULT_MOVE(Data);
			UC_TYPE_DELETE_COPY(Data);
		};

		static bool parse_token(StringView32 str, Token& info, Logger* logger = nullptr);
		static Data parse(StringView32 source, Logger* logger = nullptr);
	};

	class Template
	{
	public:
		using ValueType = Variant<Int, Int64, Double, String, String32>;
		using DataType = Dictionary<String32, ValueType>;

		explicit Template(StringView32 str, Logger* logger = nullptr);

		UC_NODISCARD String32 execute(const DataType& data = {}) const;

	protected:
		Logger* _logger;
		TemplateParser::Data _data;
	};
}