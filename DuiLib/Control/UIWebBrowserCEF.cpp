#include "StdAfx.h"

#ifdef UILIB_WEBBROSER_CEF

#include "UIWebBrowserCEF.h"
#include "../Utils/cef_web_browser.h"
#include "../Utils/cef_web_browser_event_handler_i.h"
#include <Shlwapi.h>
#include <set>

#pragma comment(lib, "w2x_cef.lib")

namespace DuiLib
{

class CInternEventHandle: public ICefWebBrowserEventHandler
{
public:
	CInternEventHandle(CWebBrowserCefUI* _ui, ICefWebBrowserEventHandler* _impl)
		: m_ui(_ui), m_pImpl(_impl) {}
	~CInternEventHandle(void) {}

public:
	virtual bool RegisterWebBrowser(ICefWebBrowser* _cef_web_browser)
	{
		if (NULL == m_pImpl) { return false; }
		return m_pImpl->RegisterWebBrowser(_cef_web_browser);
	}

	virtual void UnregisterWebBrowser(void)
	{
		if (NULL == m_pImpl) { return; }
		m_pImpl->UnregisterWebBrowser();
	}

	virtual void OnAfterCreated(const TSTDSTR& _startup_url)
	{
		//m_ui->SetInternVisible(true);
		if (NULL == m_pImpl) { return; }
		m_pImpl->OnAfterCreated(_startup_url);
	}

	virtual void OnBeforeClose(void)
	{
		if (NULL == m_pImpl) { return; }
		m_pImpl->OnBeforeClose();
	}

	virtual void OnContextCreated(const TSTDSTR& _url, bool _is_main)
	{
		if (NULL == m_pImpl) { return; }
		m_pImpl->OnContextCreated(_url, _is_main);
	}

	virtual void OnLoadStart(const TSTDSTR& _url, bool _is_main)
	{
		if (NULL == m_pImpl) { return; }
		m_pImpl->OnLoadStart(_url, _is_main);
	}

	virtual void OnLoadEnd(const TSTDSTR& _url, bool _is_main)
	{
		if (NULL == m_pImpl) { return; }
		m_pImpl->OnLoadEnd(_url, _is_main);
	}

	virtual bool OnLoadError(
		/* [out] */ TSTDSTR& _error_text,
		/* [in] */ const TSTDSTR& _failed_url,
		/* [in] */ DWORD _error_code
		)
	{
		if (NULL == m_pImpl) { return false; }
		return m_pImpl->OnLoadError(_error_text, _failed_url, _error_code);
	}

	virtual void OnNavStateChange(
		const TSTDSTR& _url,
		bool canGoBack,
		bool canGoForward
		)
	{
		if (NULL == m_pImpl) { return; }
		m_pImpl->OnNavStateChange(_url, canGoBack, canGoForward);
	}

	virtual void OnAddressChange(const TSTDSTR& _url)
	{
		if (NULL == m_pImpl) { return; }
		m_pImpl->OnAddressChange(_url);
	}

	virtual bool OnJsInvoke(
		LPCTSTR _fn_name,
		LPCTSTR _args
		)
	{
		if (NULL == m_pImpl) { return false; }
		return m_pImpl->OnJsInvoke(_fn_name, _args);
	}

	virtual void OnConsoleMessage(const TSTDSTR& _msg)
	{
		if (NULL == m_pImpl) { return; }
		m_pImpl->OnConsoleMessage(_msg);
	}
	
	virtual void OnUncaughtException(
		const TSTDSTR& _msg,
		const TSTDSTR& _file_name,
		const TSTDSTR& _src_line,
		int _line_num,
		int _column
		)
	{
		if (NULL == m_pImpl) { return; }
		m_pImpl->OnUncaughtException(_msg, _file_name, _src_line, _line_num, _column);
	}

public:
	void SetImpl(ICefWebBrowserEventHandler* _impl)
	{
		m_pImpl = _impl;
	}

private:
	ICefWebBrowserEventHandler* m_pImpl;
	CWebBrowserCefUI* m_ui;
};


class CWebBrowserCefUI::CImpl
{
public:
	typedef std::set<TSTDSTR> ExternalSet;

public:
	CImpl(CWebBrowserCefUI* _parent);
	~CImpl(void);

public:
	CCefWebBrowser* const m_pCefWebBrowser;
	CInternEventHandle* m_pEventHandler;	// 浏览器事件处理
	bool m_bInit;
	bool m_bAutoNavi;	// 是否启动时打开默认页面

	static ExternalSet sm_external_set;
};

CWebBrowserCefUI::CImpl::ExternalSet CWebBrowserCefUI::CImpl::sm_external_set;

CWebBrowserCefUI::CImpl::CImpl(CWebBrowserCefUI* _parent)
	: m_pCefWebBrowser(new CCefWebBrowser())
	, m_pEventHandler(new CInternEventHandle(_parent, NULL))
	, m_bInit(false)
	, m_bAutoNavi(false)
{
	m_pEventHandler->RegisterWebBrowser(m_pCefWebBrowser);
}


CWebBrowserCefUI::CImpl::~CImpl(void)
{
	if (NULL == m_pEventHandler)
	{
		m_pEventHandler->UnregisterWebBrowser();
		delete m_pEventHandler;
	}

	delete m_pCefWebBrowser;
}


CWebBrowserCefUI::CWebBrowserCefUI()
	: m_impl(new CImpl(this))
	
{}


CWebBrowserCefUI::~CWebBrowserCefUI()
{
	delete m_impl;
}

LPCTSTR CWebBrowserCefUI::GetClass() const
{
	return _T("WebBrowserCefUI");
}

LPVOID CWebBrowserCefUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_WEBBROWSER) == 0 ||
		_tcscmp(pstrName, DUI_CTR_WEBBROWSERCEF) == 0 ) {
		return static_cast<CWebBrowserCefUI*>(this);
	}
	return CControlUI::GetInterface(pstrName);
}

void CWebBrowserCefUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("homepage")) == 0) 
	{
		this->SetHomePage(pstrValue);
	} 
	else if (_tcscmp(pstrName, _T("autonavi"))==0) 
	{
		m_bAutoNavi = (_tcscmp(pstrValue, _T("true")) == 0);
	} else {
		CControlUI::SetAttribute(pstrName, pstrValue);
	}
}

void CWebBrowserCefUI::SetPos(RECT rc)
{
	CControlUI::SetPos(rc);

	if (NULL == m_impl->m_pCefWebBrowser) {
		return;
	}

	m_impl->m_pCefWebBrowser->MoveBrowser(
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

void CWebBrowserCefUI::SetInternVisible(bool bVisible)
{
	CControlUI::SetInternVisible(bVisible);

	if (false == m_impl->m_bInit || NULL == m_impl->m_pCefWebBrowser) {
		return;
	}

	HWND hWnd = m_impl->m_pCefWebBrowser->GetHwnd();
	if (NULL != hWnd && FALSE != ::IsWindow(hWnd)) {
		if (true == bVisible) {
			::ShowWindow(hWnd, SW_SHOWNORMAL);
		} else {
			::ShowWindow(hWnd, SW_HIDE);
		}
	}
}

void CWebBrowserCefUI::DoInit()
{
	CControlUI::DoInit();

	if (true == m_impl->m_bInit) {
		return;
	} else {
		m_impl->m_bInit = true;
	}
	if (NULL == m_impl->m_pCefWebBrowser) {
		return;
	}

	LPCTSTR pszUrl = TEXT("about:blank");
	if (true == m_bAutoNavi && false == m_sHomePage.IsEmpty()) {
		pszUrl = m_sHomePage.GetData();
	}
	HWND hWndParent = m_pManager->GetPaintWindow();
	if (true == m_impl->m_pCefWebBrowser->Create(
		pszUrl, hWndParent, m_impl->m_pEventHandler))
	{
		for (CImpl::ExternalSet::iterator it = CImpl::sm_external_set.begin(),
				itEnd = CImpl::sm_external_set.end(); itEnd != it; ++it)
		{
			m_impl->m_pCefWebBrowser->AddExternal(*it);
		}
	}
}

void CWebBrowserCefUI::Navigate( LPCTSTR lpszUrl )
{
	if (lpszUrl == NULL)
		return;

	if (m_impl->m_pCefWebBrowser)
	{
		m_impl->m_pCefWebBrowser->LoadUrl(lpszUrl);
	}
}

void CWebBrowserCefUI::Refresh()
{
	if (m_impl->m_pCefWebBrowser)
	{
		//m_pCefWebBrowser->
	}
}

void CWebBrowserCefUI::GoBack()
{
	if (m_impl->m_pCefWebBrowser)
	{
		//m_pCefWebBrowser->
	}
}

void CWebBrowserCefUI::GoForward()
{
	if (m_impl->m_pCefWebBrowser)
	{
		//m_pCefWebBrowser->
	}
}

void CWebBrowserCefUI::NavigateHomePage()
{
	if (!m_sHomePage.IsEmpty())
		this->Navigate(m_sHomePage);
}

void CWebBrowserCefUI::SetWebBrowserEventHandler( CWebBrowserEventHandler* pEventHandler )
{
	if (NULL != pEventHandler && NULL != m_impl->m_pEventHandler &&
		NULL != pEventHandler->GetInterface(DUI_WEB_BROWSER_EVENT_HANDLER_CEF))
	{
		CWebBrowserEventHandlerCEF* pEventHandlerCef = 
			static_cast<CWebBrowserEventHandlerCEF*>(pEventHandler);
		pEventHandlerCef->RegisterWebBrowser(this);
		m_impl->m_pEventHandler->SetImpl(pEventHandlerCef);
	}
}

bool CWebBrowserCefUI::ExecuteJsCode(LPCTSTR pstrJsCode)
{
	if (NULL == m_impl->m_pCefWebBrowser) {
		return false;
	}
	return m_impl->m_pCefWebBrowser->ExecuteJsCode(pstrJsCode);
}

bool CWebBrowserCefUI::AddExternal(LPCTSTR _fn_name)
{
	if (NULL == _fn_name || TEXT('\0') == _fn_name[0]) {
		return false;
	}

	CImpl::sm_external_set.insert(_fn_name);
	
	return true;
}

bool CWebBrowserCefUI::RegisterCustomScheme(
	LPCTSTR _scheme_name, 
	CustomSchemeHandler _handler
	)
{
	return CCefWebBrowser::RegisterCustomScheme(_scheme_name, _handler);
}


}; // namespace DuiLib

#endif // UILIB_WEBBROSER_CEF