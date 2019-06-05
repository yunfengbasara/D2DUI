#pragma once
/*******************************************
	采用DirectUI和Direct2D的窗口基类
*******************************************/

class CWndBase
{
public:
	CWndBase();
	virtual ~CWndBase();
	typedef enum {
		W,
		E,
		N,
		S,
		NW,
		SE,
		NE,
		SW,
		SIZEALL,
		HAND,
	}MOUSETYPE;

	HWND	GetWnd();
	SIZE	WndSize();
	RECT	WndRect();
	void	SetMouse(MOUSETYPE);
	void	DoPaint();

	virtual HWND	CreateWnd(int, int, HWND);
	virtual void	DoDraw();
	virtual void	Redraw();
	virtual void	MsgProc(HWND, UINT, WPARAM, LPARAM);

private:
	MOUSETYPE _GetMouseType(SIZE&, POINT&);
	BOOL	_MouseW(SIZE&, POINT&);
	BOOL	_MouseE(SIZE&, POINT&);
	BOOL	_MouseN(SIZE&, POINT&);
	BOOL	_MouseS(SIZE&, POINT&);
	BOOL	_MouseNW(SIZE&, POINT&);
	BOOL	_MouseSE(SIZE&, POINT&);
	BOOL	_MouseNE(SIZE&, POINT&);
	BOOL	_MouseSW(SIZE&, POINT&);
	void	_Resize(MOUSETYPE&, POINT&);

private:
	HWND	m_hWnd;

	UINT	m_nEdgeSize;
	MOUSETYPE m_nMouseType;
	MOUSETYPE m_nScaleType;	// 用于缩放窗口
	BOOL	m_bMouseLeave;  // 用于鼠标离开消息

	SIZE	m_nGapNW;		// 鼠标距左上角距离
	SIZE	m_nGapSE;		// 鼠标距右下角距离
};