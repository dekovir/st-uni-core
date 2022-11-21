#include "unicore/ui/UIDocumentParseXML.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/io/Logger.hpp"

namespace unicore
{
	static const Dictionary<StringView, UINodeTag> s_tag_type =
	{
		{"group", UINodeTag::Group},
		{"visual", UINodeTag::Visual},
		{"input", UINodeTag::Input},
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
		{"vbox", UIGroupType::Vertical},
		{"hbox", UIGroupType::Horizontal},
		{"child", UIGroupType::Child},
		{"list", UIGroupType::List},
		{"tree", UIGroupType::Tree},
		{"combo", UIGroupType::Combo},
		{"flex", UIGroupType::Flex},
		{"table", UIGroupType::Table},
		{"th", UIGroupType::TableHeader},
		{"tr", UIGroupType::TableRow},
		{"td", UIGroupType::TableCell},
		{"popup", UIGroupType::Popup},
		{"tooltip", UIGroupType::Tooltip},
		{"modal", UIGroupType::Modal},
	};

	static const Dictionary<StringView, UIVisualType> s_visual_type = {
		{"text", UIVisualType::Text},
		{"color", UIVisualType::Color},
		{"image", UIVisualType::Image},
		{"img", UIVisualType::Image},
		{"progress", UIVisualType::Progress},
		{"separator", UIVisualType::Separator},
		{"bullet", UIVisualType::Bullet},
	};

	static const Dictionary<StringView, UIInputType> s_input_type =
	{
		{"textarea", UIInputType::TextArea},
		{"toggle", UIInputType::Toggle},
		{"radio", UIInputType::Radio},
		{"button", UIInputType::Button},
		{"item", UIInputType::Item},
		{"image", UIInputType::Image},
		{"integer", UIInputType::Integer},
		{"float", UIInputType::Float},
		{"range", UIInputType::RangeF},
		{"rangei", UIInputType::RangeI},
		{"rangef", UIInputType::RangeF},
		{"color3", UIInputType::Color3},
		{"color4", UIInputType::Color4},
	};

	static const Dictionary<StringView, UIInputType> s_input_synonum =
	{
		{"textarea", UIInputType::TextArea},
		{"toggle", UIInputType::Toggle},
		{"radio", UIInputType::Radio},
		{"button", UIInputType::Button},
		{"item", UIInputType::Item},
		{"number", UIInputType::Float},
		{"slider", UIInputType::RangeF},
	};

	using VariantType = StdVariant<std::nullopt_t, UIGroupType, UIVisualType, UIInputType>;

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

		for (const auto& it : s_visual_type)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = it.second;
				return UINodeTag::Visual;
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

		return std::nullopt;
	}

	static Variant parse_value(StringView str)
	{
		char* end;

		if (StringHelper::equals(str, "true", true))
			return true;

		if (StringHelper::equals(str, "false", true))
			return false;

		if (StringHelper::starts_with(str, "0x"sv))
		{
			const auto hex = strtoll(str.data() + 2, &end, 16);
			return hex;
		}

		const auto i = strtoll(str.data(), &end, 10);
		if (end - str.data() == str.size())
			return i;

		const auto d = strtod(str.data(), &end);
		if (end - str.data() == str.size())
			return d;

		return str;
	}

	template<typename T>
	static T parse_enum_variant(StringView str, const Dictionary<StringView, T>& dict)
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

		if (node_tag.value() == UINodeTag::Visual)
		{
			if (const auto str = node->Attribute("type"); str != nullptr)
				input_variant = parse_enum_variant<UIVisualType>(str, s_visual_type);
		}

		if (node_tag.value() == UINodeTag::Input)
		{
			if (const auto str = node->Attribute("type"); str != nullptr)
				input_variant = parse_enum_variant<UIInputType>(str, s_input_type);
		}

		// Fill options
		UINodeOptions options;

		if (const auto ptr = std::get_if<UIGroupType>(&input_variant))
			options.attributes[UIAttribute::Type] = *ptr;

		if (const auto ptr = std::get_if<UIVisualType>(&input_variant))
			options.attributes[UIAttribute::Type] = *ptr;

		if (const auto ptr = std::get_if<UIInputType>(&input_variant))
			options.attributes[UIAttribute::Type] = *ptr;

		if (const auto value = node->GetText(); value != nullptr)
		{
			const auto s = StringHelper::rtrim(StringView(value));
			options.attributes[UIAttribute::Text] = s;
		}
		else if (const auto str = node->Attribute("text"); str != nullptr)
		{
			options.attributes[UIAttribute::Text] = str;
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