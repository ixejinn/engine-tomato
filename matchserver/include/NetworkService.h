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
#include <tomato/net/NetBitWriter.h>

struct Packet;
struct MatchRequest;

class SessionManager;
class MatchManager;

class NetworkService
{
public:
	NetworkService(SessionManager& sessionMgr, MatchManager& matchMgr, tomato::SPSCQueue<MatchRequestCommand, 128>& requestQ, tomato::SPSCQueue<SendCommandPtr, 256>& netSendRequestQ)
		:
		sessionMgr_(sessionMgr),
		matchMgr_(matchMgr),
		MatchRequestQueue(requestQ),
		NetSendRequestQueue(netSendRequestQ)
	{};

	/////////////ONLY FOR TEST////////////
	void AddNetMassage(PacketPtr& packets);
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
	void ProcessPacket(const TCPPacketType& header, tomato::NetBitReader& reader, SessionId& client);
	void HandlePacketRequest(const TCPPacketType& header, tomato::NetBitReader& reader, SessionId& client);
	void HandlePacketTimeSync(const TCPPacketType& header, tomato::NetBitReader& reader, SessionId& client);
	void ProcessDataFromClient(const SessionId& sessionId, const uint8_t* data, const int len);
	void TCPRecvThreadLoop();
private:
	tomato::WinsockContext winsock_;
	tomato::NetDriver driver_;
	tomato::TCPNetDriver tcpDriver_;

	SessionManager& sessionMgr_;
	MatchManager& matchMgr_;

	tomato::SPSCQueue<PacketPtr, 256> TCPRecvQueue;
	tomato::SPSCQueue<Packet, 256> NetRecvQueue;
	tomato::SPSCQueue<MatchRequestCommand, 128>& MatchRequestQueue;
	tomato::SPSCQueue<SendCommandPtr, 256>& NetSendRequestQueue;

	MatchId testMatchId = 0;
};

#endif // !NETWORK_SERVICE_H