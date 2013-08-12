#pragma once
#ifndef _TILEY_SERVER_HPP
#define _TILEY_SERVER_HPP


#include <string>
#include <chrono>
#include <functional>
#include <memory>
#include "config.hpp"
#include "tn_object.hpp"
#include "participant.hpp"

namespace tiley {


template<typename Ch = TILEY_DEFAULT_CHAR>
class ServerConfig
{
public:
	typedef std::basic_string<Ch> string;
public:
	ServerConfig(const string& name)
		: mName(name)
	{
	}

	const string& name() const
	{
		return mName;
	}

	void name(const string& name)
	{
		mName = name;
	}


	const string& description() const
	{
		return mTilesetName;
	}

	void description(const string& desc)
	{
		mDescription = desc;
	}
	

	const string& tilesetName() const
	{
		return mTilesetName;
	}

	void tilesetName(const string& name)
	{
		mTilesetName = name;
	}

	const string& interface() const
	{
		return mTilesetInterface;
	}

	void interface(const string& iface)
	{
		mTilesetInterface = iface;
	}



private:
	string mName;
	string mDescription;
	string mTilesetName;
	string mTilesetInterface;
};


template<typename Ch = TILEY_DEFAULT_CHAR, typename Mutex = TILEY_DEFAULT_MUTEX>
class Server
	: public Lockable
	, public TNObject
{
public:
	typedef std::basic_string<Ch> string;
	typedef Mutex mutex_type;
public:
	Server(const ServerConfig& cfg)
		: mConfig(cfg)
	{
		mEventIterator = mEventBuffer.begin();
		reset(Impl::CreateServer(cfg.name(), cfg.description(), cfg.tilesetName(), cfg.interface()));
	}

	bool fetch(TNEVENT& evt)
	{
		if(mEventIterator == mEventBuffer.end())
		{
			mEventBuffer = Impl::FetchEvents(native_ref(), nullptr);

			mEventIterator = mEventBuffer.begin();
			if(mEventBuffer.empty())
				return false;
		}

		internal_fetch(evt);
		return true;
	}

	template<typename Rep, typename Period>
	bool fetch(TNEVENT& evt, std::chrono::duration<Rep,Period>& timeout_duration)
	{
		using std::chrono::duration_cast;
		typedef std::chrono::milliseconds target_time_format;
		typedef std::chrono::duration<Rep,Period> src_time_format;

		if(mEventIterator == mEventBuffer.end())
		{
			size_t timeout = duration_cast<target_time_format>(timeout_duration).count();
			mEventBuffer = Impl::FetchEvents(native_ref(), &timeout);

			timeout_duration = target_time_format(timeout);

			if(mEventBuffer.empty())
				return false;
		}

		internal_fetch(evt);

		return true;
	}

	template<typename Rep, typename Period>
	bool fetch(TNEVENT& evt, const std::chrono::duration<Rep,Period>& timeout_duration)
	{
		auto timeout_dummy = timeout_duration;
		return fetch(evt, timeout_dummy);
	}

	const ServerConfig& config() const
	{
		return mConfig;
	}

	std::shared_ptr<Participant> participant(TNPARTICIPANT p)
	{
		std::shared_ptr<Participant> result;
		unsigned int index = TNEXTRACTID(p);

		if(index < mParticipants.size())
		{
			result = mParticipants[index];
		}

		if(!index)
			throw TileyException("Unknown participant requested!");

		return result;
	}

private:
	void internal_fetch(TNEVENT& evt)
	{
		assert(mEventIterator != mEventBuffer.end());
		evt = *mEventIterator;
		++mEventIterator;

		switch(evt.type)
		{
		case TNEV_CONNECT:
			handle_connect(evt);
			break;
		case  TNEV_DISCONNECT:
			handle_disconnect(evt);
			break;
		default:
			break;
		}
	}


	void handle_connect(const TNEVENT& evt)
	{
		TNPARTICIPANT p = evt.participant;
		TNPARTICIPANT index = TNEXTRACTID(p);
		assert(evt.type == TNEV_CONNECT);

		auto newP = std::make_shared<Participant>(p);

		if(index < mParticipants.size())
		{
			assert(!mParticipants[index]);

			mParticipants[index] = newP;
		}else
		{
			assert(index == mParticipants.size());
			mParticipants.push_back(newP);
		}
	}

	void handle_disconnect(const TNEVENT& evt)
	{
		TNPARTICIPANT p = evt.participant;
		TNPARTICIPANT index = TNEXTRACTID(p);
		assert(evt.type == TNEV_DISCONNECT);
		assert(index < mParticipants.size());
		assert(mParticipants[index]);

		mParticipants[index] = nullptr;
	}

private:
	const ServerConfig mConfig;
	bool mStarted;

	std::vector<TNEVENT> mEventBuffer;
	std::vector<TNEVENT>::iterator mEventIterator;

	std::vector<std::shared_ptr<Participant>> mParticipants;

	mutex_type mMutex;
};



}

#endif