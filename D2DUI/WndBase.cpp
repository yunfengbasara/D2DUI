#include "include.h"
#include "WndBase.h"
#include "WndManage.h"
#include "RenderEngine.h"
using namespace DirectX;

CWndBase::CWndBase():
	m_hWnd(nullptr),
	m_nEdgeSize(50),
	m_nMouseType(SIZEALL),
	m_nScaleType(SIZEALL),
	m_bMouseLeave(FALSE),
	m_nGapNW({0,0}),
	m_nGapSE({0,0})
{
	
}

CWndBase::~CWndBase()
{
}

HWND CWndBase::CreateWnd(int w, int h, HWND hParent)
{
	m_hWnd = CreateWindowEx(WS_EX_LAYERED, g_sD2DUIWNDName,
		NULL, WS_POPUPWINDOW,
		0, 0, 0, 0, hParent, NULL, GetModuleHandle(NULL), NULL
	);

	::ShowWindow(m_hWnd, SW_SHOW);

	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	::SetWindowPos(m_hWnd, NULL, (width - w) / 2, (height - h) / 2, w, h, SWP_NOZORDER);

	::InvalidateRect(m_hWnd, NULL, FALSE);

	return m_hWnd;
}

HWND CWndBase::GetWnd()
{
	return m_hWnd;
}

SIZE CWndBase::WndSize()
{
	RECT rc = { 0 };
	::GetWindowRect(m_hWnd, &rc);
	::OffsetRect(&rc, -rc.left, -rc.top);
	return { rc.right, rc.bottom };
}

RECT CWndBase::WndRect()
{
	RECT rc = { 0 };
	::GetWindowRect(m_hWnd, &rc);
	::OffsetRect(&rc, -rc.left, -rc.top);
	return rc;
}

void CWndBase::SetMouse(CWndBase::MOUSETYPE type)
{
	m_nMouseType = type;
}

void CWndBase::DoPaint()
{
	SIZE size = WndSize();
	HDC hdc = ::GetDC(m_hWnd);
	HDC hMemDC = ::CreateCompatibleDC(hdc);
	HBITMAP hMemBmp = ::CreateCompatibleBitmap(hdc, size.cx, size.cy);
	::SelectObject(hMemDC, hMemBmp);

	RECT rc = WndRect();
	ID2D1DCRenderTarget* rt = CRenderEngine::GetInstance().GetRT();
	rt->BindDC(hMemDC, &rc);
	rt->BeginDraw();

	DoDraw();

	rt->EndDraw();

	POINT ptSrc = { 0,0 };
	BLENDFUNCTION blendFunction = { AC_SRC_OVER , 0,
		255, AC_SRC_ALPHA };

	UpdateLayeredWindow(m_hWnd, NULL, NULL, &size, hMemDC,
		&ptSrc, 0, &blendFunction, ULW_ALPHA);

	::DeleteDC(hMemDC);
	::DeleteObject(hMemBmp);
}

void CWndBase::DoDraw()
{

}

void CWndBase::Redraw()
{

}

void CWndBase::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_LBUTTONDOWN: {
			// 拖动窗口
			if (m_nMouseType == SIZEALL) {
				SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
			}
			
			SIZE size = WndSize();
			POINTS pts = MAKEPOINTS(lParam);
			POINT pt = { pts.x, pts.y };
			m_nScaleType = _GetMouseType(size, pt);

			m_nGapNW = { pt.x - 0, pt.y - 0 };
			m_nGapSE = { pt.x - size.cx, pt.y - size.cy };

			// 鼠标离开窗口依然要相应窗口拖动
			SetCapture(hwnd);
		}break;

		case WM_LBUTTONUP: {
			m_nScaleType = SIZEALL;

			ReleaseCapture();
		}break;

		case WM_MOUSELEAVE: {
			m_nScaleType == SIZEALL;
			m_bMouseLeave = FALSE;
			m_nGapNW = { 0,0 };
			m_nGapSE = { 0,0 };
		}break;

		case WM_ENTERSIZEMOVE: {
		}break;
		case WM_EXITSIZEMOVE: {
		}break;

		case WM_MOUSEMOVE: {
			if (!m_bMouseLeave) {
				m_bMouseLeave = TRUE;
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hwnd;
				::TrackMouseEvent(&tme);
			}

			SIZE size = WndSize();
			POINTS pts = MAKEPOINTS(lParam);
			POINT pt = { pts.x, pts.y };

			m_nMouseType = _GetMouseType(size, pt);
				
			_Resize(m_nScaleType, pt);
		}break;

		case WM_SETCURSOR: {
			HCURSOR hc = (HCURSOR)wParam;

			switch (m_nMouseType) {
				case SIZEALL:hc = LoadCursor(NULL, IDC_SIZEALL);break;
				case W: case E:hc = LoadCursor(NULL, IDC_SIZEWE); break;
				case N: case S:hc = LoadCursor(NULL, IDC_SIZENS); break;
				case NW: case SE:hc = LoadCursor(NULL, IDC_SIZENWSE); break;
				case NE: case SW:hc = LoadCursor(NULL, IDC_SIZENESW); break;
				case HAND:hc = LoadCursor(NULL, IDC_HAND); break;
			}

			SetCursor(hc);
		}break;
	}
}

