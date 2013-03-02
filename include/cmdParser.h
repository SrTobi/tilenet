#pragma once

#include "Settings.h"


namespace tn{

class CommandLineParser;

class CommandLineOption
{
	friend class CommandLineParser;
public:

	CommandLineOption(const string& shortcut, const string& fullname, const string& description, const string& pattern);
	~CommandLineOption();
	
	void mustHave(const CommandLineOption* other);
	void mustNotHave(const CommandLineOption* other);

	void printDescription(std::basic_ostream<charc>& stream);
	void reset();
	
	
	inline bool has() const { return mHas; }
	inline const std::vector<string>& getParameter() const { return mParameter; }

private:
	void start();
	bool hasConflict();

	const string mShortCut;
	const string mFullname;
	const string mDescription;
	const string mPattern;

	bool mHas;

	std::vector<const CommandLineOption*> mMustHave;
	std::vector<const CommandLineOption*> mMustNotHave;
	std::vector<string>	mParameter;
};




class CommandLineParser
{
public:
	CommandLineParser(const string& name, const string& description, const string& pattern, const string& examples);
	~CommandLineParser();

	CommandLineParser& add(CommandLineOption* option);

	void parse(int argc, const char** argv);
	void parse(const char* argLine);
	void parse(int argc, const wchar_t** argv);
	void parse(const string& argLine);
	void parse(const std::vector<string>& line);

	void printDescription(std::basic_ostream<charc>& stream);
	bool isOk() const;
	
	const std::vector<string>& getParameter() const;

	inline bool hasArg() const { return mHasArg; }
private:

	CommandLineOption				mSelfOption;
	std::vector<CommandLineOption*>	mOptions;
	bool							mOk;
	bool							mHasArg;
	string							mExamples;
};

}
