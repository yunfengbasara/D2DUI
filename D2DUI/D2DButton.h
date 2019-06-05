#pragma once
#include "ControlUI.h"
/*******************************************
	°´Å¥
*******************************************/

class CD2DButton : public CControlUI
{
public:
	CD2DButton();
	virtual ~CD2DButton();

	BOOL LoadBtnImage(LPCWSTR, LPCWSTR hot=L"", LPCWSTR push=L"", int w = 0, int h = 0);

	virtual BOOL	DoDraw();
	virtual void	MsgProc(HWND, UINT, WPARAM, LPARAM);
private:
	ID2D1Bitmap* m_pBitmapNormal;
	ID2D1Bitmap* m_pBitmapHover;
	ID2D1Bitmap* m_pBitmapPush;
};

