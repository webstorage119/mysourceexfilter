#pragma once

#define TEMPLATE_NAME	(L"My Source Ex Filter")
#define PROP_TEMPLATE_NAME	(L"My Source Ex Filter Prop")
#define PROP_NAME		(PROP_TEMPLATE_NAME)
#define FILTER_NAME		(TEMPLATE_NAME)
#define OUTPUT_PIN_NAME (L"Output")

// {8D531FB5-3934-4b44-8CAB-E26505D1B394}
DEFINE_GUID(CLSID_MySourceEx, 
			0x8d531fb5, 0x3934, 0x4b44, 0x8c, 0xab, 0xe2, 0x65, 0x5, 0xd1, 0xb3, 0x94);

// {3248D86E-4450-4f2b-AD43-5D8DDCFBABB6}
DEFINE_GUID(CLSID_MySourceExProp,
			0x3248d86e, 0x4450, 0x4f2b, 0xad, 0x43, 0x5d, 0x8d, 0xdc, 0xfb, 0xab, 0xb6);

// ピンタイプの定義
const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
	&MEDIATYPE_Video,
	&MEDIASUBTYPE_RGB32
};
// 入力用、出力用ピンの情報
const AMOVIESETUP_PIN sudPins =
{
	OUTPUT_PIN_NAME,
	FALSE,
	TRUE,
	FALSE,
	FALSE,
	&CLSID_NULL,
	NULL,
	1,
	&sudPinTypes
};

// フィルタ情報
const AMOVIESETUP_FILTER afFilterInfo=
{
	&CLSID_MySourceEx,
	FILTER_NAME,
	MERIT_DO_NOT_USE,
	1,
	&sudPins
};

// 色設定インターフェイス
__interface
	__declspec(uuid("{86A96761-8CA0-4d6a-94F7-F73DF2D5F0D6}"))
IColor : public IUnknown {
public:
	STDMETHODIMP SetColor(DWORD color);
	STDMETHODIMP GetColor(DWORD *pColor);
};

// ソースフィルタクラス
class CMySourceEx : CSource, public IColor, public ISpecifyPropertyPages {
public:
	DECLARE_IUNKNOWN
	CMySourceEx(LPUNKNOWN pUnk,HRESULT *phr);
	~CMySourceEx();
	static CUnknown * WINAPI	CreateInstance(LPUNKNOWN pUnk, HRESULT *phr);
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);
	//IColor
	STDMETHODIMP SetColor(DWORD color);
	STDMETHODIMP GetColor(DWORD *pColor);
	//ISpecifyPropertyPages
	STDMETHODIMP GetPages(CAUUID *pPages);
protected:
private:
	DWORD m_Color;
};

// プッシュピンクラス
class CPushPin : CSourceStream {
public:
	CPushPin(HRESULT *phr, CSource *pFilter);
	~CPushPin();
	STDMETHODIMP	Notify(IBaseFilter *pSelf, Quality q);
	// CSourceStreamの実装
	HRESULT			GetMediaType(CMediaType *pMediaType);
	HRESULT			CheckMediaType(const CMediaType *pMediaType);
	HRESULT			DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pRequest);
	HRESULT			FillBuffer(IMediaSample *pSample);
protected:
private:
	const REFERENCE_TIME	m_rtFrameLength;	//1フレームあたりの時間
};

// ソースフィルタのプロパティクラス
class CMySourceExProp : CBasePropertyPage
{
public:
	CMySourceExProp(IUnknown *pUnknown);
	static CUnknown * WINAPI CreateInstance(IUnknown *pUnk, HRESULT *phr);
	HRESULT OnConnect(IUnknown *pUnknown);
	HRESULT OnActivate(void);
	BOOL OnReceiveMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HRESULT OnApplyChanges(void);
	HRESULT OnDisconnect(void);
private:
	IColor *m_pColor;
	DWORD   m_CurrVal;
	DWORD   m_NewVal;
};
