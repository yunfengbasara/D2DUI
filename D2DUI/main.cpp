#include "include.h"
#include "WndManage.h"

int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nCmdShow)
{
	CoInitialize(NULL);
	CWndManage::GetInstance().CreateWnd();
	CWndManage::GetInstance().MsgLoop();
	CoUninitialize();
}