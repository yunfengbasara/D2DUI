#pragma once
#include "WndBase.h"
#include "D2DButton.h"
/*******************************************
	¿ØÖÆ´°¿Ú
*******************************************/

class CControlWnd : public CWndBase
{
public:
	CControlWnd();
	virtual ~CControlWnd();

	virtual HWND	CreateWnd(int, int, HWND);
	virtual void	DoDraw();
	virtual void	Redraw();
	virtual void	MsgProc(HWND, UINT, WPARAM, LPARAM);

private:
	CD2DButton		m_nRecordBtn;
	BOOL			m_bRecording;
};

