#include "StdAfx.h"
#include "UIWebBrowser.h"


DuiLib::IWebBrowserUI::IWebBrowserUI()
	: m_bAutoNavi(false)
{
	m_sHomePage.Empty();
}

DuiLib::IWebBrowserUI::~IWebBrowserUI()
{}

void DuiLib::IWebBrowserUI::SetHomePage( LPCTSTR lpszUrl )
{
	m_sHomePage.Format(_T("%s"),lpszUrl);
}

LPCTSTR DuiLib::IWebBrowserUI::GetHomePage()
{
	return m_sHomePage.GetData();
}

void DuiLib::IWebBrowserUI::SetAutoNavigation( bool bAuto /*= TRUE*/ )
{
	if (m_bAutoNavi==bAuto)	return;

	m_bAutoNavi=bAuto;
}

bool DuiLib::IWebBrowserUI::IsAutoNavigation()
{
	return m_bAutoNavi;
}
