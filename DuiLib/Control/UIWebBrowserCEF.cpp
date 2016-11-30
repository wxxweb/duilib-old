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

class CInternEventHandler: public ICefWebBrowserEventHandler
{
public:
	CInternEventHandler(CWebBrowserCefUI* _ui, ICefWebBrowserEventHandler* _impl)
		: m_ui(_ui), m_pImpl(_impl) {}
	~CInternEventHandler(void) {}

public:
	virtual void OnAfterCreated(const TSTDSTR& _startup_url)
	{
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
		if (NULL == m_pImpl) {
			m_ui->SetInternVisible(true);
			return;
		}
		m_pImpl->OnLoadEnd(_url, _is_main);
	}

	virtual bool OnLoadError(
		/* [out] */ TSTDSTR& _error_text,
		/* [in] */ const TSTDSTR& _failed_url,
		/* [in] */ CefErrorCode _error_code
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

// 	virtual void OnConsoleMessage(const TSTDSTR& _msg)
// 	{
// 		if (NULL == m_pImpl) { return; }
// 		m_pImpl->OnConsoleMessage(_msg);
// 	}
	
	virtual void OnUncaughtException(
		const TSTDSTR& _msg,
		const TSTDSTR& _stack_trace,
		const TSTDSTR& _file_name,
		const TSTDSTR& _src_line,
		int _line_num,
		int _column
		)
	{
		if (NULL == m_pImpl) { return; }
		m_pImpl->OnUncaughtException(_msg, _stack_trace, _file_name, _src_line, _line_num, _column);
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
	CImpl(void);
	~CImpl(void);

public:
	CCefWebBrowserWnd* m_pWindow;
	CCefWebBrowser* const m_pCefWebBrowser;
	CInternEventHandler* m_pEventHandler;	// 浏览器事件处理
	bool m_bInit;
	bool m_bAutoNavi;	// 是否启动时打开默认页面

	static ExternalSet sm_external_set;
};


class CCefWebBrowserWnd : public CWindowWnd
{
public:
	CCefWebBrowserWnd();

	void Init(CWebBrowserCefUI* pOwner);

	virtual LPCTSTR GetWindowClassName() const;
	virtual void OnFinalMessage(HWND hWnd);

protected:
	CWebBrowserCefUI* m_pOwner;
	bool m_bInit;
};


CCefWebBrowserWnd::CCefWebBrowserWnd() : m_pOwner(NULL), m_bInit(false)
{
}

void CCefWebBrowserWnd::Init(CWebBrowserCefUI* pOwner)
{
	m_pOwner = pOwner;
	RECT rcPos = m_pOwner->GetPos();
	this->Create(m_pOwner->GetManager()->GetPaintWindow(),
		TEXT("CefWebBrowserWnd"), UI_WNDSTYLE_CHILD, WS_EX_TOOLWINDOW, rcPos);
	::SetClassLong(m_hWnd, GCL_HBRBACKGROUND, (LONG)::GetStockObject(WHITE_BRUSH));
	m_bInit = true;
}


LPCTSTR CCefWebBrowserWnd::GetWindowClassName() const
{
	return _T("CefWebBrowserWnd");
}


void CCefWebBrowserWnd::OnFinalMessage(HWND /*hWnd*/)
{
	m_pOwner->Invalidate();
	m_pOwner->m_impl->m_pWindow = NULL;
	delete this;
}


CWebBrowserCefUI::CImpl::ExternalSet CWebBrowserCefUI::CImpl::sm_external_set;


CWebBrowserCefUI::CImpl::CImpl(void)
	: m_pWindow(new CCefWebBrowserWnd)
	, m_pCefWebBrowser(new CCefWebBrowser())
	, m_pEventHandler(NULL)
	, m_bInit(false)
	, m_bAutoNavi(false)
{
	
}


CWebBrowserCefUI::CImpl::~CImpl(void)
{
	if (NULL == m_pEventHandler) {
		delete m_pEventHandler;
	}

	delete m_pCefWebBrowser;
	delete m_pWindow;
}


CWebBrowserCefUI::CWebBrowserCefUI()
	: m_impl(new CImpl)
{
	m_impl->m_pEventHandler = new CInternEventHandler(this, NULL);
}


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

	RECT rcPos = this->GetPos();
	HWND hwnd = m_impl->m_pWindow->GetHWND();
	::SetWindowPos(hwnd, NULL, 
		rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
		rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);  

	::GetWindowRect(hwnd, &rcPos);

	m_impl->m_pCefWebBrowser->MoveBrowser(0, 0);
	m_impl->m_pCefWebBrowser->ChangeBrowserSize(rc.right - rc.left, rc.bottom - rc.top);
}

void CWebBrowserCefUI::SetInternVisible(bool bVisible)
{
	CControlUI::SetInternVisible(bVisible);

	if (false == m_impl->m_bInit || NULL == m_impl->m_pCefWebBrowser) {
		return;
	}

	HWND hWndParent = m_impl->m_pWindow->GetHWND();
	HWND hWnd = m_impl->m_pCefWebBrowser->GetHwnd();
	if (true == bVisible) {
		::ShowWindow(hWndParent, SW_SHOW);
		::ShowWindow(hWnd, SW_SHOW);
	} else {
		::ShowWindow(hWndParent, SW_SHOW);
		::ShowWindow(hWnd, SW_HIDE);
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

	m_impl->m_pWindow->Init(this);

	LPCTSTR pszUrl = TEXT("about:blank");
	if (true == m_bAutoNavi && false == m_sHomePage.IsEmpty()) {
		pszUrl = m_sHomePage.GetData();
	}
	
	if (true == m_impl->m_pCefWebBrowser->Create(
		pszUrl, m_impl->m_pWindow->GetHWND(), m_impl->m_pEventHandler))
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
	if (NULL == m_impl->m_pEventHandler) {
		return;
	}
	
	if (NULL != pEventHandler &&
		NULL != pEventHandler->GetInterface(DUI_WEB_BROWSER_EVENT_HANDLER_CEF))
	{
		CWebBrowserEventHandlerCEF* pEventHandlerCef = 
			static_cast<CWebBrowserEventHandlerCEF*>(pEventHandler);
		pEventHandlerCef->RegisterControl(this);
		m_impl->m_pEventHandler->SetImpl(pEventHandlerCef);
	}
	else if (NULL == pEventHandler) {
		m_impl->m_pEventHandler->SetImpl(NULL);
	}
	else {
		ASSERT(false);
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