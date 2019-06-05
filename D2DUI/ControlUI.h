#pragma once

/*******************************************
	¿Ø¼þ»ùÀà
*******************************************/

class CControlUI
{
public:
	CControlUI();
	virtual ~CControlUI();

	typedef enum {
		MOUSE_IN,
		MOUSE_LEAVE,
		MOUSE_DOWN,
	}STATE;

	void	SetID(UINT);
	UINT	GetID();

	void	SetRect(RECT);
	RECT	GetRect();

	void	SetState(STATE);
	STATE   GetState();

	void	SetRedraw(BOOL);
	BOOL	GetRedraw();
	
	virtual BOOL	DoDraw();
	virtual void	MsgProc(HWND, UINT, WPARAM, LPARAM);

private:
	RECT	m_nRect;
	STATE	m_nState;
	BOOL	m_bNeedRedraw;
	UINT	m_nID;
};

