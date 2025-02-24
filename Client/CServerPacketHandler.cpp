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

    //말풍선 그리기

    return false;
}

bool Handle_S_NEW_CHATTING(shared_ptr<Session> _pSession, Protocol::S_NEW_CHATTING& _pkt)
{
    //다른 애들 말풍선 그리기
    return false;
}

bool Handle_S_EXIT(shared_ptr<Session> _pSession, Protocol::S_EXIT& _pkt)
{
    //이건 필요없을듯 어차피 클라에서 끊으면 확인할 필요가 없음
    return false;
}
