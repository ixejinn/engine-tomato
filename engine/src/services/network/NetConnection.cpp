#include "tomato/services/network/NetConnection.h"
#include "tomato/net/NetBitReader.h"
#include "tomato/net/NetBitWriter.h"

namespace tomato
{
	void NetConnectionPacket::Write(NetBitWriter& writer)
	{
		writer.WriteInt(static_cast<uint16_t>(0), std::numeric_limits<uint16_t>::max()); //packet len
		writer.WriteInt(static_cast<uint16_t>(TCPPacketType::MATCH_INTRO), static_cast<uint16_t>(TCPPacketType::COUNT));

		for (int i = 0; i < EngineConfig::MAX_PLAYER_NUM; i++)
		{
			writer.WriteInt(static_cast<uint16_t>(netconn[i].matchId), std::numeric_limits<uint16_t>::max());
			writer.WriteInt(static_cast<uint16_t>(netconn[i].name.size()), std::numeric_limits<uint16_t>::max());
			writer.WriteInt(static_cast<uint8_t>(netconn[i].playerId), std::numeric_limits<uint8_t>::max());
			for (int j = 0; j < netconn[i].name.size(); j++)
				writer.WriteInt(static_cast<uint8_t>(netconn[i].name[j]), std::numeric_limits<uint8_t>::max());
			writer.WriteInt(static_cast<uint32_t>(netconn[i].addr.GetIPv4()), std::numeric_limits<uint32_t>::max());
		}
		//uint16_t size = writer.GetByteSize();
		//memcpy(writer, &size, sizeof(uint16_t));
	}
	
	void NetConnectionPacket::Read(NetBitReader& reader)
	{
		uint16_t readMatchId{ 0 }, readPlayerId{ 0 }, readNameSize{ 0 };
		uint8_t rname[50];
		std::string readName{};
		uint32_t addr{ 0 };
		
		for (int i = 0; i < EngineConfig::MAX_PLAYER_NUM; i++)
		{
			//reader.ReadInt(readSize, std::numeric_limits<uint16_t>::max());
			//reader.ReadInt(readType, static_cast<uint16_t>(TCPPacketType::COUNT));
			reader.ReadInt(readMatchId, std::numeric_limits<uint16_t>::max());
			reader.ReadInt(readPlayerId, std::numeric_limits<uint8_t>::max());
			reader.ReadInt(readNameSize, std::numeric_limits<uint16_t>::max());
			for (int j = 0; j < readNameSize; j++)
			{
				reader.ReadInt(rname[j], std::numeric_limits<uint8_t>::max());
				netconn[readPlayerId].name.push_back(rname[j]);
			}
			reader.ReadInt(addr, std::numeric_limits<uint32_t>::max());
			SocketAddress readAddr(addr, 7777);

			netconn[readPlayerId].matchId = readMatchId;
			netconn[readPlayerId].playerId = readPlayerId;
			netconn[readPlayerId].addr = readAddr;
		}
	}
}