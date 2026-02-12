#ifndef TOMATO_NETWORKSERVICE_H
#define TOMATO_NETWORKSERVICE_H

#include <map>
#include <unordered_map>
#include <string>
#include <atomic>

#include "tomato/tomato_packet_types.h"
#include "tomato/services/network/WinsockContext.h"
#include "tomato/services/network/Socket.h"         // SocketPtr socket_
#include "tomato/services/network/TCPSocket.h"
#include "tomato/services/network/NetDriver.h"
#include "tomato/services/network/NetConnection.h"
#include "tomato/containers/MemoryPool.h"
#include "tomato/containers/SPSCQueue.h"

namespace tomato
{
    class Engine;

    

	class NetworkService
	{
	public:
		explicit NetworkService(Engine& engine);
        ~NetworkService();

        // !!! FOR TEST !!!
		void ReadIncomingData();
		void SendOutgoingData(const SocketAddress& inToAddress);
        // !!! FOR TEST !!!
        
        void ConnectToServer();

        //void Dispatch();
        void NetThreadLoop();
        void ProcessPendingPacket();
        //void SendPacket(uint32_t messageType);
        void BuildUDPPacket(NetBitWriter& writer, UDPPacketType messageType);
        void BroadcastToPeers(const void* buffer);
        void SendUDPPacket(UDPPacketType messageType, SendPolicy policy, const SocketAddress* inToAddress = nullptr);
        
        void TCPNetRecvThreadLoop();
        void ProcessQueuedTCPPacket();
        void ProcessTCPPacket(const TCPPacketType& header, NetBitReader& reader);
        void SendTCPPacket(TCPPacketType messageType);

        void HandleMatchIntroPacket(NetBitReader& reader);

        PlayerId GetPlayerID() const { return playerID_; }
        PlayerId GetPlayerID(const SocketAddress& addr) { return addToId[addr]; }

        std::atomic<bool> isNetThreadRunning_{false};

	private:
        WinsockContext winsock_;
        NetDriver driver_;
        TCPSocketPtr server_;

        std::vector<uint8_t> recvBuffer;
        SPSCQueue<std::unique_ptr<TCPPacket>, 128> pendingTCPPackets_;

        MemoryPool<RawBuffer, 128> bufferPool_;
        SPSCQueue<Packet, 128> pendingPackets_;

        std::unordered_map<PlayerId, NetConnection> conn;
        std::unordered_map<SocketAddress, PlayerId> addToId;

		//std::map<uint32_t, std::string> playerToName;
		//std::map<uint32_t, SocketAddress> playerToSocket;
		//std::unordered_map<SocketAddress, uint32_t> socketToPlayer;

		//SocketPtr socket_;
        std::string name_ = "testing";
        PlayerId playerID_{ 0 };
        MatchId matchID_{ 0 };

        Engine& engine_;
	};
}

#endif // !TOMATO_NETWORKSERVICE_H