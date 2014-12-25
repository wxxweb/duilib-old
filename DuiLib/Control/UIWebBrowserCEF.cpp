#include "StdAfx.h"

#ifdef UILIB_WEBBROSER_CEF

#include "UIWebBrowserCEF.h"
#include "../Utils/cef_web_browser.h"
#include "../Utils/cef_web_browser_event_handler_i.h"
#include <Shlwapi.h>

#pragma comment(lib, "w2x_cef.lib")

namespace DuiLib
{

static size_t gs_nInstanceCount = 0;

class CInternEventHandle: public ICefWebBrowserEventHandler
{
public:
	CInternEventHandle(ICefWebBrowserEventHandler* _impl) : m_pImpl(_impl) {}
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
};

CWebBrowserCefUI::CWebBrowserCefUI()
	: m_pCefWebBrowser(new CCefWebBrowser())
	, m_pEventHandler(new CInternEventHandle(NULL))
	, m_bInit(false)
	, m_bAutoNavi(false)
{
	m_bInternVisible = false;

	if (0 == gs_nInstanceCount++)
	{
		TSTDSTR sRootPath = CPaintManagerUI::GetCurrentPath().GetData();
		TSTDSTR sLocalesPath = sRootPath + TEXT("locales");
		TSTDSTR sCachePath = sRootPath + TEXT("cache");
		TSTDSTR sLogPath = sRootPath + TEXT("log\\cef.log");
		bool ret = CCefWebBrowser::Initialize(
			sLocalesPath.c_str(), sCachePath.c_str(), sLogPath.c_str());
		ASSERT(ret);
	}
	m_pEventHandler->RegisterWebBrowser(m_pCefWebBrowser);
}


CWebBrowserCefUI::~CWebBrowserCefUI()
{
	if (NULL == m_pCefWebBrowser)
	{
		m_pEventHandler->UnregisterWebBrowser();
		delete m_pEventHandler;
	}
	
	delete m_pCefWebBrowser;

	if (0 == --gs_nInstanceCount)
	{
		CCefWebBrowser::Uninitialize();
	}
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

	if (NULL == m_pCefWebBrowser) {
		return;
	}

	HWND hWnd = m_pCefWebBrowser->GetHwnd();
	if (NULL == hWnd || FALSE == ::IsWindow(hWnd)) {
		return;
	}
	::MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
}

void CWebBrowserCefUI::SetInternVisible(bool bVisible)
{
	const bool bPreInternVisible = m_bInternVisible;
	CControlUI::SetInternVisible(bVisible);

	if (false == m_bInit || NULL == m_pCefWebBrowser) {
		return;
	}
	HWND hWnd = m_pCefWebBrowser->GetHwnd();
	if (NULL == hWnd || FALSE == ::IsWindow(hWnd)) {
		return;
	}
	if (true == bVisible) {
		if (bPreInternVisible != bVisible) {
			::MoveWindow(hWnd, m_rcItem.left, m_rcItem.top,
				m_rcItem.right - m_rcItem.left, 
				m_rcItem.bottom - m_rcItem.top, FALSE);
		}
		::ShowWindow(hWnd, SW_SHOWNOACTIVATE);
	} else {
		::ShowWindow(hWnd, SW_HIDE);
	}

	
}

void CWebBrowserCefUI::DoInit()
{
	CControlUI::DoInit();

	if (true == m_bInit) {
		return;
	} else {
		m_bInit = true;
	}
	if (NULL == m_pCefWebBrowser) {
		return;
	}

	LPCTSTR pszUrl = TEXT("about:blank");
	if (true == m_bAutoNavi && false == m_sHomePage.IsEmpty()) {
		pszUrl = m_sHomePage.GetData();
	}
	HWND hWndParent = m_pManager->GetPaintWindow();
	if (true == m_pCefWebBrowser->Create(pszUrl, hWndParent, m_pEventHandler))
	{
		::ShowWindow(m_pCefWebBrowser->GetHwnd(), SW_HIDE);
		m_pCefWebBrowser->AddExternal(TEXT("notifyWebFrame"));
	}

	return;
}

void CWebBrowserCefUI::Navigate( LPCTSTR lpszUrl )
{
	if (lpszUrl == NULL)
		return;

	if (m_pCefWebBrowser)
	{
		m_pCefWebBrowser->LoadUrl(lpszUrl);
	}
}

void CWebBrowserCefUI::Refresh()
{
	if (m_pCefWebBrowser)
	{
		//m_pCefWebBrowser->
	}
}

void CWebBrowserCefUI::GoBack()
{
	if (m_pCefWebBrowser)
	{
		//m_pCefWebBrowser->
	}
}

void CWebBrowserCefUI::GoForward()
{
	if (m_pCefWebBrowser)
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
	if (NULL != pEventHandler && NULL != m_pEventHandler &&
		NULL != pEventHandler->GetInterface(DUI_WEB_BROWSER_EVENT_HANDLER_CEF))
	{
		CWebBrowserEventHandlerCEF* pEventHandlerCef = 
			static_cast<CWebBrowserEventHandlerCEF*>(pEventHandler);
		pEventHandlerCef->RegisterWebBrowser(this);
		m_pEventHandler->SetImpl(pEventHandlerCef);
	}
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