#include"stdafx.h"

CPushPin::CPushPin(HRESULT *phr, CSource *pFilter) : 
	CSourceStream(NAME("CPushPin"), phr, pFilter, OUTPUT_PIN_NAME)
	,m_rtFrameLength(666666)
{
}

CPushPin::~CPushPin() {
}

STDMETHODIMP CPushPin::Notify(IBaseFilter *pSelf, Quality q) {
	return E_FAIL;
}

HRESULT CPushPin::GetMediaType(CMediaType *pMediaType) {
	HRESULT hr=NOERROR;
	CheckPointer(pMediaType,E_POINTER);

	VIDEOINFO *pvi=(VIDEOINFO*)pMediaType->AllocFormatBuffer(sizeof(VIDEOINFO));
	ZeroMemory(pvi, sizeof(VIDEOINFO));

	pvi->AvgTimePerFrame=m_rtFrameLength;

	BITMAPINFOHEADER *pBmi=&(pvi->bmiHeader);
	pBmi->biSize=sizeof(BITMAPINFOHEADER);
	pBmi->biWidth=240;
	pBmi->biHeight=-180;
	pBmi->biPlanes=1;
	pBmi->biBitCount=32;
	pBmi->biCompression=BI_RGB;
	pvi->bmiHeader.biSizeImage=DIBSIZE(pvi->bmiHeader);

	pMediaType->SetType(&MEDIATYPE_Video);
	pMediaType->SetFormatType(&FORMAT_VideoInfo);

	const GUID subtype=GetBitmapSubtype(&pvi->bmiHeader);
	pMediaType->SetSubtype(&subtype);
	pMediaType->SetTemporalCompression(FALSE);
	pMediaType->SetSampleSize(DIBSIZE(*pBmi));
	return hr;
}
HRESULT CPushPin::CheckMediaType(const CMediaType *pMediaType) {
	HRESULT hr=NOERROR;
	CheckPointer(pMediaType, E_POINTER);
	CMediaType mt;
	GetMediaType(&mt);
	if(mt==*pMediaType) {
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CPushPin::DecideBufferSize(IMemAllocator *pAlloc,
								   ALLOCATOR_PROPERTIES *pRequest)
{
	HRESULT hr=NOERROR;
	VIDEOINFO *pvi=(VIDEOINFO*)m_mt.Format();
	ASSERT(pvi != NULL);
	pRequest->cBuffers=1;
	// バッファサイズはビットマップ1枚分以上である必要がある
	if(pvi->bmiHeader.biSizeImage > (DWORD)pRequest->cbBuffer) {
		pRequest->cbBuffer=pvi->bmiHeader.biSizeImage;
	}
	// アロケータプロパティを設定しなおす
	ALLOCATOR_PROPERTIES Actual;
	hr=pAlloc->SetProperties(pRequest, &Actual);
	if(FAILED(hr)) {
		return hr;
	}
	if(Actual.cbBuffer<pRequest->cbBuffer) {
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CPushPin::FillBuffer(IMediaSample *pSample) {
	HRESULT hr;
	CheckPointer(pSample,E_POINTER);
	// ダウンストリームフィルタが
	// フォーマットを動的に変えていないかチェック
	ASSERT(m_mt.formattype == FORMAT_VideoInfo);
	ASSERT(m_mt.cbFormat >= sizeof(VIDEOINFOHEADER));

	//バッファのポインタを取得して書き込む
	LPBYTE pSampleData=NULL;
	const long size=pSample->GetSize();
	pSample->GetPointer(&pSampleData);
	LPDWORD pWrite=(LPDWORD)pSampleData;

	DWORD value;
	IColor *pColor=NULL;
	hr=m_pFilter->QueryInterface(__uuidof(IColor), (void**)&pColor);
	pColor->GetColor(&value);
	pColor->Release();
	for(int x=0;x<240;x++) {
		for(int y=0;y<180;y++) {
			*pWrite++=value;
		}
	}

	CRefTime ref;
	m_pFilter->StreamTime(ref);
	const REFERENCE_TIME delta=m_rtFrameLength;
	REFERENCE_TIME start_time=ref;
	FILTER_STATE state;
	m_pFilter->GetState(0, &state);
	if(state==State_Paused)
		start_time = 0;
	REFERENCE_TIME end_time=(start_time+delta);
	pSample->SetTime(&start_time, &end_time);
	pSample->SetActualDataLength(size);
	pSample->SetSyncPoint(TRUE);
	return S_OK;
}
