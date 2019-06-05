#pragma once

/*******************************************
	‰÷»æ
*******************************************/

class CRenderEngine
{
public:
	static CRenderEngine& GetInstance() {
		static CRenderEngine instance;
		return instance;
	}

	CRenderEngine();
	~CRenderEngine();

	ID2D1Factory*		GetFactory();
	IDWriteFactory*		GetWriteFactory();
	IWICImagingFactory* GetWICFactory();
	ID2D1DCRenderTarget* GetRT();

	HRESULT LoadBitmapFromFile(ID2D1RenderTarget*, IWICImagingFactory*, LPCWSTR, UINT, UINT, ID2D1Bitmap**);
	HRESULT SaveBitmapToFile(ID2D1Factory*, IWICImagingFactory*, ID2D1Bitmap*, LPCWSTR);

private:
	HRESULT _InitD2D();
	HRESULT _InitDevice();
	HRESULT _InitResource();

private:
	ID2D1Factory* m_pD2DFactory;
	IDWriteFactory* m_pDWriteFactory;
	IWICImagingFactory*	 m_pWICFactory;
	ID2D1DCRenderTarget* m_pD2DRT;
};

