#pragma once
#include "unicore/pattern/PatternParser.hpp"

namespace unicore
{
	class Pattern
	{
	public:
		using ValueType = StdVariant<Int, Int64, Double, String, String32>;
		using DataType = Dictionary<String32, ValueType>;

		explicit Pattern(StringView32 str, Logger* logger = nullptr);

		UC_NODISCARD String32 execute(const DataType& data = {}) const;

	protected:
		Logger* _logger;
		PatternParser::Data _data;
	};
}