#include "include.h"
#include "CaptureWnd.h"
#include "RenderEngine.h"

CCaptureWnd::CCaptureWnd()
{
}

CCaptureWnd::~CCaptureWnd()
{
}

HWND CCaptureWnd::CreateWnd(int w, int h, HWND hParent)
{
	HWND hwnd = __super::CreateWnd(w, h, hParent);
	SIZE size = WndSize();

	m_nCloseBtn.LoadBtnImage(TEXT("res/close-normal.png"), 
		TEXT("res/close-hover.png"), TEXT("res/close-pushed.png"));
	m_nCloseBtn.SetRect(RECT{ size.cx - 60, 10, size.cx - 28, 42 });
	m_nCloseBtn.SetID(20);

	return hwnd;
}

void CCaptureWnd::DoDraw()
{
	__super::DoDraw();

	ID2D1DCRenderTarget* rRT = CRenderEngine::GetInstance().GetRT();

	rRT->Clear(D2D1::ColorF(D2D1::ColorF::White, 0.5f));

	SIZE size = WndSize();
	ID2D1SolidColorBrush* pBrush = nullptr;
	rRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &pBrush);
	rRT->DrawRectangle(D2D1::RectF(0.f, 0.f, size.cx, size.cy),pBrush);
	SafeRelease(&pBrush);

	m_nCloseBtn.DoDraw();
}

void CCaptureWnd::Redraw()
{
	__super::Redraw();

	SIZE size = WndSize();
	m_nCloseBtn.SetRect(RECT{ size.cx - 60, 10, size.cx - 28, 42 });

	m_nCloseBtn.SetRedraw(TRUE);
}

void CCaptureWnd::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	__super::MsgProc(hwnd, msg, wParam, lParam);

	m_nCloseBtn.MsgProc(hwnd, msg, wParam, lParam);

	if (m_nCloseBtn.GetRedraw()) {
		::InvalidateRect(hwnd, NULL, FALSE);
	}

	switch (msg) {
		case WM_MOUSEMOVE: {
			if (m_nCloseBtn.GetState() == CControlUI::STATE::MOUSE_IN ||
				m_nCloseBtn.GetState() == CControlUI::STATE::MOUSE_DOWN) {
				SetMouse(HAND);
			}
		}break;

		case WM_BTNCLICK: {
			::PostMessage(hwnd, WM_CLOSE, 0, 0);
		}break;

		case WM_STARTRECORD: {

		}break;

		case WM_STOPRECORD: {

		}break;
	}
}