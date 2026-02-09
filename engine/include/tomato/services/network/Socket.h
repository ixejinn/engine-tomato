#ifndef TOMATO_SOCKET_H
#define TOMATO_SOCKET_H

#include "tomato/services/network/SocketUtil.h"
#include <WinSock2.h>
#include <memory>

namespace tomato
{
	class SocketAddress;
	class Socket;
	typedef std::shared_ptr<Socket> SocketPtr;

	/**
	 * Windows WinSock API를 감싸는 UDP Socket 래퍼
	 * 자원 해제 안정성을 위해 RAII 형태로 설계
	 * non-blocking mode를 기본 전제
	 */
	class Socket
	{
	public:
		/**
		 * UDP 소켓 생성
		 * 실패 시 nullptr 반환
		 */
		static SocketPtr CreateSocket();

		// 소켓 핸들 정리 (closesocket)
		~Socket();

		int Bind(const SocketAddress& inBindAddress);

		/**
		 * 특정 주소로 데이터 전송
		 * non-blocking 모드 기준이므로, EWOULDBLOCK 발생 가능
		 */
		int SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress);
		int ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress);

		/**
		 * 소켓을 non-blocking / blocking 모드로 전환
		 * 프레임 정지 방지를 위해 non-blocking 모드 사용 권장
		 */
		int SetNonBlockingMode(bool nonBlocking);

	private:
		friend class SocketUtil;
		Socket(SOCKET inSocket) : socket_(inSocket) {};
		SOCKET socket_;
	};

}

#endif // !TOMATO_SOCKET_H