#pragma once
#include "PacketSession.h"

class CServerSession : public PacketSession
{
public:
	CServerSession();
	virtual ~CServerSession();

    virtual void OnConnected();
    virtual int OnRecvPacket(BYTE* buffer, int len);
    virtual void OnSend(int len);
    virtual void OnDisConnected();


private:
   
};

