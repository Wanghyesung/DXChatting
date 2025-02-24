#include "pch.h"
#include "NetFunc.h"
#include "Enter.pb.h"
#include "CRoomMgr.h"
#include "CServerPacketHandler.h"

void ClientLogin()
{
	Protocol::C_ENTER pkt;
	const wstring& strClientName = CRoomMgr::GetInst()->GetClientName();

	pkt.set_name(WstringToString(strClientName));
	shared_ptr<SendBuffer> pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	
	//clientStart에서 Session을 밖에서 만들고 접근해서 connected를 하던가 아니면 Service에서 내 클라 세션을 가져오게 static으로 만들어서(lib수정)사용하든가
}

wstring StringToWString(const string& str)
{
    size_t llSize = str.size() + 1; //널문자
    std::wstring wstr(llSize, 0);   //크기 미리 할당

    size_t llNumCvt = 0;
    mbstowcs_s(&llNumCvt, &wstr[0], llSize, str.c_str(), llSize - 1);

    wstr.pop_back(); // 널 문자 제거
    return wstr;
}

string WstringToString(const wstring& _str)
{
	int iSize = WideCharToMultiByte(CP_UTF8, 0, _str.c_str(), -1, NULL, 0, NULL, NULL);
	string str(iSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, _str.c_str(), -1, &str[0], iSize, NULL, NULL);
	return str;
}
