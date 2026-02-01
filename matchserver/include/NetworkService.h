#ifndef NETWORK_SERVICE_H
#define NETWORK_SERVICE_H

#include "MatchTypes.h"
#include "PacketTypes.h"
#include "SessionManager.h"
#include <tomato/services/network/WinsockContext.h>
#include <tomato/services/network/NetDriver.h>
#include <tomato/services/network/TCPNetDriver.h>
#include <tomato/containers/SPSCQueue.h>
#include <tomato/net/NetBitReader.h>

struct Packet;
struct MatchRequest;

class SessionManager;
class MatchManager;

class NetworkService
{
public:
	NetworkService(SessionManager& sessionMgr, MatchManager& matchMgr, tomato::SPSCQueue<MatchRequestCommand, 128>& requestQ, tomato::SPSCQueue<SendRequestCommand, 256>& netSendRequestQ)
		:
		sessionMgr_(sessionMgr),
		matchMgr_(matchMgr),
		MatchRequestQueue(requestQ),
		NetSendRequestQueue(netSendRequestQ)
	{};

	/////////////ONLY FOR TEST////////////
	void AddNetMassage(TCPPacket& packets);
	//////////////////////////////////////


	void Update(float dt);

	void ProcessPendingPacket();
	void ProcessPacketMatchReq(tomato::NetBitReader& reader, SessionId sessionId);
	void OnPacket();

	void NetRecvThreadLoop();

	//TCP
	void ProcessNetSendRequest();
	void ProcessSendPacket();

	void ProcessQueuedPackets();
	void ProcessPacket(const TCPHeader& header, tomato::NetBitReader& reader, tomato::TCPSocketPtr& client);
	void ProcessPacketRequest(tomato::NetBitReader& reader, tomato::TCPSocketPtr& client);
	void ProcessDataFromClient(const tomato::TCPSocketPtr socket, const uint8_t* data, const int len);
	void TCPRecvThreadLoop();
private:
	tomato::WinsockContext winsock_;
	tomato::NetDriver driver_;
	tomato::TCPNetDriver tcpDriver_;

	SessionManager& sessionMgr_;
	MatchManager& matchMgr_;

	tomato::SPSCQueue<TCPPacket, 256> TCPRecvQueue;
	tomato::SPSCQueue<Packet, 256> NetRecvQueue;
	tomato::SPSCQueue<MatchRequestCommand, 128>& MatchRequestQueue;
	tomato::SPSCQueue<SendRequestCommand, 256>& NetSendRequestQueue;

	SessionId testId = 0;
};

#endif // !NETWORK_SERVICE_H