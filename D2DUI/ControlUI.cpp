#include "include.h"
#include "ControlUI.h"
#include "RenderEngine.h"

CControlUI::CControlUI():
	m_nState(MOUSE_LEAVE),
	m_bNeedRedraw(TRUE),
	m_nID(0)
{
}

CControlUI::~CControlUI()
{

}

void CControlUI::SetID(UINT id)
{
	m_nID = id;
}

UINT CControlUI::GetID()
{
	return m_nID;
}

void CControlUI::SetRect(RECT rc)
{
	m_nRect = rc;
}

RECT CControlUI::GetRect()
{
	return m_nRect;
}

void CControlUI::SetState(STATE st)
{
	m_nState = st;
}

CControlUI::STATE CControlUI::GetState()
{
	return m_nState;
}
BOOL CControlUI::GetRedraw()
{
	return m_bNeedRedraw;
}

void CControlUI::SetRedraw(BOOL bredraw)
{
	m_bNeedRedraw = bredraw;
}

BOOL CControlUI::DoDraw()
{
	if (!m_bNeedRedraw) {
		return FALSE;
	}

	m_bNeedRedraw = FALSE;
	return TRUE;
}

void CControlUI::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
}