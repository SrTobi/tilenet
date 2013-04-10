#include "includes.hpp"
#include "utils/log.hpp"



Log::LogStream::LogStream( Log& log, Category cat )
	: mLog(log)
	, mCategory(cat)
{
}

Log::LogStream::~LogStream()
{
	mLog.log(mCategory, mStream.str());
}

Log::Log()
{
}

Log::Log( const string& component )
	: mComponent(component)
{
}

Log::~Log()
{
}

void Log::log( Category cat, const string& msg )
{
	static std::mutex Mutex;
	std::lock_guard<std::mutex> lock(Mutex);

	switch (cat)
	{
	case Log::Debug:
		std::wcout << L"Debug: ";
		break;
	case Log::Info:
		std::wcout << L"Info:  ";
		break;
	case Log::Warning:
		std::wcout << L"Warn:  ";
		break;
	case Log::Error:
		std::wcout << L"Error: ";
		break;
	default:
		std::wcout << L"---:   ";
		break;
	}

	std::wcout << msg << std::endl;
}
