#include "include.h"
#include "RenderEngine.h"

CRenderEngine::CRenderEngine():
	m_pD2DFactory(nullptr),
	m_pDWriteFactory(nullptr),
	m_pWICFactory(nullptr),
	m_pD2DRT(nullptr)
{
	_InitD2D();
	_InitDevice();
	_InitResource();
}

CRenderEngine::~CRenderEngine()
{
	SafeRelease(&m_pD2DRT);
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pD2DFactory);
}

ID2D1Factory* CRenderEngine::GetFactory()
{
	return m_pD2DFactory;
}

IDWriteFactory*	CRenderEngine::GetWriteFactory()
{
	return m_pDWriteFactory;
}

IWICImagingFactory* CRenderEngine::GetWICFactory()
{
	return m_pWICFactory;
}

ID2D1DCRenderTarget* CRenderEngine::GetRT()
{
	return m_pD2DRT;
}

HRESULT CRenderEngine::LoadBitmapFromFile(ID2D1RenderTarget *pRenderTarget,
	IWICImagingFactory *pIWICFactory,LPCWSTR uri,UINT width,UINT height,ID2D1Bitmap **ppBitmap)
{
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	if (!SUCCEEDED(hr)) {
		return hr;
	}

	hr = pDecoder->GetFrame(0, &pSource);
	if (!SUCCEEDED(hr)) {
		return hr;
	}

	hr = pIWICFactory->CreateFormatConverter(&pConverter);
	if (!SUCCEEDED(hr)) {
		return hr;
	}

	UINT originalWidth, originalHeight;
	hr = pSource->GetSize(&originalWidth, &originalHeight);
	if (!SUCCEEDED(hr)) {
		return hr;
	}

	if (width == 0) {
		width = originalWidth;
	}

	if (height == 0) {
		height = originalHeight;
	}

	hr = pIWICFactory->CreateBitmapScaler(&pScaler);
	if (!SUCCEEDED(hr)) {
		return hr;
	}

	hr = pScaler->Initialize(
		pSource,
		width,
		height,
		WICBitmapInterpolationModeFant
	);
	if (!SUCCEEDED(hr)) {
		return hr;
	}

	hr = pConverter->Initialize(
		pScaler,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut
	);
	if (!SUCCEEDED(hr)) {
		return hr;
	}

	hr = pRenderTarget->CreateBitmapFromWicBitmap(
		pConverter,
		NULL,
		ppBitmap
	);
	if (!SUCCEEDED(hr)) {
		return hr;
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}

HRESULT CRenderEngine::SaveBitmapToFile(ID2D1Factory* pD2DFactory,
	IWICImagingFactory* pWICFactory, ID2D1Bitmap* bitmap, LPCWSTR filename)
{
	HRESULT hr = S_FALSE;
	IWICStream*	pStream = NULL;
	IWICBitmapEncoder* pEncoder = NULL;
	IWICBitmapFrameEncode* pFrameEncode = NULL;
	IWICBitmap*	pWicBitmap = NULL;
	ID2D1RenderTarget* pD2DRenderTarget = NULL;
	D2D1_SIZE_F sizeToSave = bitmap->GetSize();

	hr = pWICFactory->CreateBitmap(
		sizeToSave.width,
		sizeToSave.height,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapCacheOnLoad,
		&pWicBitmap);
	if (!SUCCEEDED(hr)) {
		return hr;
	}

	hr = pD2DFactory->CreateWicBitmapRenderTarget(
		pWicBitmap,
		&D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_SOFTWARE),
		&pD2DRenderTarget);
	if (!SUCCEEDED(hr)) {
		return hr;
	}

	pD2DRenderTarget->BeginDraw();
	pD2DRenderTarget->DrawBitmap(
			bitmap,
			D2D1::RectF(0, 0, sizeToSave.width, sizeToSave.height));
	pD2DRenderTarget->EndDraw();

	pWICFactory->CreateStream(&pStream);
	pStream->InitializeFromFilename(filename, GENERIC_WRITE);
	pWICFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);

	pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
	pEncoder->CreateNewFrame(&pFrameEncode, NULL);
	pFrameEncode->Initialize(NULL);
	pFrameEncode->SetSize(sizeToSave.width, sizeToSave.height);
	WICPixelFormatGUID format = GUID_WICPixelFormatDontCare;
	pFrameEncode->SetPixelFormat(&format);
	pFrameEncode->WriteSource(pWicBitmap, NULL);
	pFrameEncode->Commit();
	pEncoder->Commit();

	SafeRelease(&pStream);
	SafeRelease(&pEncoder);
	SafeRelease(&pFrameEncode);
	SafeRelease(&pWicBitmap);
	SafeRelease(&pD2DRenderTarget);

	return hr;
}

HRESULT CRenderEngine::_InitD2D()
{
	HRESULT hr = S_FALSE;

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory1), reinterpret_cast<void**>(&m_pD2DFactory));

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));

	CoCreateInstance(CLSID_WICImagingFactory2, NULL,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));

	D2D1_RENDER_TARGET_PROPERTIES props;
	props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED),
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_NONE,
		D2D1_FEATURE_LEVEL_DEFAULT
	);

	m_pD2DFactory->CreateDCRenderTarget(&props, &m_pD2DRT);
	return hr;
}

HRESULT CRenderEngine::_InitDevice()
{
	HRESULT hr = S_FALSE;

	return hr;

}
HRESULT CRenderEngine::_InitResource()
{
	HRESULT hr = S_FALSE;

	return hr;
}