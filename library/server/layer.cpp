#include "includes.hpp"
#include "server/layer.hpp"


namespace srv {


Layer::Layer()
	: mCommit(0)
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
	return ++mCommit;
}



}