// pch.h: �̸� �����ϵ� ��� �����Դϴ�.
// �Ʒ� ������ ������ �� ���� �����ϵǾ�����, ���� ���忡 ���� ���� ������ ����մϴ�.
// �ڵ� ������ �� ���� �ڵ� �˻� ����� �����Ͽ� IntelliSense ���ɿ��� ������ ��Ĩ�ϴ�.
// �׷��� ���⿡ ������ ������ ���� �� ������Ʈ�Ǵ� ��� ��� �ٽ� �����ϵ˴ϴ�.
// ���⿡ ���� ������Ʈ�� ������ �߰����� ������. �׷��� ������ ���ϵ˴ϴ�.
#pragma once

#include <header.h>
#include<CEngine.h>
#include <Define.h>
#include <CDevice.h>

#ifndef PCH_H
#define PCH_H

#ifdef _DEBUG
#pragma comment(lib, "IOCP\\Debug\\IOCP.lib")
#pragma comment(lib, "Engine\\Debug\\Engine.lib")
#pragma comment(lib, "ProtoBuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "IOCP\\Release\\IOCP.lib")
#pragma comment(lib, "Engine\\Release\\Engine.lib")
#pragma comment(lib, "ProtoBuf\\Release\\libprotobuf.lib")

#endif
#endif
