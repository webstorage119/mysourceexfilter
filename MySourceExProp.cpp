#include "stdafx.h"

CMySourceExProp::CMySourceExProp(IUnknown *pUnknown) : 
	CBasePropertyPage(PROP_NAME, pUnknown, IDD_PROP, IDS_PROP_TITLE)
	, m_pColor(NULL)
{
}

CUnknown * WINAPI CMySourceExProp::CreateInstance(IUnknown *pUnk, HRESULT *phr) {
	CMySourceExProp *pProp=new CMySourceExProp(pUnk);
	if (pProp==NULL) {
		*phr=E_OUTOFMEMORY;
	}
	return pProp;
}

HRESULT CMySourceExProp::OnConnect(IUnknown *pUnknown) {
	HRESULT hr=pUnknown->QueryInterface(
		__uuidof(IColor), (void**)&m_pColor);
	return hr;
}

HRESULT CMySourceExProp::OnActivate(void) {
	InitCommonControls();
	m_pColor->GetColor(&m_CurrVal);
	SendDlgItemMessage(m_Dlg, IDC_SLIDER1, TBM_SETRANGE, 0,MAKELONG(0, 255));
	SendDlgItemMessage(m_Dlg, IDC_SLIDER1, TBM_SETPOS, 1, m_CurrVal);
	return S_OK;
}

BOOL CMySourceExProp::OnReceiveMessage
	(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_HSCROLL:
		switch(LOWORD(wParam)) {
		case TB_PAGEDOWN:
		case SB_THUMBTRACK:
		case TB_PAGEUP:
			m_NewVal=SendDlgItemMessage(m_Dlg, IDC_SLIDER1,
				TBM_GETPOS, 0, 0);
			m_pColor->SetColor(m_NewVal);
			__super::m_bDirty=TRUE;
			if(__super::m_pPageSite) {
				__super::m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
			}
		}
		return (LRESULT)1;
	}
	return CBasePropertyPage::OnReceiveMessage(hwnd,uMsg,wParam,lParam);
}

HRESULT CMySourceExProp::OnApplyChanges(void) {
	m_CurrVal=m_NewVal;
	return S_OK;
}

HRESULT CMySourceExProp::OnDisconnect(void) {
	if(m_pColor) {
		m_pColor->SetColor(m_CurrVal);
		m_pColor->Release();
		m_pColor=NULL;	
	}
	return S_OK;
}
