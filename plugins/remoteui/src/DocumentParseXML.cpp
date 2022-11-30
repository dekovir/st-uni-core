#include "unicore/remoteui/DocumentParseXML.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/io/Logger.hpp"

namespace unicore::remoteui
{
	static const Dictionary<StringView, ElementTag> s_tag_type =
	{
		{"group", ElementTag::Group},
		{"visual", ElementTag::Visual},
		{"input", ElementTag::Input},
	};

	static const Dictionary<StringView, Attribute> s_attr_name =
	{
		{"name", Attribute::Name},
		{"value", Attribute::Value},
		{"hidden", Attribute::Hidden},
		{"disabled", Attribute::Disabled},
		{"width", Attribute::Width},
		{"w", Attribute::Width},
		{"height", Attribute::Height},
		{"h", Attribute::Height},
		{"tooltip", Attribute::Tooltip},
		{"step", Attribute::Step},
		{"min", Attribute::Min},
		{"max", Attribute::Max},
	};

	static const Dictionary<StringView, GroupType> s_group_type = {
		{"vertical", GroupType::Vertical},
		{"horizontal", GroupType::Horizontal},
		{"vbox", GroupType::Vertical},
		{"hbox", GroupType::Horizontal},
		{"child", GroupType::Child},
		{"list", GroupType::List},
		{"tree", GroupType::Tree},
		{"combo", GroupType::Combo},
		{"flex", GroupType::Flex},
		{"table", GroupType::Table},
		{"th", GroupType::TableHeader},
		{"tr", GroupType::TableRow},
		{"td", GroupType::TableCell},
		{"popup", GroupType::Popup},
		{"tooltip", GroupType::Tooltip},
		{"modal", GroupType::Modal},
	};

	static const Dictionary<StringView, VisualType> s_visual_type = {
		{"text", VisualType::Text},
		{"color", VisualType::Color},
		{"image", VisualType::Image},
		{"img", VisualType::Image},
		{"progress", VisualType::Progress},
		{"separator", VisualType::Separator},
		{"bullet", VisualType::Bullet},
	};

	static const Dictionary<StringView, InputType> s_input_type =
	{
		{"textarea", InputType::TextArea},
		{"toggle", InputType::Toggle},
		{"radio", InputType::Radio},
		{"button", InputType::Button},
		{"item", InputType::Item},
		{"image", InputType::Image},
		{"integer", InputType::Integer},
		{"float", InputType::Float},
		{"range", InputType::RangeF},
		{"rangei", InputType::RangeI},
		{"rangef", InputType::RangeF},
		{"color3", InputType::Color3},
		{"color4", InputType::Color4},
	};

	static const Dictionary<StringView, InputType> s_input_synonum =
	{
		{"textarea", InputType::TextArea},
		{"toggle", InputType::Toggle},
		{"radio", InputType::Radio},
		{"button", InputType::Button},
		{"item", InputType::Item},
		{"number", InputType::Float},
		{"slider", InputType::RangeF},
	};

	using VariantType = StdVariant<std::nullopt_t, GroupType, VisualType, InputType>;

	static Optional<ElementTag> parse_tag(StringView tag, VariantType& variant)
	{
		for (const auto& it : s_tag_type)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = InputType::Text;
				return it.second;
			}
		}

		for (const auto& it : s_group_type)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = it.second;
				return ElementTag::Group;
			}
		}

		for (const auto& it : s_visual_type)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = it.second;
				return ElementTag::Visual;
			}
		}

		for (const auto& it : s_input_synonum)
		{
			if (StringHelper::equals(it.first, tag, true))
			{
				variant = it.second;
				return ElementTag::Input;
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
		Document& doc, const Element& parent, Logger* logger)
	{
		VariantType input_variant = std::nullopt;
		const auto tag = StringView(node->Value());
		const auto node_tag = parse_tag(tag, input_variant);

		if (!node_tag.has_value())
		{
			UC_LOG_WARNING(logger) << "Failed to parse '" << tag << "' tag at line " << node->GetLineNum();
			return;
		}

		if (node_tag.value() == ElementTag::Group)
		{
			if (const auto str = node->Attribute("type"); str != nullptr)
				input_variant = parse_enum_variant<GroupType>(str, s_group_type);
		}

		if (node_tag.value() == ElementTag::Visual)
		{
			if (const auto str = node->Attribute("type"); str != nullptr)
				input_variant = parse_enum_variant<VisualType>(str, s_visual_type);
		}

		if (node_tag.value() == ElementTag::Input)
		{
			if (const auto str = node->Attribute("type"); str != nullptr)
				input_variant = parse_enum_variant<InputType>(str, s_input_type);
		}

		// Fill options
		ElementOptions options;

		if (const auto ptr = std::get_if<GroupType>(&input_variant))
			options.attributes[Attribute::Type] = *ptr;

		if (const auto ptr = std::get_if<VisualType>(&input_variant))
			options.attributes[Attribute::Type] = *ptr;

		if (const auto ptr = std::get_if<InputType>(&input_variant))
			options.attributes[Attribute::Type] = *ptr;

		if (const auto value = node->GetText(); value != nullptr)
		{
			const auto s = StringHelper::rtrim(StringView(value));
			options.attributes[Attribute::Text] = s;
		}
		else if (const auto str = node->Attribute("text"); str != nullptr)
		{
			options.attributes[Attribute::Text] = str;
		}

		//if (const auto str = node->Attribute("id"); str != nullptr) options.uid = str;

		if (const auto str = node->Attribute("visible"); str != nullptr)
			options.attributes[Attribute::Hidden] = !parse_value(str).get_bool(true);

		for (const auto& [name, type] : s_attr_name)
		{
			if (const auto t = node->Attribute(name.data()); t != nullptr)
				options.attributes[type] = parse_value(t);
		}

		const auto index = doc.create_node(node_tag.value(), options, parent);
		for (auto child = node->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
			parse_node_recurse(child, doc, index, logger);
	}

	Bool DocumentParseXML::parse(StringView xml,
		Document& document, const Element& parent, Logger* logger)
	{
		XMLData data;
		if (!data.parse(xml, logger))
			return false;

		return parse(data, document, parent, logger);
	}

	Bool DocumentParseXML::parse(const XMLData& data,
		Document& document, const Element& parent, Logger* logger)
	{
		const auto root = data.doc.RootElement();
		if (!root) return false;

		parse_node_recurse(root, document, parent, logger);
		return true;
	}
}
#endif