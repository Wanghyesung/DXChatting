#include "pch.h"
#include "IOCP.h"
#include "Session.h"
#include "IOCPEvent.h"
#include "GThread.h"

IOCP::IOCP()
{
	//icop 생성
	m_IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
}

IOCP::~IOCP()
{

}

void IOCP::RegisterEvent(shared_ptr<Session> _pSession)
{
	//기존 IOCP 핸들(m_IOCPHandle)에 새로운 핸들(handle)을 연결
	HANDLE handle = _pSession->GetHandle();
	CreateIoCompletionPort(handle, m_IOCPHandle, 0, 0 );

}

void IOCP::RegisterDummyPacket()
{
	PostQueuedCompletionStatus(m_IOCPHandle, 0, 0, nullptr);
}

void IOCP::Excute()
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;
	IOCPEvent* iocpEvnet = nullptr;
	
	
	//지정된 I/O 완료 포트에서 I/O 완료 패킷을 큐에서 제거하려고 시도합니다. 없다면 생길때까지 대기 완료 패킷"을 받을 때까지 블록(blocking) 상태
	if (GetQueuedCompletionStatus(m_IOCPHandle, &numOfBytes, &key, reinterpret_cast<LPOVERLAPPED*>(&iocpEvnet), INFINITE) != false)
	{
		if (iocpEvnet == nullptr)
			return;
			

		shared_ptr<Session> pSession = iocpEvnet->GetOwner();
		pSession->DisPatch(iocpEvnet, numOfBytes);
	}
	else
	{
		int error = GetLastError();
		if(error == WAIT_TIMEOUT)
			return;
		else
		{
			shared_ptr<Session> pSession = iocpEvnet->GetOwner();
			pSession->DisPatch(iocpEvnet, numOfBytes);
		}
	}
}
