#include "unicore/ui/UIDocumentParseXML.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/io/Logger.hpp"

namespace unicore
{
	static const Dictionary<StringView, UINodeType> s_tag_type =
	{
		{"group", UINodeType::Group},
		{"text", UINodeType::Text},
		{"image", UINodeType::Image},
		{"img", UINodeType::Image},
		{"input", UINodeType::Input},
		{"tooltip", UINodeType::Tooltip},
		{"item", UINodeType::Item},
		{"tree", UINodeType::Tree},
		{"combo", UINodeType::Combo},
		{"table", UINodeType::Table},
		{"th", UINodeType::TableHeader},
		{"tr", UINodeType::TableRow},
		{"td", UINodeType::TableCell},
		{"progress", UINodeType::Progress},
	};

	static const Dictionary<StringView, UIAttributeType> s_attr_name =
	{
		{"value", UIAttributeType::Value},
		{"width", UIAttributeType::Width},
		{"w", UIAttributeType::Width},
		{"height", UIAttributeType::Height},
		{"h", UIAttributeType::Height},
		{"tooltip", UIAttributeType::Tooltip},
		{"step", UIAttributeType::StepValue},
		{"min", UIAttributeType::MinValue},
		{"max", UIAttributeType::MaxValue},
	};

	static const Dictionary<StringView, UIGroupVariant> s_group_variant = {
		{"vertical", UIGroupVariant::Vertical},
		{"horizontal", UIGroupVariant::Horizontal},
		{"list", UIGroupVariant::List},
		{"flex", UIGroupVariant::Flex},
		{"popup", UIGroupVariant::Popup},
		{"modal", UIGroupVariant::Modal},
	};

	static const Dictionary<StringView, UIInputVariant> s_input_variant =
	{
		{"textarea", UIInputVariant::TextArea},
		{"toggle", UIInputVariant::Toggle},
		{"radio", UIInputVariant::Radio},
		{"button", UIInputVariant::Button},
		{"image", UIInputVariant::Image},
		{"number", UIInputVariant::Number},
		{"range", UIInputVariant::Range},
		{"vector2", UIInputVariant::Vector2},
		{"vector3", UIInputVariant::Vector3},
		{"color3", UIInputVariant::Color3},
		{"color4", UIInputVariant::Color4},
	};

	static const Dictionary<StringView, UIInputVariant> s_input_synonum =
	{
		{"textarea", UIInputVariant::TextArea},
		{"toggle", UIInputVariant::Toggle},
		{"radio", UIInputVariant::Radio},
		{"button", UIInputVariant::Button},
		{"slider", UIInputVariant::Range},
	};

	using VariantType = StdVariant<std::nullopt_t, UIGroupVariant, UIInputVariant>;

	static Optional<UINodeType> parse_tag(StringView tag, VariantType& variant)
	{
		for (const auto& it : s_tag_type)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = UIInputVariant::Text;
				return it.second;
			}
		}

		for (const auto& it : s_group_variant)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = it.second;
				return UINodeType::Group;
			}
		}

		for (const auto& it : s_input_synonum)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = it.second;
				return UINodeType::Input;
			}
		}

		return std::nullopt;
	}

	static Variant parse_value(const char* str)
	{
		char* end;

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
		const auto node_type = parse_tag(tag, input_variant);

		if (!node_type.has_value())
		{
			UC_LOG_WARNING(logger) << "Failed to parse '" << tag << "' tag at line " << node->GetLineNum();
			return;
		}

		if (node_type.value() == UINodeType::Group)
		{
			if (const auto str = node->Attribute("variant"); str != nullptr)
				input_variant = parse_enum_variant<UIGroupVariant>(str, s_group_variant);
		}

		if (node_type.value() == UINodeType::Input)
		{
			if (const auto str = node->Attribute("variant"); str != nullptr)
				input_variant = parse_enum_variant<UIInputVariant>(str, s_input_variant);
		}

		// Fill options
		UINodeOptions options;

		if (const auto ptr = std::get_if<UIGroupVariant>(&input_variant))
			options.attributes[UIAttributeType::Variant] = *ptr;

		if (const auto ptr = std::get_if<UIInputVariant>(&input_variant))
			options.attributes[UIAttributeType::Variant] = *ptr;

		if (const auto value = node->GetText(); value != nullptr)
		{
			const auto s = StringHelper::rtrim(StringView(value));
			options.attributes[UIAttributeType::Text] = s;
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

		const auto index = doc.create_node(node_type.value(), options, parent);
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