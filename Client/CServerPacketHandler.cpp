#include "pch.h"
#include "CServerPacketHandler.h"
#include "CRoomMgr.h"
#include "CSpeechBar.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_S_ENTER(shared_ptr<Session> _pSession, Protocol::S_ENTER& _pkt)
{
    if (_pkt.success() == false)
        return false;

    CRoomMgr::GetInst()->ClientLogin();

  
    int iUserSize = _pkt.users_size();
    for (int i = 0; i < iUserSize; ++i)
        CRoomMgr::GetInst()->AddProPile(StringToWString(_pkt.users(i)));

    return true;
}

bool Handle_S_NEW_ENTER(shared_ptr<Session> _pSession, Protocol::S_NEW_ENTER& _pkt)
{
    CRoomMgr::GetInst()->AddProPile(StringToWString(_pkt.user()));

    return true;
}

bool Handle_S_CHATTING(shared_ptr<Session> _pSession, Protocol::S_CHATTING& _pkt)
{
    if (_pkt.success() == false)
        return false;

    //�� ��ǳ�� �׸���
    if(GChattingBar != nullptr)
       GChattingBar->ShowText();

    return true;
}

bool Handle_S_NEW_CHATTING(shared_ptr<Session> _pSession, Protocol::S_NEW_CHATTING& _pkt)
{   
    string strOtherText = _pkt.text();

    //�ٸ� �ֵ� ��ǳ�� �׸���
    if (GChattingBar != nullptr)
        GChattingBar->ShowOtherText(StringToWString(strOtherText));

    return true;
}

bool Handle_S_EXIT(shared_ptr<Session> _pSession, Protocol::S_EXIT& _pkt)
{
    //�̰� �ʿ�X ������ Ŭ�󿡼� ������ Ȯ���� �ʿ䰡 ����
    return true;
}

bool Handle_S_NEW_EXIT(shared_ptr<Session> _pSession, Protocol::S_NEW_EXIT& _pkt)
{
    CRoomMgr::GetInst()->EraseProPile(StringToWString(_pkt.name()));
    return false;
}
