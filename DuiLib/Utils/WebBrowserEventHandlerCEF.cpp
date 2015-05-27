#include "stdafx.h"
#include "WebBrowserEventHandlerCEF.h"


namespace DuiLib
{


CWebBrowserEventHandlerCEF::CWebBrowserEventHandlerCEF()
	: m_control(NULL)
{}


CWebBrowserEventHandlerCEF::~CWebBrowserEventHandlerCEF()
{}


PVOID CWebBrowserEventHandlerCEF::GetInterface(LPCTSTR _name)
{
	if ( _tcscmp(_name, DUI_WEB_BROWSER_EVENT_HANDLER_CEF) == 0 ) {
		return static_cast<CWebBrowserEventHandlerCEF*>(this);
	}
	return NULL;
}


bool CWebBrowserEventHandlerCEF::RegisterControl(CWebBrowserCefUI* _control)
{
	if (NULL != _control) {
		m_control = _control;
		return true;
	}
	return false;
}


void CWebBrowserEventHandlerCEF::OnAfterCreated(const TSTDSTR& _startup_url)
{

}


void CWebBrowserEventHandlerCEF::OnBeforeClose(void)
{}


void CWebBrowserEventHandlerCEF::OnContextCreated(const TSTDSTR& _url, bool _is_main)
{}


void CWebBrowserEventHandlerCEF::OnLoadStart(const TSTDSTR& _url, bool _is_main)
{}


void CWebBrowserEventHandlerCEF::OnLoadEnd(const TSTDSTR& _url, bool _is_main)
{
	if (true == _is_main && NULL != m_control) {
		m_control->SetInternVisible(true);
	}
}


bool CWebBrowserEventHandlerCEF::OnLoadError(
	/* [out] */ TSTDSTR& _error_text,
	/* [in] */ const TSTDSTR& _failed_url,
	/* [in] */ DWORD _error_code
	)
{
	return false;
}


void CWebBrowserEventHandlerCEF::OnNavStateChange(
	const TSTDSTR& _url,
	bool canGoBack,
	bool canGoForward
	)
{}


void CWebBrowserEventHandlerCEF::OnAddressChange(const TSTDSTR& _url)
{}


bool CWebBrowserEventHandlerCEF::OnJsInvoke(
	LPCTSTR _fn_name,
	LPCTSTR _args
	)
{ return false; }


void CWebBrowserEventHandlerCEF::OnConsoleMessage(const TSTDSTR& _msg)
{}


void CWebBrowserEventHandlerCEF::OnUncaughtException(
	const TSTDSTR& _msg,
	const TSTDSTR& _file_name,
	const TSTDSTR& _src_line,
	int _line_num,
	int _column
	)
{}


} // namespace DuiLib