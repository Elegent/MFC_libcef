#pragma once
//#include <afx.h>
#include <afxdialogex.h>

class CefMainDlg : public CDialogEx
{
	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1};
#endif

public:
	CefMainDlg(CWnd* pParent = NULL);
	virtual ~CefMainDlg();
	virtual BOOL OnInitDialog();
	CRect GetRect();
	void SetRect(CRect m_Rect);
public:
	CRect m_MainRect;



};

