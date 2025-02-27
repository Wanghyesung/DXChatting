#pragma once
#include "Enter.pb.h"
#include "Exit.pb.h"
#include "Chatting.pb.h"
#include "NetFunc.h"
#include "PacketSession.h"


enum PACKET_TYPE
{
	S_ENTER = 1000,
	C_ENTER = 1001,
	S_NEW_ENTER = 1002,

	S_CHATTING = 1003,
	C_CHATTING = 1004,
	S_NEW_CHATTING = 1005,

	S_EXIT = 1006,
	C_EXIT = 1007,
	S_NEW_EXIT = 1008,
};

class PacketSession;
class Session;

using PacketHandlerFunc = std::function<bool(shared_ptr<PacketSession>&, BYTE*, INT)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

template <typename T>
shared_ptr<SendBuffer> _MakeSendBuffer(T& _pkt, UINT _ID);



bool Handle_S_ENTER(shared_ptr<Session> _pSession, Protocol::S_ENTER& _pkt);
bool Handle_S_NEW_ENTER(shared_ptr<Session> _pSession, Protocol::S_NEW_ENTER& _pkt);
bool Handle_S_CHATTING(shared_ptr<Session> _pSession, Protocol::S_CHATTING& _pkt);
bool Handle_S_NEW_CHATTING(shared_ptr<Session> _pSession, Protocol::S_NEW_CHATTING& _pkt);
bool Handle_S_EXIT(shared_ptr<Session> _pSession, Protocol::S_EXIT& _pkt);
bool Handle_S_NEW_EXIT(shared_ptr<Session> _pSession, Protocol::S_NEW_EXIT& _pkt);


class CServerPacketHandler
{
public:
	static bool HandlePacket(shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
	{
		PacketHeader* pHeader = reinterpret_cast<PacketHeader*>(_pBuffer);
		return GPacketHandler[pHeader->id](_pSession, _pBuffer, _iLen);//해당 아이디에 맞는 함수 호출
	}


	static void Init()
	{
		for (int i = 0; i < UINT16_MAX; ++i)
			GPacketHandler[i] = nullptr;

		GPacketHandler[S_ENTER] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::S_ENTER>(Handle_S_ENTER, _pSession, _pBuffer, _iLen); };
		GPacketHandler[S_NEW_ENTER] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::S_NEW_ENTER>(Handle_S_NEW_ENTER, _pSession, _pBuffer, _iLen); };
		GPacketHandler[S_CHATTING] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::S_CHATTING>(Handle_S_CHATTING, _pSession, _pBuffer, _iLen); };
		GPacketHandler[S_NEW_CHATTING] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::S_NEW_CHATTING>(Handle_S_NEW_CHATTING, _pSession, _pBuffer, _iLen); };
		GPacketHandler[S_EXIT] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::S_EXIT>(Handle_S_EXIT, _pSession, _pBuffer, _iLen); };
		GPacketHandler[S_NEW_EXIT] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::S_NEW_EXIT>(Handle_S_NEW_EXIT, _pSession, _pBuffer, _iLen); };
	}

	template <typename T, typename Func>
	static bool HandlePacket(Func _pFunc, shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
	{
		T pkt;
		if (pkt.ParseFromArray(_pBuffer + sizeof(PacketHeader), _iLen - sizeof(PacketHeader)) == false)
			return false;

		return _pFunc(_pSession, pkt);
	}
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_ENTER _pkt) { return _MakeSendBuffer(_pkt, C_ENTER); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_CHATTING _pkt) { return _MakeSendBuffer(_pkt, C_CHATTING); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_EXIT _pkt) { return _MakeSendBuffer(_pkt, C_EXIT); }
	
};

template<typename T>
inline shared_ptr<SendBuffer> _MakeSendBuffer(T& _pkt, UINT _ID)
{
	const UINT16 iDataSize = static_cast<UINT16>(_pkt.ByteSizeLong());
	const UINT16 iPacketSize = iDataSize + sizeof(PacketHeader);

	shared_ptr<SendBuffer> pSendBuffer = SendBufferMgr->Open(iPacketSize);
	PacketHeader* pHeader = reinterpret_cast<PacketHeader*>(pSendBuffer->GetData());
	pHeader->id = _ID;
	pHeader->size = iPacketSize;	

	_pkt.SerializeToArray(&pHeader[1], iDataSize);
	pSendBuffer->Close(iPacketSize);

	return pSendBuffer;

}
