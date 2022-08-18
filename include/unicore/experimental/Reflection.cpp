#include "unicore/Reflection.hpp"
#include "unicore/Logger.hpp"
#include "unicore/Rect.hpp"

namespace unicore::Reflection
{
	UNICORE_REFLECTION_REGISTER()
	{
		RegisterFundamental<bool>("bool");

		RegisterFundamental<char>("char");
		RegisterFundamental<wchar_t>("wchar");

		RegisterFundamental<int8_t>("i8");
		RegisterFundamental<uint8_t>("ui8");

		RegisterFundamental<int16_t>("i16");
		RegisterFundamental<uint16_t>("ui16");

		RegisterFundamental<int32_t>("i32");
		RegisterFundamental<uint32_t>("ui32");

		RegisterFundamental<int64_t>("i64");
		RegisterFundamental<uint64_t>("ui64");

		RegisterFundamental<float>("f32");
		RegisterFundamental<double>("f64");

		UNICORE_REFLECTION_REGISTER_CLASS(Vector2i);
		UNICORE_REFLECTION_REGISTER_CLASS(Vector2f);

		UNICORE_REFLECTION_REGISTER_CLASS(Recti);
		UNICORE_REFLECTION_REGISTER_CLASS(Rectf);
	}

	struct ContextData
	{
		List<const Type*> types;
		Dictionary<TypeIndex, unsigned> index_type;
		Dictionary<StringView, unsigned> name_types;
	};
	static ContextData* s_context_data;

	const PropertyInfo* Type::find_property(StringView name) const
	{
		for (const auto& info : properties)
		{
			if (info->name == name)
				return info;
		}

		return nullptr;
	}

	const MethodInfo* Type::find_method(StringView name) const
	{
		for (const auto& info : methods)
		{
			if (info->name == name)
				return info;
		}

		return nullptr;
	}

	UNICODE_STRING_BUILDER_FORMAT(const Type&)
	{
		return builder << "<" << value.classname << ">";
	}

	void Context::add(const Type& type)
	{
		const auto data = get_data();

		const auto index = data->types.size();
		data->types.push_back(&type);

		data->name_types.emplace(type.classname, index);
		data->index_type.emplace(type.index, index);

		//std::cout << "Add '" << type.classname << "' type" << std::endl;
		//for (const auto& info : type.properties)
		//	std::cout << "\t" << *info << std::endl;
		//for (const auto& info : type.methods)
		//	std::cout << "\t" << *info << std::endl;
	}

	const Type* Context::find(TypeIndex index)
	{
		const auto data = get_data();
		const auto it = data->index_type.find(index);
		if (it == data->index_type.end())
			return nullptr;

		return data->types[it->second];
	}

	void Context::dump(Logger& logger)
	{
		const auto data = get_data();
		UC_LOG_INFO(logger) << "Total types: " << data->types.size();
		for (const auto type : data->types)
			UC_LOG_INFO(logger) << *type;
	}

	ContextData* Context::get_data()
	{
		if (s_context_data) return s_context_data;
		s_context_data = new ContextData();
		return s_context_data;
	}

	UNICODE_STRING_BUILDER_FORMAT(const PropertyInfo&)
	{
		return builder << value.name << ":" << value.type;
	}

	UNICODE_STRING_BUILDER_FORMAT(const MethodInfo&)
	{
		if (value.return_type)
			builder << *value.return_type;
		else builder << "void";

		builder << " " << value.name << "(";

		for (unsigned i = 0; i < value.arg_types.size(); i++)
		{
			if (i > 0) builder << ",";
			builder << *value.arg_types[i];
		}

		builder << ")";

		return builder;
	}
};