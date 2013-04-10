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
		LogStream(LogStream&& other);
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

	inline LogStream debug() { return log(Debug); }
	inline LogStream info()	{ return log(Info); }
	inline LogStream warn()	{ return log(Warning); }
	inline LogStream error() { return log(Error); }


	LogStream log(Category cat);
	void log(Category cat, const string& msg);
private:
	const string mComponent;
};






#endif