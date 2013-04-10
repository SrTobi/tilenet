#pragma once
#ifndef _LOG_HPP
#define _LOG_HPP

#include <boost/noncopyable.hpp>
#include "settings.hpp"


class Log
	: public boost::noncopyable
{
public:
	enum Category
	{
		Debug,
		Info,
		Warning,
		Error
	};
public:
	class LogStream
	{
	public:
		LogStream(Log& log, Category cat);
		~LogStream();

		template<typename T>
		LogStream& operator << (const T& v)
		{
			mStream << v;
			return *this;
		}

	private:
		std::wostringstream mStream;
		Log& mLog;
		Category mCategory;
	};

public:
	Log();
	Log(const string& component);
	~Log();

	LogStream&& debug() { return log(Debug); }
	LogStream&& info()	{ return log(Info); }
	LogStream&& warn()	{ return log(Warning); }
	LogStream&& error() { return log(Error); }


	LogStream&& log(Category cat);
	void log(Category cat, const string& msg);
private:
	const string mComponent;
};






#endif