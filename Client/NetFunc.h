#pragma once

void ClientLogin();
void SenPkt();
void Exit();

void InitChattingBar();
void InitLoginBar();

wstring StringToWString(const string& str);
string WstringToString(const wstring& _str);