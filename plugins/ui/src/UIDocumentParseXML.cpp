#include "unicore/ui/UIDocumentParseXML.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/io/Logger.hpp"

namespace unicore
{
	static const Dictionary<StringView, UINodeTag> s_tag_type =
	{
		{"group", UINodeTag::Group},
		{"text", UINodeTag::Text},
		{"color", UINodeTag::Color},
		{"image", UINodeTag::Image},
		{"img", UINodeTag::Image},
		{"input", UINodeTag::Input},
		{"item", UINodeTag::Item},
		{"tree", UINodeTag::Tree},
		{"table", UINodeTag::Table},

		{"progress", UINodeTag::Progress},
	};

	static const Dictionary<StringView, UIAttribute> s_attr_name =
	{
		{"value", UIAttribute::Value},
		{"width", UIAttribute::Width},
		{"w", UIAttribute::Width},
		{"height", UIAttribute::Height},
		{"h", UIAttribute::Height},
		{"tooltip", UIAttribute::Tooltip},
		{"step", UIAttribute::Step},
		{"min", UIAttribute::Min},
		{"max", UIAttribute::Max},
	};

	static const Dictionary<StringView, UIGroupType> s_group_type = {
		{"vertical", UIGroupType::Vertical},
		{"horizontal", UIGroupType::Horizontal},
		{"child", UIGroupType::Child},
		{"list", UIGroupType::List},
		{"combo", UIGroupType::Combo},
		{"flex", UIGroupType::Flex},
		{"popup", UIGroupType::Popup},
		{"tooltip", UIGroupType::Tooltip},
		{"modal", UIGroupType::Modal},
	};

	static const Dictionary<StringView, UITextType> s_text_type = {
		{"h1", UITextType::Heading1},
		{"h2", UITextType::Heading2},
		{"h3", UITextType::Heading3},
		{"h4", UITextType::Heading4},
		{"h5", UITextType::Heading5},
		{"h6", UITextType::Heading6},
	};

	static const Dictionary<StringView, UIInputType> s_input_type =
	{
		{"textarea", UIInputType::TextArea},
		{"toggle", UIInputType::Toggle},
		{"radio", UIInputType::Radio},
		{"button", UIInputType::Button},
		{"image", UIInputType::Image},
		{"integer", UIInputType::Integer},
		{"float", UIInputType::Float},
		{"range", UIInputType::Range},
		{"vector2", UIInputType::Vector2},
		{"vector3", UIInputType::Vector3},
		{"color3", UIInputType::Color3},
		{"color4", UIInputType::Color4},
	};

	static const Dictionary<StringView, UIInputType> s_input_synonum =
	{
		{"textarea", UIInputType::TextArea},
		{"toggle", UIInputType::Toggle},
		{"radio", UIInputType::Radio},
		{"button", UIInputType::Button},
		{"slider", UIInputType::Range},
	};

	static const Dictionary<StringView, UITableType> s_table_type =
	{
		{"th", UITableType::Header},
		{"tr", UITableType::Row},
		{"td", UITableType::Cell},
	};

	using VariantType = StdVariant<std::nullopt_t, UIGroupType, UITextType, UIInputType, UITableType>;

