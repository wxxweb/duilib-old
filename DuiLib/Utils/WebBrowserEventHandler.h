#pragma once

#define DUI_WEB_BROWSER_EVENT_HANDLER_IE	(_T("WebBrowserEventHandlerIE"))
#define DUI_WEB_BROWSER_EVENT_HANDLER_CEF	(_T("WebBrowserEventHandlerCEF"))

namespace DuiLib
{
	class CWebBrowserEventHandler
	{
	public:
		virtual ~CWebBrowserEventHandler() = 0 {}

	public:
		virtual PVOID GetInterface(LPCTSTR _name) = 0;
	};
}
