#include"stdafx.h"

CMySourceEx::CMySourceEx(IUnknown *pUnk, HRESULT *phr) : 
	CSource(FILTER_NAME, pUnk, CLSID_MySourceEx), m_Color(0xff)
{
	CPushPin *pPin=new CPushPin(phr, this);
	if (pPin==NULL) {
		*phr=E_OUTOFMEMORY;
	}
}

CMySourceEx::~CMySourceEx() {
}

CUnknown * WINAPI CMySourceEx::CreateInstance(IUnknown *pUnk, HRESULT *phr) {
	CMySourceEx *pNewFilter=new CMySourceEx(pUnk, phr );
	if (pNewFilter==NULL) {
		*phr=E_OUTOFMEMORY;
	}
	return pNewFilter;
}

STDMETHODIMP CMySourceEx::NonDelegatingQueryInterface(REFIID riid, void **ppv) {
	if (riid==IID_ISpecifyPropertyPages) {
		return GetInterface((ISpecifyPropertyPages*)this, ppv);
	}
	if (riid==__uuidof(IColor)) {
		return GetInterface((IColor*)this, ppv);
	}
	return CSource::NonDelegatingQueryInterface(riid, ppv);
}

STDMETHODIMP CMySourceEx::SetColor(DWORD color) {
	m_Color=color;
	return S_OK;
}
STDMETHODIMP CMySourceEx::GetColor(DWORD *pColor) {
	*pColor=m_Color;
	return S_OK;
}

STDMETHODIMP CMySourceEx::GetPages(CAUUID *pPages) {
	CheckPointer(pPages, E_POINTER);
	pPages->cElems=1;
	pPages->pElems=(GUID*)CoTaskMemAlloc(sizeof(GUID));
	CheckPointer(pPages->pElems, E_OUTOFMEMORY);
	pPages->pElems[0]=CLSID_MySourceExProp;
	return S_OK;
}
