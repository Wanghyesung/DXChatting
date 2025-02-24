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
	
	//clientStart���� Session�� �ۿ��� ����� �����ؼ� connected�� �ϴ��� �ƴϸ� Service���� �� Ŭ�� ������ �������� static���� ����(lib����)����ϵ簡
}

wstring StringToWString(const string& str)
{
    size_t llSize = str.size() + 1; //�ι���
    std::wstring wstr(llSize, 0);   //ũ�� �̸� �Ҵ�

    size_t llNumCvt = 0;
    mbstowcs_s(&llNumCvt, &wstr[0], llSize, str.c_str(), llSize - 1);

    wstr.pop_back(); // �� ���� ����
    return wstr;
}

string WstringToString(const wstring& _str)
{
	int iSize = WideCharToMultiByte(CP_UTF8, 0, _str.c_str(), -1, NULL, 0, NULL, NULL);
	string str(iSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, _str.c_str(), -1, &str[0], iSize, NULL, NULL);
	return str;
}
