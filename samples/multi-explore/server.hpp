#pragma once
#ifndef _MULTI_EXPLORE_SERVER_HPP
#define _MULTI_EXPLORE_SERVER_HPP

#include <memory>
#include <tiley/tiley.hpp>

#include "map.hpp"

class Participant
{
public:
	Participant(Map& map, const std::shared_ptr<tiley::Participant>& p);
	~Participant();

	bool move(const tiley::Point& dir);
	void someone_moved(const tiley::Point& from, const tiley::Point& to);
	void draw();

	const tiley::Point& position() const;
private:
	TNCOLOR mColor;
	Map& mMap;
	std::shared_ptr<tiley::Participant> mHandle;
	tiley::Point mPosition;

	tiley::TileLayer<> mLayer;
};



class MEServer
{
public:
	MEServer(unsigned short port);
	~MEServer();

	void run();

private:
	void handle_events(TNEVENT& evt);

private:
	std::unique_ptr<tiley::Server<>> mServer;
	std::unique_ptr<tiley::ListenAcceptor> mListenAcceptor;

	std::vector<Participant*> mParticipants;

	Map mMap;
};




#endif
