#include "include.h"
#include "WndManage.h"
#include "WndBase.h"
#include "CaptureWnd.h"
#include "ControlWnd.h"

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
LPCTSTR g_sD2DUIWNDName = TEXT("D2DUIWnd");

CWndManage::CWndManage()
{
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX), CS_CLASSDC, WindowProcedure, 0L, 0L,
		GetModuleHandle(NULL), LoadIcon(NULL, IDI_APPLICATION), 
		LoadCursor(NULL, IDC_ARROW), NULL, NULL,
		g_sD2DUIWNDName, LoadIcon(NULL, IDI_APPLICATION)
	};

	RegisterClassEx(&wc);
}

CWndManage::~CWndManage()
{
	for (auto &it : m_nWndCenter) {
		delete it.second;
	}
}

HWND CWndManage::CreateWnd()
{
	CWndBase* pCapWnd = new CCaptureWnd();
	HWND hCapWnd = pCapWnd->CreateWnd(1200, 600, NULL);
	m_nWndCenter.insert(std::make_pair(hCapWnd, pCapWnd));

	CWndBase* pConWnd = new CControlWnd();
	HWND hConWnd = pConWnd->CreateWnd(150, 52, hCapWnd);
	m_nWndCenter.insert(std::make_pair(hConWnd, pConWnd));

	return hCapWnd;
}

int CWndManage::MsgLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void CWndManage::MsgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto pWndit = m_nWndCenter.find(hwnd);
	if (pWndit != m_nWndCenter.end()) {
		pWndit->second->MsgProc(hwnd, message, wParam, lParam);
	}
}

void CWndManage::DoPaint(HWND hwnd)
{
	auto pWndit = m_nWndCenter.find(hwnd);
	if (pWndit != m_nWndCenter.end()) {
		pWndit->second->DoPaint();
	}
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CWndManage::GetInstance().MsgProc(hwnd, message, wParam, lParam);

	switch (message){
		case WM_DESTROY:
			PostQuitMessage(0);     
		break;
		case WM_SETCURSOR:
		break;
		case WM_PAINT: {
			PAINTSTRUCT ps = { 0 };
			::BeginPaint(hwnd, &ps);
			CWndManage::GetInstance().DoPaint(hwnd);
			::EndPaint(hwnd, &ps);
		}
		break;
		default:                    
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}