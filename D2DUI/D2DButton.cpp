#include "include.h"
#include "D2DButton.h"
#include "RenderEngine.h"

CD2DButton::CD2DButton():
	m_pBitmapNormal(nullptr),
	m_pBitmapHover(nullptr),
	m_pBitmapPush(nullptr)
{
}

CD2DButton::~CD2DButton()
{
	SafeRelease(&m_pBitmapNormal);
	SafeRelease(&m_pBitmapHover);
	SafeRelease(&m_pBitmapPush);
}

BOOL CD2DButton::LoadBtnImage(LPCWSTR normal, LPCWSTR hot, LPCWSTR push, int w, int h)
{	
	SafeRelease(&m_pBitmapNormal);
	SafeRelease(&m_pBitmapHover);
	SafeRelease(&m_pBitmapPush);

	ID2D1DCRenderTarget* rRT = CRenderEngine::GetInstance().GetRT();
	IWICImagingFactory* pIWIC = CRenderEngine::GetInstance().GetWICFactory();

	HRESULT hr;
	hr = CRenderEngine::GetInstance().LoadBitmapFromFile(rRT, pIWIC, normal, w, h, &m_pBitmapNormal);
	if (!SUCCEEDED(hr)) {
		return FALSE;
	}

	if (hot != L"") {
		hr = CRenderEngine::GetInstance().LoadBitmapFromFile(rRT, pIWIC, hot, w, h, &m_pBitmapHover);
		if (!SUCCEEDED(hr)) {
			return FALSE;
		}
	}

	if (push != L"") {
		hr = CRenderEngine::GetInstance().LoadBitmapFromFile(rRT, pIWIC, push, w, h, &m_pBitmapPush);
		if (!SUCCEEDED(hr)) {
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CD2DButton::DoDraw()
{
	if (!__super::DoDraw()) {
		return FALSE;
	}

	STATE  st = GetState();
	RECT rc = GetRect();
	ID2D1DCRenderTarget* rRT = CRenderEngine::GetInstance().GetRT();

	if (m_pBitmapNormal != NULL && 
		m_pBitmapHover == NULL && 
		m_pBitmapPush == NULL) {
		rRT->DrawBitmap(
			m_pBitmapNormal,
			D2D1::RectF(rc.left, rc.top,
				rc.right, rc.bottom));
		return TRUE;
	}

	if (st == MOUSE_IN && m_pBitmapHover != NULL) {
		rRT->DrawBitmap(
			m_pBitmapHover,
			D2D1::RectF(rc.left, rc.top,
				rc.right, rc.bottom));
	}
	else if (st == MOUSE_LEAVE && m_pBitmapNormal != NULL) {
		rRT->DrawBitmap(
			m_pBitmapNormal,
			D2D1::RectF(rc.left, rc.top,
				rc.right, rc.bottom));
	}
	else if (st == MOUSE_DOWN && m_pBitmapPush != NULL) {
		rRT->DrawBitmap(
			m_pBitmapPush,
			D2D1::RectF(rc.left, rc.top,
				rc.right, rc.bottom));
	}

	return TRUE;
}

void CD2DButton::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	__super::MsgProc(hwnd, msg, wParam, lParam);

	STATE  st = GetState();
	RECT rc = GetRect();
	switch (msg) {
	case WM_LBUTTONDOWN: {
		if (st == MOUSE_IN) {
			SetState(MOUSE_DOWN);
			SetRedraw(TRUE);
		}
	}break;
	case WM_LBUTTONUP: {
		if (st == MOUSE_DOWN) {
			SetState(MOUSE_IN);
			SetRedraw(TRUE);
			::PostMessage(hwnd, WM_BTNCLICK, GetID(), 0);
		}
	}break;

	case WM_MOUSELEAVE: {
		SetState(CControlUI::STATE::MOUSE_LEAVE);
		SetRedraw(TRUE);
	}break;

	case WM_MOUSEMOVE: {
		POINTS pts = MAKEPOINTS(lParam);
		POINT pt = { pts.x, pts.y };
		if (PtInRect(&rc, pt) && st == MOUSE_DOWN) {
			// 鼠标在按钮内部保持按下状态不处理
		}
		else if (PtInRect(&rc, pt) && st != MOUSE_IN) {
			SetState(MOUSE_IN);
			SetRedraw(TRUE);
		}
		else if (!PtInRect(&rc, pt) && st != MOUSE_LEAVE) {
			SetState(MOUSE_LEAVE);
			SetRedraw(TRUE);
		}
	}
	break;
	}
}