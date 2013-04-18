#pragma once
#ifndef _SENDABLE_HPP
#define _SENDABLE_HPP


#include "settings.hpp"

namespace net {

class Message;

class Sendable
{
public:
	virtual shared_ptr<Message> getStateMessage() = 0;
};




}


#endif