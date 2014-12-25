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
	 * Ҫʹ�� CEF ��Ϊ Web ����������Ҫȷ���������Ŀ¼�°��������ļ���Ŀ¼��
	 * icudt.dll��libcef.dll��w2x_cef.dll��locales
	 */
	class UILIB_API CWebBrowserCefUI
		: public IWebBrowserUI
		, public CControlUI
	{
		friend class CCefWebBrowserWnd;
	public:
		CWebBrowserCefUI();
		virtual ~CWebBrowserCefUI();

	public: /// ʵ�� CWebBrowserUI �ӿ�
		virtual void Navigate(LPCTSTR lpszUrl);
		virtual void Refresh();
		virtual void GoBack();
		virtual void GoForward();
		virtual void NavigateHomePage();
		virtual void SetWebBrowserEventHandler(CWebBrowserEventHandler* pEventHandler);

	public: /// ���� CControlUI ���Ա����
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
		CInternEventHandle* m_pEventHandler;	//������¼�����
		
		bool m_bInit;
		bool m_bAutoNavi;	// �Ƿ�����ʱ��Ĭ��ҳ��
	};
}; // namespace DuiLib

#endif // UILIB_WEBBROSER_CEF

#endif // __UIWEBBROWSERCEF_H__