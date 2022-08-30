#pragma once
#include "unicore/Logger.hpp"
#include "unicore/Stream.hpp"

namespace unicore
{
	class StreamLogger : public Logger
	{
		UC_OBJECT(StreamLogger, Logger)
	public:
		explicit StreamLogger(const Shared<WriteStream>& stream);

		void write(LogType type, const StringView text) override;

	protected:
		Shared<WriteStream> _stream;
		StreamWriter _writer;
	};
}
