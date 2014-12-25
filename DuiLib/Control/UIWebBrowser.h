#ifndef __UIWEBBROWSER_H__
#define __UIWEBBROWSER_H__

#pragma once

#include "../Utils/WebBrowserEventHandler.h"

namespace DuiLib
{
	class UILIB_API IWebBrowserUI
	{
	public:
		/// 构造函数
		IWebBrowserUI();
		virtual ~IWebBrowserUI();

	public:
		virtual LPVOID GetInterface(LPCTSTR pstrName) = 0;
		virtual void Navigate(LPCTSTR lpszUrl) = 0;
		virtual void Refresh() = 0;
		virtual void GoBack() = 0;
		virtual void GoForward() = 0;
		virtual void NavigateHomePage() = 0;
		virtual void SetWebBrowserEventHandler(CWebBrowserEventHandler* pEventHandler) = 0;

	public:
		void SetHomePage(LPCTSTR lpszUrl);
		LPCTSTR GetHomePage();
		void SetAutoNavigation(bool bAuto = true);
		bool IsAutoNavigation();
		
	protected:
		CDuiString m_sHomePage;	// 默认页面
		bool m_bAutoNavi;	// 是否启动时打开默认页面
	};
} // namespace DuiLib

#endif // __UIWEBBROWSER_H__