CWndBase::MOUSETYPE CWndBase::_GetMouseType(SIZE& s, POINT& pt)
{
	MOUSETYPE type = SIZEALL;

	if (_MouseNW(s, pt)) {
		type = NW;
	}else if (_MouseSE(s, pt)) {
		type = SE;
	}else if (_MouseNE(s, pt)) {
		type = NE;
	}else if (_MouseSW(s, pt)) {
		type = SW;
	}else if (_MouseW(s, pt)) {
		type = W;
	}else if (_MouseE(s, pt)) {
		type = E;
	}else if (_MouseN(s, pt)) {
		type = N;
	}else if (_MouseS(s, pt)) {
		type = S;
	}
	return type;
}

BOOL CWndBase::_MouseW(SIZE& s, POINT& pt)
{
	if (pt.x >= 0 && pt.x <= 0 + m_nEdgeSize) {
		return TRUE;
	}
	return FALSE;
}

BOOL CWndBase::_MouseE(SIZE& s, POINT& pt)
{
	if (pt.x <= s.cx && pt.x >= s.cx - m_nEdgeSize) {
		return TRUE;
	}
	return FALSE;
}

BOOL CWndBase::_MouseN(SIZE& s, POINT& pt)
{
	if (pt.y >= 0 && pt.y <= 0 + m_nEdgeSize) {
		return TRUE;
	}
	return FALSE;
}

BOOL CWndBase::_MouseS(SIZE& s, POINT& pt)
{
	if (pt.y <= s.cy && pt.y >= s.cy - m_nEdgeSize) {
		return TRUE;
	}
	return FALSE;
}

BOOL CWndBase::_MouseNW(SIZE& s, POINT& pt)
{
	if (pt.x >= 0 && pt.x <= 0 + m_nEdgeSize &&
		pt.y >= 0 && pt.y <= 0 + m_nEdgeSize) {
		return TRUE;
	}
	return FALSE;
}

BOOL CWndBase::_MouseSE(SIZE& s, POINT& pt)
{
	if (pt.x <= s.cx && pt.x >= s.cx - m_nEdgeSize &&
		pt.y <= s.cy && pt.y >= s.cy - m_nEdgeSize) {
		return TRUE;
	}
	return FALSE;
}

BOOL CWndBase::_MouseNE(SIZE& s, POINT& pt)
{
	if (pt.x <= s.cx && pt.x >= s.cx - m_nEdgeSize &&
		pt.y >= 0 && pt.y <= 0 + m_nEdgeSize) {
		return TRUE;
	}
	return FALSE;
}

BOOL CWndBase::_MouseSW(SIZE& s, POINT& pt)
{
	if (pt.x >= 0 && pt.x <= 0 + m_nEdgeSize &&
		pt.y <= s.cy && pt.y >= s.cy - m_nEdgeSize) {
		return TRUE;
	}
	return FALSE;
}

void CWndBase::_Resize(MOUSETYPE& type, POINT& pt)
{
	RECT rc = { 0 };
	::GetWindowRect(m_hWnd, &rc);
	SIZE size = { rc.right - rc.left, rc.bottom - rc.top };

	if (m_nGapNW.cx == 0 && m_nGapNW.cy == 0 &&
		m_nGapSE.cx == 0 && m_nGapSE.cy == 0) {
		return;
	}

	int dNWX = pt.x - m_nGapNW.cx;
	int dNWY = pt.y - m_nGapNW.cy;
	int dSEX = pt.x - m_nGapSE.cx;
	int dSEY = pt.y - m_nGapSE.cy;
	switch (m_nScaleType) {
	case W: size.cx -= dNWX; rc.left += dNWX; break;
	case E: size.cx = dSEX; break;
	case N: size.cy -= dNWY; rc.top += dNWY; break;
	case S: size.cy = dSEY; break;
	case NW: size.cy -= dNWY; rc.top += dNWY;
		size.cx -= dNWX; rc.left += dNWX; break;
	case NE: size.cy -= dNWY; rc.top += dNWY;
		size.cx = dSEX; break;
	case SW: size.cx -= dNWX; rc.left += dNWX;
		size.cy = dSEY; break;
	case SE: size.cy = dSEY; size.cx = dSEX; break;
	}

	if (m_nScaleType != SIZEALL &&
		((dNWX != 0 || dNWY != 0) || (dSEX != size.cx || dSEY != size.cy))) {
		Redraw();
		::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, size.cx, size.cy, SWP_NOZORDER);
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
}