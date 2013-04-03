#include "includes.hpp"
#include "server/commandset.hpp"


namespace srv {


CommandSet::CommandSet( const string& name )
{
	NOT_IMPLEMENTED();
}

CommandSet::~CommandSet()
{
	NOT_IMPLEMENTED();
}

void CommandSet::inherit( const shared_ptr<CommandSet>& cs )
{
	NOT_IMPLEMENTED();
}

TNID CommandSet::registerCmd( const string& name, const string& defKey )
{
	NOT_IMPLEMENTED();
}

TNID CommandSet::registerCmd( const string& name, const string& defKey, TNID id )
{
	NOT_IMPLEMENTED();
}



OVERRIDE void CommandSet::destroy()
{
	NOT_IMPLEMENTED();
}

OVERRIDE shared_ptr<TilenetObject> CommandSet::clone()
{
	NOT_IMPLEMENTED();
}

OVERRIDE void CommandSet::flush()
{
	NOT_IMPLEMENTED();
}


}