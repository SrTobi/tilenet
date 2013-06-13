#include "includes.hpp"
#include "server/layer.hpp"


namespace srv {


Layer::Layer()
	: mCommit(1)
{

}

Layer::~Layer()
{

}

TNID Layer::currentCommitNr() const
{
	return mCommit;
}

TNID Layer::newCommit()
{
	return mCommit++;
}



}