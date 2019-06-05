#include "include.h"
#include "ControlWnd.h"
#include "RenderEngine.h"

CControlWnd::CControlWnd():
	m_bRecording(FALSE)
{
}

CControlWnd::~CControlWnd()
{

}

HWND CControlWnd::CreateWnd(int w, int h, HWND hParent)
{
	HWND hwnd = __super::CreateWnd(w, h, hParent);

	SIZE size = WndSize();

	m_nRecordBtn.LoadBtnImage(TEXT("res/start.png"),TEXT(""), TEXT(""));
	m_nRecordBtn.SetRect(RECT{0, 1, 50, 51 });
	m_nRecordBtn.SetID(20);

	return hwnd;
}

void CControlWnd::DoDraw()
{
	__super::DoDraw();

	ID2D1DCRenderTarget* rRT = CRenderEngine::GetInstance().GetRT();
	IDWriteFactory* pDW = CRenderEngine::GetInstance().GetWriteFactory();

	SIZE s = WndSize();
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
		D2D1::RectF(0, 0, s.cx, s.cy), 30.0f, 50.0f
	);

	ID2D1SolidColorBrush* pBrush = NULL;
	rRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 0.4f), &pBrush);
	rRT->DrawRoundedRectangle(roundedRect, pBrush);
	rRT->FillRoundedRectangle(roundedRect, pBrush);
	SafeRelease(&pBrush);

	m_nRecordBtn.DoDraw();

	RECT btnrc = m_nRecordBtn.GetRect();
	SIZE btnsz = { btnrc.right - btnrc.left, btnrc.bottom - btnrc.top };

	ID2D1SolidColorBrush* pTxtBrush = NULL;
	rRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &pTxtBrush);
	IDWriteTextFormat* pTxtFormat = NULL;
	pDW->CreateTextFormat(L"Î¢ÈíÑÅºÚ", NULL, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16, L"", &pTxtFormat);
	pTxtFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	
	if (m_nRecordBtn.GetState() == CControlUI::STATE::MOUSE_IN ||
		m_nRecordBtn.GetState() == CControlUI::STATE::MOUSE_DOWN) {
		LPCWSTR txt;
		if (!m_bRecording) {
			txt = L"¿ªÊ¼Â¼ÖÆ";
		}
		else {
			txt = L"Í£Ö¹Â¼ÖÆ";
		}
		IDWriteTextLayout* pTextLayout = NULL;
		pDW->CreateTextLayout(txt, lstrlen(txt), pTxtFormat, 
			s.cx - btnsz.cx, s.cy, &pTextLayout);
		DWRITE_TEXT_METRICS textMetrics;
		pTextLayout->GetMetrics(&textMetrics);
		rRT->DrawText(txt, lstrlen(txt), pTxtFormat,
			D2D1::RectF(btnrc.right + 4, (s.cy - textMetrics.height) / 2, 
				s.cx, (s.cy - textMetrics.height) / 2 + textMetrics.height),
			pTxtBrush
		);
		SafeRelease(&pTextLayout);
	}
	
	SafeRelease(&pTxtFormat);
	SafeRelease(&pTxtBrush);
}

void CControlWnd::Redraw()
{
	__super::Redraw();

	m_nRecordBtn.SetRedraw(TRUE);
}

void CControlWnd::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	__super::MsgProc(hwnd, msg, wParam, lParam);

	m_nRecordBtn.MsgProc(hwnd, msg, wParam, lParam);

	if (m_nRecordBtn.GetRedraw()) {
		::InvalidateRect(hwnd, NULL, FALSE);
	}

	switch (msg) {
	case WM_MOUSEMOVE: {
		if (m_nRecordBtn.GetState() == CControlUI::STATE::MOUSE_IN ||
			m_nRecordBtn.GetState() == CControlUI::STATE::MOUSE_DOWN) {
			SetMouse(HAND);
		}
	}break;

	case WM_BTNCLICK: {
		m_bRecording = !m_bRecording;
		if (!m_bRecording){
			m_nRecordBtn.LoadBtnImage(TEXT("res/start.png"), TEXT(""), TEXT(""));
		}
		else {
			m_nRecordBtn.LoadBtnImage(TEXT("res/stop.png"), TEXT(""), TEXT(""));

			SIZE s = WndSize();
			ID2D1DCRenderTarget* pRT = CRenderEngine::GetInstance().GetRT();
			ID2D1Factory* pFactory = CRenderEngine::GetInstance().GetFactory();
			IWICImagingFactory* pWICFactory = CRenderEngine::GetInstance().GetWICFactory();

			ID2D1HwndRenderTarget* pHwndRT;
			pFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_SOFTWARE),
				D2D1::HwndRenderTargetProperties(GetWnd(), D2D1::SizeU(s.cx, s.cy)),
				&pHwndRT
			);

			ID2D1BitmapRenderTarget* rMemRT;
			pHwndRT->CreateCompatibleRenderTarget(D2D1::SizeF(s.cx, s.cy), &rMemRT);

			D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
				D2D1::RectF(0, 0, s.cx, s.cy), 30.0f, 50.0f
			);

			rMemRT->BeginDraw();
			rMemRT->Clear();

			ID2D1SolidColorBrush* pBrush = NULL;
			rMemRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::PaleGreen, 0.2f), &pBrush);
			rMemRT->DrawRoundedRectangle(roundedRect, pBrush);
			rMemRT->FillRoundedRectangle(roundedRect, pBrush);
			SafeRelease(&pBrush);

			rMemRT->EndDraw();

			ID2D1Bitmap* pBitmapToSave = NULL;
			rMemRT->GetBitmap(&pBitmapToSave);

			CRenderEngine::GetInstance().SaveBitmapToFile(pFactory, pWICFactory, pBitmapToSave, L"out.png");

			SafeRelease(&pBitmapToSave);
			SafeRelease(&rMemRT);
			SafeRelease(&pHwndRT);
		}
	}break;
	}
}