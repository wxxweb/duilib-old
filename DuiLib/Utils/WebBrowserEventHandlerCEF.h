#pragma once

#ifdef UILIB_WEBBROSER_CEF

#include "cef_web_browser_event_handler_i.h"
#include "WebBrowserEventHandlerCEF.h"


namespace DuiLib
{
	class CWebBrowserCefUI;


	class UILIB_API CWebBrowserEventHandlerCEF
		: public ICefWebBrowserEventHandler
		, public CWebBrowserEventHandler
	{
	public:
		CWebBrowserEventHandlerCEF();
		virtual ~CWebBrowserEventHandlerCEF();

	public:
		virtual PVOID GetInterface(LPCTSTR _name);

		virtual bool RegisterControl(CWebBrowserCefUI* _control);

	public: // ICefWebBrowserEventHandler
		virtual void OnAfterCreated(const TSTDSTR& _startup_url);
		virtual void OnBeforeClose(void);
		virtual void OnContextCreated(const TSTDSTR& _url, bool _is_main);
		virtual void OnLoadStart(const TSTDSTR& _url, bool _is_main);
		virtual void OnLoadEnd(const TSTDSTR& _url, bool _is_main);
		virtual bool OnLoadError(
			/* [out] */ TSTDSTR& _error_text,
			/* [in] */ const TSTDSTR& _failed_url,
			/* [in] */ CefErrorCode _error_code
			);
		virtual void OnNavStateChange(
			const TSTDSTR& _url,
			bool canGoBack,
			bool canGoForward
			);
		virtual void OnAddressChange(const TSTDSTR& _url);
		virtual bool OnJsInvoke(LPCTSTR _fn_name, LPCTSTR _args);
//		virtual void OnConsoleMessage(const TSTDSTR& _msg);
		virtual void OnUncaughtException(
			const TSTDSTR& _msg,
			const TSTDSTR& _stack_trace,
			const TSTDSTR& _file_name,
			const TSTDSTR& _src_line,
			int _line_num,
			int _column
			);

	protected:
		DuiLib::CWebBrowserCefUI* m_control;
	};
}

#endif // UILIB_WEBBROSER_CEF