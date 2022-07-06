#include "unicore/wasm/WAFunction.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	WAFunction::WAFunction(IM3Function handle)
		: WAValue(handle)
	{}

	StringView WAFunction::name() const
	{
		return m3_GetFunctionName(_handle);
	}

	void WAFunction::get_arguments(List<M3ValueType>& types) const
	{
		const auto count = m3_GetArgCount(_handle);
		for (unsigned i = 0; i < count; i++)
			types.push_back(m3_GetArgType(_handle, i));
	}

	void WAFunction::get_returns(List<M3ValueType>& types) const
	{
		const auto count = m3_GetRetCount(_handle);
		for (unsigned i = 0; i < count; i++)
			types.push_back(m3_GetRetType(_handle, i));
	}

	LogHelper& operator<<(LogHelper& helper, M3ValueType type)
	{
		switch (type)
		{
		case c_m3Type_none: return helper << "none";
		case c_m3Type_i32: return helper << "i32";
		case c_m3Type_i64: return helper << "i64";
		case c_m3Type_f32: return helper << "f32";
		case c_m3Type_f64: return helper << "i64";
		case c_m3Type_unknown: return helper << "unknown";
		}

		UC_ASSERT_ALWAYS_MSG("Invalid value");
		return helper << "error";
	}

	LogHelper& operator<<(LogHelper& helper, const WAFunction& func)
	{
		const auto handle = func.handle();

		// RET /////////////////////////////////////////////////////////////////////
		if (const auto ret_count = m3_GetRetCount(handle); ret_count == 0)
		{
			helper << "void";
		}
		else if (ret_count == 1)
		{
			helper << m3_GetRetType(handle, 0);
		}
		else
		{
			helper << "[";
			for (unsigned i = 0; i < ret_count; i++)
			{
				if (i > 0) helper << ",";
				helper << m3_GetRetType(handle, i);
			}
			helper << "]";
		}

		// NAME ////////////////////////////////////////////////////////////////////

		helper << " " << func.name() << "(";

		// ARG /////////////////////////////////////////////////////////////////////
		if (const auto arg_count = m3_GetArgCount(handle); arg_count == 0)
		{
		}
		else if (arg_count == 1)
		{
			helper << m3_GetRetType(handle, 0);
		}
		else
		{
			for (unsigned i = 0; i < arg_count; i++)
			{
				if (i > 0) helper << ",";
				helper << m3_GetArgType(handle, i);
			}
		}
		helper << ")";

		return helper;
	}
}