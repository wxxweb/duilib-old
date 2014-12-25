#ifndef __UIWEBBROWSERCEF_H__
#define __UIWEBBROWSERCEF_H__

#pragma once

#ifdef UILIB_WEBBROSER_CEF

#include "../Utils/WebBrowserEventHandlerCEF.h"

class CCefWebBrowser;

namespace DuiLib
{
	typedef bool (CALLBACK* CustomSchemeHandler)(
		LPCTSTR _scheme_name, 
		LPCTSTR _url
		);

	class CInternEventHandle;

	/**
	 * 要使用 CEF 作为 Web 浏览器框架需要确保主程序根目录下包含以下文件及目录：
	 * icudt.dll、libcef.dll、w2x_cef.dll、locales
	 */
	class UILIB_API CWebBrowserCefUI
		: public IWebBrowserUI
		, public CControlUI
	{
		friend class CCefWebBrowserWnd;
	public:
		CWebBrowserCefUI();
		virtual ~CWebBrowserCefUI();

	public: /// 实现 CWebBrowserUI 接口
		virtual void Navigate(LPCTSTR lpszUrl);
		virtual void Refresh();
		virtual void GoBack();
		virtual void GoForward();
		virtual void NavigateHomePage();
		virtual void SetWebBrowserEventHandler(CWebBrowserEventHandler* pEventHandler);

	public: /// 重载 CControlUI 虚成员函数
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface( LPCTSTR pstrName ) override;
		virtual void SetPos(RECT rc) override;
		virtual void DoInit() override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		virtual void SetInternVisible(bool bVisible = true) override;

	public:
		static bool RegisterCustomScheme(
			LPCTSTR _scheme_name, 
			CustomSchemeHandler _handler
			);

	private:
		CCefWebBrowser* const m_pCefWebBrowser;
		CInternEventHandle* m_pEventHandler;	//浏览器事件处理
		
		bool m_bInit;
		bool m_bAutoNavi;	// 是否启动时打开默认页面
	};
}; // namespace DuiLib

#endif // UILIB_WEBBROSER_CEF

#endif // __UIWEBBROWSERCEF_H__