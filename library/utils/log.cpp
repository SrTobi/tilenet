#include "includes.hpp"
#include "utils/log.hpp"



Log::LogStream::LogStream( Log& log, Category cat )
	: mLog(log)
	, mCategory(cat)
{
}

Log::LogStream::LogStream(LogStream&& other )
	: mLog(other.mLog)
	, mCategory(other.mCategory)
	// HACK: workaroung for gcc
	//, mStream(std::move(other.mStream))
	, mStream(other.mStream.str())
{
}

Log::LogStream::~LogStream()
{
	mLog.log(mCategory, mStream.str());
}

Log::Log()
	: mComponent(L"unknown")
{
}

Log::Log( const string& component )
	: mComponent(component)
{
}

Log::~Log()
{
}


Log::LogStream Log::log(Category cat)
{
	return LogStream(*this, cat);
}

void Log::log( Category cat, const string& msg )
{
	static std::mutex Mutex;
	std::lock_guard<std::mutex> lock(Mutex);

	std::wcout << L'[' << std::setw(10) << std::setfill(L' ') << mComponent << L']';

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
