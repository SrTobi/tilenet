#pragma once
#ifndef _COMMANDSET_HPP
#define _COMMANDSET_HPP

#include "settings.hpp"


namespace srv {


class CommandSet
	:	public TilenetObject
	,	public IdObject<CommandSet>
{
public:
	CommandSet(const string& name);
	~CommandSet();

	void inherit(const shared_ptr<CommandSet>& cs);

	TNID registerCmd(const string& name, const string& defKey);
	TNID registerCmd(const string& name, const string& defKey, TNID id);

	virtual OVERRIDE void destroy();
	virtual OVERRIDE shared_ptr<TilenetObject> clone();
	virtual OVERRIDE void flush();

private:
};



}


#endif