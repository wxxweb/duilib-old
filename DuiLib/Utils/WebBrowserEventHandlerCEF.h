#pragma once

#ifdef UILIB_WEBBROSER_CEF

#include "cef_web_browser_event_handler_i.h"
#include "WebBrowserEventHandlerCEF.h"


namespace DuiLib
{
	class CWebBrowserCefUI;


	class CWebBrowserEventHandlerCEF
		: public ICefWebBrowserEventHandler
		, public CWebBrowserEventHandler
	{
	public:
		CWebBrowserEventHandlerCEF() {}
		virtual ~CWebBrowserEventHandlerCEF() {}

	public:
		virtual PVOID GetInterface(LPCTSTR _name)
		{
			if ( _tcscmp(_name, DUI_WEB_BROWSER_EVENT_HANDLER_CEF) == 0 ) {
				return static_cast<CWebBrowserEventHandlerCEF*>(this);
			}
			return NULL;
		}

		virtual bool RegisterWebBrowser(CWebBrowserCefUI* _web_browser_cef_ui)
		{ return false; }

		virtual bool RegisterWebBrowser(ICefWebBrowser* _cef_web_browser)
		{ return false; }

		virtual void UnregisterWebBrowser(void) {}

		virtual void OnAfterCreated(const TSTDSTR& _startup_url) {}

		virtual void OnBeforeClose(void) {}

		virtual void OnContextCreated(const TSTDSTR& _url, bool _is_main) {}

		virtual void OnLoadStart(const TSTDSTR& _url, bool _is_main) {}

		virtual void OnLoadEnd(const TSTDSTR& _url, bool _is_main) {}

		virtual bool OnLoadError(
			/* [out] */ TSTDSTR& _error_text,
			/* [in] */ const TSTDSTR& _failed_url,
			/* [in] */ DWORD _error_code
			) { return false; }

		virtual void OnNavStateChange(
			const TSTDSTR& _url,
			bool canGoBack,
			bool canGoForward
			) {}

		virtual void OnAddressChange(const TSTDSTR& _url) {}

		virtual bool OnJsInvoke(LPCTSTR _fn_name, LPCTSTR _args)
		{ return false; }

		virtual void OnConsoleMessage(const TSTDSTR& _msg) {}

		virtual void OnUncaughtException(
			const TSTDSTR& _msg,
			const TSTDSTR& _file_name,
			const TSTDSTR& _src_line,
			int _line_num,
			int _column
			) {}
	};
}

#endif // UILIB_WEBBROSER_CEF