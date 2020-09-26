#include "Debug.h"
#include <chrono>
#include <cstdarg>
#include <iostream>


MessageType Debug::m_currentSev = MessageType::TYPE_NONE;
std::string Debug::m_outputName = "";

void Debug::DebugInit()
{
	m_outputName = "OdeumEngineLog.txt";
	std::ofstream out;
	out.open(m_outputName.c_str(), std::ios::out);
	out.close();
	m_currentSev = MessageType::TYPE_FATAL_ERROR;
}

void Debug::SetSeverity(MessageType type_)
{
	m_currentSev = type_;
}

void Debug::Info(const std::string & message_, const std::string & fileName_, const int line_)
{
	Log(MessageType::TYPE_INFO, "[INFO]: " + message_, fileName_, line_);
}

void Debug::Trace(const std::string & message_, const std::string & fileName_, const int line_)
{
	Log(MessageType::TYPE_TRACE, "[TRACE]: " + message_, fileName_, line_);
}

void Debug::Warning(const std::string & message_, const std::string & fileName_, const int line_)
{
	Log(MessageType::TYPE_WARNING, "[WARNING]: " + message_, fileName_, line_);
}

void Debug::Error(const std::string & message_, const std::string & fileName_, const int line_)
{
	Log(MessageType::TYPE_ERROR, "[ERROR]: " + message_, fileName_, line_);
}

void Debug::FatalError(const std::string & message_, const std::string & fileName_, const int line_)
{
	Log(MessageType::TYPE_FATAL_ERROR, "[FATAL_ERROR]: " + message_, fileName_, line_);
}

void Debug::AssertionError(int pNum, ...)
{
	va_list va_args;
	va_start(va_args, pNum);
	std::string msg = std::string(va_arg(va_args, char*));
	std::string file = std::string(va_arg(va_args, char*));
	int line = va_arg(va_args, int);
	Log(MessageType::TYPE_ASSERTION_ERROR, msg, file, line);

	va_end(va_args);
}

void Debug::Log(const MessageType type_, const std::string & message_, const std::string & fileName_, const int line_)
{
	if (type_ <= m_currentSev && m_currentSev > MessageType::TYPE_NONE)
	{
		std::ofstream out;
		out.open(m_outputName.c_str(), std::ios::app | std::ios::out);

		auto timenow =
			std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		char mtime[100];
		ctime_s(mtime, sizeof(mtime), &timenow);

		std::string output = message_ + " in: " + fileName_ + " on line: " + std::to_string(line_);
		out << output << " at: " << mtime;
		std::cout << output << std::endl;
		out.flush();
		out.close();
	}
}