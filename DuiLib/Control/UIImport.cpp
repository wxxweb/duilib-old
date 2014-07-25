#include "stdafx.h"
#include "UIImport.h"

namespace DuiLib
{
	CImportUI::CImportUI()
	{
	}

	CImportUI::~CImportUI()
	{
	}

	LPCTSTR CImportUI::GetClass() const
	{
		return _T("ImportUI");
	}

	LPVOID CImportUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_IMPORT) == 0 ) {
			return static_cast<CImportUI*>(this);
		}
		return CControlUI::GetInterface(pstrName);
	}

	void CImportUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("name")) == 0 ) {
			this->SetName(pstrValue);
		}
		else if( _tcscmp(pstrName, _T("text")) == 0 ) {
			this->SetText(pstrValue);
		}
	}
}