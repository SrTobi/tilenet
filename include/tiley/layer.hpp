#pragma once
#ifndef _TILEY_LAYER_HPP
#define _TILEY_LAYER_HPP


#include <string>

#include "auto_object.hpp"
#include "flushable.hpp"
#include "lockable.hpp"
#include "tn_object.hpp"


namespace tiley {

template<typename Ch, typename Mutex>
class Layer
	: public AutoObject
	, public virtual Flushable
	, public Lockable
	, public TNObject
{
public:
	typedef std::basic_string<Ch> string;

public:
	Layer()
	{
	}

	virtual ~Layer()
	{
	}

private:
};




}



#endif