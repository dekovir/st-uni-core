#include "unicore/wasm/WasmFunction.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	WasmFunction::WasmFunction(IM3Function handle)
		: WasmValue(handle)
	{}

	StringView WasmFunction::name() const
	{
		return m3_GetFunctionName(_handle);
	}

	void WasmFunction::get_arguments(List<M3ValueType>& types) const
	{
		const auto count = m3_GetArgCount(_handle);
		for (unsigned i = 0; i < count; i++)
			types.push_back(m3_GetArgType(_handle, i));
	}

	void WasmFunction::get_returns(List<M3ValueType>& types) const
	{
		const auto count = m3_GetRetCount(_handle);
		for (unsigned i = 0; i < count; i++)
			types.push_back(m3_GetRetType(_handle, i));
	}

	UNICODE_STRING_BUILDER_FORMAT(M3ValueType)
	{
		switch (value)
		{
		case c_m3Type_none: return builder << "none";
		case c_m3Type_i32: return builder << "i32";
		case c_m3Type_i64: return builder << "i64";
		case c_m3Type_f32: return builder << "f32";
		case c_m3Type_f64: return builder << "i64";
		case c_m3Type_unknown: return builder << "unknown";
		}

		UC_ASSERT_ALWAYS_MSG("Invalid value");
		return builder << "error";
	}

	UNICODE_STRING_BUILDER_FORMAT(const WasmFunction&)
	{
		const auto handle = value.handle();

		// RET /////////////////////////////////////////////////////////////////////
		if (const auto ret_count = m3_GetRetCount(handle); ret_count == 0)
		{
			builder << "void";
		}
		else if (ret_count == 1)
		{
			builder << m3_GetRetType(handle, 0);
		}
		else
		{
			builder << "[";
			for (unsigned i = 0; i < ret_count; i++)
			{
				if (i > 0) builder << ",";
				builder << m3_GetRetType(handle, i);
			}
			builder << "]";
		}

		// NAME ////////////////////////////////////////////////////////////////////

		builder << " " << value.name() << "(";

		// ARG /////////////////////////////////////////////////////////////////////
		if (const auto arg_count = m3_GetArgCount(handle); arg_count == 0)
		{
		}
		else if (arg_count == 1)
		{
			builder << m3_GetArgType(handle, 0);
		}
		else
		{
			for (unsigned i = 0; i < arg_count; i++)
			{
				if (i > 0) builder << ",";
				builder << m3_GetArgType(handle, i);
			}
		}
		builder << ")";

		return builder;
	}
}