	static Optional<UINodeTag> parse_tag(StringView tag, VariantType& variant)
	{
		for (const auto& it : s_tag_type)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = UIInputType::Text;
				return it.second;
			}
		}

		for (const auto& it : s_group_type)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = it.second;
				return UINodeTag::Group;
			}
		}

		for (const auto& it : s_text_type)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = it.second;
				return UINodeTag::Text;
			}
		}

		for (const auto& it : s_input_synonum)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = it.second;
				return UINodeTag::Input;
			}
		}

		for (const auto& it : s_table_type)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = it.second;
				return UINodeTag::Table;
			}
		}

		return std::nullopt;
	}

	static Variant parse_value(const char* str)
	{
		char* end;

		if (StringHelper::starts_with(StringView(str), "0x"sv))
		{
			const auto hex = strtoll(str + 2, &end, 16);
			return hex;
		}

		const auto i = strtoll(str, &end, 10);
		if (end[0] == 0)
			return i;

		const auto d = strtod(str, &end);
		if (end[0] == 0)
			return d;
		return str;
	}

	template<typename T>
	static T parse_enum_variant(const char* str, const Dictionary<StringView, T>& dict)
	{
		for (const auto& it : dict)
		{
			if (StringHelper::equals(str, it.first))
				return it.second;
		}

		return parse_value(str).get_enum<T>();
	}

	static void parse_node_recurse(const tinyxml2::XMLElement* node,
		UIDocument& doc, const UINode& parent, Logger* logger)
	{
		VariantType input_variant = std::nullopt;
		const auto tag = StringView(node->Value());
		const auto node_tag = parse_tag(tag, input_variant);

		if (!node_tag.has_value())
		{
			UC_LOG_WARNING(logger) << "Failed to parse '" << tag << "' tag at line " << node->GetLineNum();
			return;
		}

		if (node_tag.value() == UINodeTag::Group)
		{
			if (const auto str = node->Attribute("type"); str != nullptr)
				input_variant = parse_enum_variant<UIGroupType>(str, s_group_type);
		}

		if (node_tag.value() == UINodeTag::Text)
		{
			if (const auto str = node->Attribute("type"); str != nullptr)
				input_variant = parse_enum_variant<UITextType>(str, s_text_type);
		}

		if (node_tag.value() == UINodeTag::Input)
		{
			if (const auto str = node->Attribute("type"); str != nullptr)
				input_variant = parse_enum_variant<UIInputType>(str, s_input_type);
		}

		if (node_tag.value() == UINodeTag::Table)
		{
			if (const auto str = node->Attribute("type"); str != nullptr)
				input_variant = parse_enum_variant<UITableType>(str, s_table_type);
		}

		// Fill options
		UINodeOptions options;

		if (const auto ptr = std::get_if<UIGroupType>(&input_variant))
			options.attributes[UIAttribute::Type] = *ptr;

		if (const auto ptr = std::get_if<UITextType>(&input_variant))
			options.attributes[UIAttribute::Type] = *ptr;

		if (const auto ptr = std::get_if<UIInputType>(&input_variant))
			options.attributes[UIAttribute::Type] = *ptr;

		if (const auto ptr = std::get_if<UITableType>(&input_variant))
			options.attributes[UIAttribute::Type] = *ptr;

		if (const auto value = node->GetText(); value != nullptr)
		{
			const auto s = StringHelper::rtrim(StringView(value));
			options.attributes[UIAttribute::Text] = s;
		}

		if (const auto str = node->Attribute("id"); str != nullptr)
			options.uid = str;

		if (const auto str = node->Attribute("name"); str != nullptr)
			options.name = str;

		if (const auto str = node->Attribute("visible"); str != nullptr)
			options.visible = parse_value(str).get_bool(true);

		for (const auto& [name, type] : s_attr_name)
		{
			if (const auto t = node->Attribute(name.data()); t != nullptr)
				options.attributes[type] = parse_value(t);
		}

		const auto index = doc.create_node(node_tag.value(), options, parent);
		for (auto child = node->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
			parse_node_recurse(child, doc, index, logger);
	}

	Bool UIDocumentParseXML::parse(StringView xml,
		UIDocument& document, const UINode& parent, Logger* logger)
	{
		XMLData data;
		if (!data.parse(xml, logger))
			return false;

		return parse(data, document, parent, logger);
	}

	Bool UIDocumentParseXML::parse(const XMLData& data,
		UIDocument& document, const UINode& parent, Logger* logger)
	{
		const auto root = data.doc.RootElement();
		if (!root) return false;

		parse_node_recurse(root, document, parent, logger);
		return true;
	}
}
#endif