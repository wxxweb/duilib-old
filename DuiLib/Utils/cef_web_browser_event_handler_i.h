/*****************************************************************************
�ļ�:	ICefWebBrowserEventHandler.h
����:	���� Chromiumem Ƕ��ʽ������¼��������ӿڡ�
����:	������					
��Ȩ:	www.utooo.com
����:	2013-09-21
�޸�:	2014-05-16
******************************************************************************/

#ifndef __W2X_CEF_WEB_BROWSER_EVENT_HANDLER_H__
#define __W2X_CEF_WEB_BROWSER_EVENT_HANDLER_H__
#pragma once

#include <string>

#ifdef W2X_CEF_EXPORTS
#  ifndef  W2X_CEF_API
#    define W2X_CEF_API __declspec(dllexport)
#  endif
#else
#  ifndef W2X_CEF_API
#    define W2X_CEF_API __declspec(dllimport)
#  endif
#endif

/** ����UNICODE����ʹ�ÿ��ַ�����ַ���C++�ַ��� */
#ifndef TSTDSTR
#  ifdef UNICODE
#    define TSTDSTR std::wstring
#  else
#    define TSTDSTR std::string
#  endif
#endif

class ICefWebBrowser;

class ICefWebBrowserEventHandler
{
public:
	virtual ~ICefWebBrowserEventHandler(void) {}

public:
	/** ע��������ӿ� */
	virtual bool RegisterWebBrowser(ICefWebBrowser* _cef_web_browser) = 0;

	/** ע��������ӿ� */
	virtual void UnregisterWebBrowser(void) = 0;

	/** ����������ڱ�������ص� */
	virtual void OnAfterCreated(const TSTDSTR& _startup_url) = 0;

	/** ����������ڱ��ر�ǰ�ص� */
	virtual void OnBeforeClose(void) = 0;

	/** �� Web �ĵ���������ص�����Ϊ�����ҳ���� _is_main = true */
	virtual void OnContextCreated(const TSTDSTR& _url, bool _is_main) = 0;

	/** �� Web �ĵ���ʼ����ʱ�ص�����Ϊ�����ҳ���� _is_main = true */
	virtual void OnLoadStart(const TSTDSTR& _url, bool _is_main) = 0;

	/** �� Web �ĵ����ؽ���ʱ�ص�����Ϊ�����ҳ���� _is_main = true */
	virtual void OnLoadEnd(const TSTDSTR& _url, bool _is_main) = 0;

	/** �� Web �ĵ����س���ʱ�ص� */
	virtual bool OnLoadError(
		/* [out] */ TSTDSTR& _error_text,
		/* [in] */ const TSTDSTR& _failed_url,
		/* [in] */ DWORD _error_code
		) = 0;

	/** ������״̬�ı�ʱ���ص� */
	virtual void OnNavStateChange(
		const TSTDSTR& _url,
		bool canGoBack,
		bool canGoForward
		) = 0;

	/** �� URL ��ַ�����ı�ʱ���ص� */
	virtual void OnAddressChange(const TSTDSTR& _url) = 0;

	/**
	 * ִ�� JS �ķ������ã����뷽�������ƺͲ����ַ������������䷵��ֵ��
	 * ���������������� true��
	 */
	virtual bool OnJsInvoke(
		LPCTSTR _fn_name,
		LPCTSTR _args
		) = 0;

	/** �����յ�����̨��Ϣʱ���ص� */
	virtual void OnConsoleMessage(const TSTDSTR& _msg) = 0;
	
	/** �� JavaScript �ű�����δ�����쳣ʱ���ص� */
	virtual void OnUncaughtException(
		const TSTDSTR& _msg,
		const TSTDSTR& _file_name,
		const TSTDSTR& _src_line,
		int _line_num,
		int _column
		) = 0;
};

#endif ///< __W2X_CEF_WEB_BROWSER_EVENT_HANDLER_H__