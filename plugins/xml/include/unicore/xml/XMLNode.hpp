#pragma once
#include "unicore/Defs.hpp"
#include <tinyxml2.h>

namespace unicore
{
	template<typename Type = tinyxml2::XMLElement,
		std::enable_if_t<std::is_base_of_v<tinyxml2::XMLNode, Type>>* = nullptr>
	class XMLValue
	{
	public:
		constexpr XMLValue() noexcept
			: _data(nullptr) {}

		explicit constexpr XMLValue(Type* data) noexcept
			: _data(data) {}

		UC_NODISCARD constexpr bool empty() const { return _data == nullptr; }

		UC_NODISCARD XMLValue parent() const
		{
			return _data ? XMLValue(_data->Parent()) : XMLValue(nullptr);
		}

		UC_NODISCARD String value() const { return _data ? _data->Value() : ""; }

		template<typename U = Type>
		std::enable_if_t<!std::is_const_v<U>, void> set_value(StringView value)
		{
			if (_data)
				_data->SetValue(value.data());
		}

		UC_NODISCARD int line() const { return _data ? _data->GetLineNum() : 0; }

		// CHILDREN NODES
		UC_NODISCARD bool no_children() const { return _data ? _data->NoChildren() : true; }

		UC_NODISCARD XMLValue first_child()
		{
			return _data ? XMLValue(_data->FirstChild()) : XMLValue(nullptr);
		}

		UC_NODISCARD XMLValue last_child()
		{
			return _data ? XMLValue(_data->LastChild()) : XMLValue(nullptr);
		}

		// CHILDREN ELEMENTS
		UC_NODISCARD XMLValue first_child_element()
		{
			return _data ? XMLValue(_data->FirstChildElement()) : XMLValue(nullptr);
		}

		UC_NODISCARD XMLValue first_child_element(StringView name)
		{
			return _data ? XMLValue(_data->FirstChildElement(name.data())) : XMLValue(nullptr);
		}

		UC_NODISCARD XMLValue next_element()
		{
			return _data ? XMLValue(_data->NextSiblingElement()) : XMLValue(nullptr);
		}

		UC_NODISCARD XMLValue next_element(StringView name)
		{
			return _data ? XMLValue(_data->NextSiblingElement(name.data())) : XMLValue(nullptr);
		}

	protected:
		Type* _data;
	};
}