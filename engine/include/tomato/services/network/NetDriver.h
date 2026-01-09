#ifndef TOMATO_NETDRIVER_H
#define TOMATO_NETDRIVER_H

#include "tomato/services/network/Socket.h"
#include "tomato/services/network/SocketAddress.h"

namespace tomato
{
	/**
	* OS 소켓 생성, 초기화, 종료 담당
	* UDP recv/send를 수행하고 수신 패킷을 
	* 로컬 환경 기준으로 작성
	*/
	class NetDriver
	{
	public:
		bool InitSocket();


	private:
		SocketPtr socket_;
	};

}

#endif // !TOMATO_NETFRINER_H