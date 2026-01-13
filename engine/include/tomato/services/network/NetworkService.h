#ifndef TOMATO_NETWORKSERVICE_H
#define TOMATO_NETWORKSERVICE_H

#include <map>
#include <unordered_map>
#include <string>
#include <atomic>

#include "tomato/services/network/CoreNetwork.h"
#include "tomato/services/network/Socket.h"         // SocketPtr socket_
#include "tomato/services/network/SocketAddress.h"  // map<..SocketAddress..> ..
#include "tomato/containers/MemoryPool.h"
#include "tomato/containers/SPSCQueue.h"

namespace tomato
{
    class Engine;

    struct Packet
    {
        RawBuffer* buffer{nullptr};
        std::size_t size;
        SocketAddress addr;

        Packet() = default;
        Packet(RawBuffer* bufPtr, std::size_t size, SocketAddress addr)
        : buffer(bufPtr), size(size), addr(addr) {}
    };

	class NetworkService
	{
	public:
		explicit NetworkService(Engine& engine);
        ~NetworkService();

        // !!! FOR TEST !!!
		void ReadIncomingData();
		void SendOutgoingData(const SocketAddress& inToAddress);
        // !!! FOR TEST !!!

		bool InitSocket();

        void Dispatch();
        //void NetThreadLoop();
        void ProcessPendingPacket();
        void SendPacket(uint32_t messageType);
        //void SendMessage(uint32_t messageType, const NetConnection& conn);

        uint32_t GetPlayerID() const { return playerID_; }
        uint32_t GetPlayerID(SocketAddress& addr)
        { return socketToPlayer[addr]; }

        std::atomic<bool> isNetThreadRunning_{false};

	private:
        //NetDriver driver_;
        MemoryPool<RawBuffer, 128> bufferPool_;
        SPSCQueue<Packet, 128> pendingPackets_;

        //std::unordered_map<uint32_t, NetConnection> conn;
		std::map<uint32_t, std::string> playerToName;
		std::map<uint32_t, SocketAddress> playerToSocket;
		std::unordered_map<SocketAddress, uint32_t> socketToPlayer;

		SocketPtr socket_;
		std::string name_;
		uint32_t playerID_;

        Engine& engine_;
	};
}

#endif // !TOMATO_NETWORKSERVICE_H