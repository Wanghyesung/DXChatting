#include "pch.h"
#include "CServerPacketHandler.h"
#include "CRoomMgr.h"


bool Handle_S_ENTER(shared_ptr<Session> _pSession, Protocol::S_ENTER& _pkt)
{
    if (_pkt.success() == false)
        return false;

    CRoomMgr::GetInst()->ClientLogin();

    auto& names = _pkt.users();
    for (int i = 0; i < names.size(); ++i)
        CRoomMgr::GetInst()->AddProPile(StringToWString(names[i]));

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

    //��ǳ�� �׸���

    return false;
}

bool Handle_S_NEW_CHATTING(shared_ptr<Session> _pSession, Protocol::S_NEW_CHATTING& _pkt)
{
    //�ٸ� �ֵ� ��ǳ�� �׸���
    return false;
}

bool Handle_S_EXIT(shared_ptr<Session> _pSession, Protocol::S_EXIT& _pkt)
{
    //�̰� �ʿ������ ������ Ŭ�󿡼� ������ Ȯ���� �ʿ䰡 ����
    return false;
}
