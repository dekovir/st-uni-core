#pragma once
#include "unicore/Logger.hpp"
#include "unicore/StreamWriter.hpp"

namespace unicore
{
	class StreamLogger : public Logger
	{
	public:
		explicit StreamLogger(const Shared<WriteStream>& stream)
		: _stream(stream), _writer(*stream.get()) {}

		void write(LogType type, const StringView text) override;

	protected:
		Shared<WriteStream> _stream;
		StreamWriter _writer;
	};
}
