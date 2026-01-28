#ifndef TOMATO_SOCKET_UTIL_H
#define TOMATO_SOCKET_UTIL_H
#include <WinSock2.h>
namespace tomato
{
	class SocketUtil
	{
	public:
		/**
		 * WinSock 초기화
		 * 프로그램 시작 시 단 한 번만 호출되어야 하며,
		 * CreateSocket 이전에 반드시 선행
		 */
		static bool InitWinsock();

		/**
		 * Winsock 종료 처리
		 * 프로그램 종료 시 호출
		 */
		static void CleanUp();

	private:
	};
}

#endif // !TOMATO_SOCKET_UTIL_H
