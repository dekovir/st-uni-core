#include "unicore/ui/UIDocumentParseXML.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/io/Logger.hpp"

namespace unicore
{
	static const Dictionary<StringView, UINodeType> s_tag_type =
	{
		{"window", UINodeType::Window},
		{"group", UINodeType::Group},
		{"text", UINodeType::Text},
		{"image", UINodeType::Image},
		{"button", UINodeType::Button},
		{"input", UINodeType::Input},
		{"slider", UINodeType::Slider},
		{"toggle", UINodeType::Toggle},
		{"tooltip", UINodeType::Tooltip},
		{"item", UINodeType::Item},
	};

	static const Dictionary<StringView, UIAttributeType> s_attr_name =
	{
		{"value", UIAttributeType::Value},
		{"tooltip", UIAttributeType::Tooltip},
		{"min", UIAttributeType::MinValue},
		{"max", UIAttributeType::MaxValue},
	};

	static UINodeType parse_tag(StringView tag)
	{
		for (const auto& it : s_tag_type)
		{
			if (StringHelper::equals(it.first, tag, true))
				return it.second;
		}

		return UINodeType::None;
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

	static void parse_node_recurse(const tinyxml2::XMLElement* node, UIDocument& doc, UINodeIndex parent, Logger* logger)
	{
		const auto tag = StringView(node->Value());
		const auto node_type = parse_tag(tag);

		if (node_type == UINodeType::None)
			UC_LOG_WARNING(logger) << "Failed to parse '" << tag << "' tag at line " << node->GetLineNum();

		// Fill options
		UINodeOptions options;
		if (const auto value = node->GetText(); value != nullptr)
			options.attributes[UIAttributeType::Text] = parse_value(value);

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

		const auto index = doc.create_node(node_type, parent, options);
		for (auto child = node->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
			parse_node_recurse(child, doc, index, logger);
	}

	Bool UIDocumentParseXML::parse(StringView xml,
		UIDocument& document, UINodeIndex parent, Logger* logger)
	{
		XMLData data;
		if (!data.parse(xml, logger))
			return false;

		return parse(data, document, parent, logger);
	}

	Bool UIDocumentParseXML::parse(const XMLData& data,
		UIDocument& document, UINodeIndex parent, Logger* logger)
	{
		const auto root = data.doc.RootElement();
		if (!root) return false;

		parse_node_recurse(root, document, parent, logger);
		return true;
	}
}
#endif