#ifndef __UIIMPORT_H__
#define __UIIMPORT_H__

#pragma once

namespace DuiLib
{
	class DUILIB_API CImportUI : public CControlUI
	{
	public:
		CImportUI();
		~CImportUI();

		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface(LPCTSTR pstrName);
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	};

} // namespace DuiLib

#endif // __UIIMPORT_H__