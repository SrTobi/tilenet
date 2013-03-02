#pragma once
#ifndef _NET_SERVICE_H
#define _NET_SERVICE_H


#include "settings.h"
#include "participant.h"
#include "utils/idmap.h"
#include "network/obuffer.h"

namespace server {
	class Layer;
	class Display;
	class TilenetServer;
}

namespace service {


class NetService
{
public:
	NetService(const TilenetConfig& config, server::TilenetServer* server);
	virtual ~NetService();


	void start();

	inline unsigned short port() const { return mServerPort; }
	inline const TilenetConfig& config() const { return mConfig; }

	uint					participantCount() const;
	shared_ptr<Participant> participant(uint p);
	shared_ptr<Participant> createParticipant(const socket_ptr& socket);
	shared_ptr<Participant>	eraseParticipant(uint p);

	void start_LayerPropertiesTransmittion_broadcast(const shared_ptr<server::Layer>& layer);
	void start_LayerTransmittion_broadcast(const shared_ptr<server::Layer>& layer, ptime::duration wait = ptime::millisec(20));
	void start_FullLayerTransmittion_broadcast(const shared_ptr<server::Layer>& layer, ptime::duration wait = ptime::seconds(0));
	void start_fullLayerTransmittion_to_participant(const shared_ptr<server::Layer>& layer, const shared_ptr<Participant>& partitipant, ptime::duration wait = ptime::seconds(0));
	void fullDisplayTransmittion(const shared_ptr<server::Display>& display);
	void fullDisplayTransmittion_to_partitipant(const shared_ptr<Participant>& participant);

	server::TilenetServer*	server() const { return mServer; }

private:

	// Async starters
	void startAsyncAccept();

	// Async handlers
	void handleAsyncAccept(socket_ptr socket, const error_code& err);

	void handleLayerPropertieTransmittion(const shared_ptr<server::Layer>& layer, const shared_ptr<asio::deadline_timer>& timer);
	void handleLayerTransmittion(const shared_ptr<server::Layer>& layer, const shared_ptr<asio::deadline_timer>& timer);
	void handleFullLayerTransmittion(const shared_ptr<server::Layer>& layer, const shared_ptr<asio::deadline_timer>& timer);
	void handleFullLayerTransmittion_to_participant(const shared_ptr<server::Layer>& layer, const shared_ptr<Participant>& partitipant, const shared_ptr<asio::deadline_timer>& timer);

	void buildFullLayerTransmittionPackage(const shared_ptr<server::Layer>& layer, net::OBuffer& pack);

	void sendToDisplay(server::Display* dispaly, const net::OBuffer& pack);

	bool init();


	void run();

	// IO Service
	asio::io_service		mIOService;
	boost::thread			mServiceThread;

	// Network
	unsigned short			mServerPort;
	asio::tcp::acceptor		mAcceptor;

	// Config
	const TilenetConfig&	mConfig;
	server::TilenetServer*	mServer;
	bool					mRunning;

	// Clients
	Syncronized<utils::IdMap< shared_ptr<Participant> > >	mParticipants;

	std::vector<Point>						mPointBuffer;
	std::vector<Point>*						mPointSwapBuffer;
};

}
#endif
