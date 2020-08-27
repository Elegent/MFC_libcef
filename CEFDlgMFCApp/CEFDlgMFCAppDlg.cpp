﻿
// CEFDlgMFCAppDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CEFDlgMFCApp.h"
#include "CEFDlgMFCAppDlg.h"
#include "afxdialogex.h"
#include "SimpleApp.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCEFDlgMFCAppDlg 对话框



CCEFDlgMFCAppDlg::CCEFDlgMFCAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CEFDLGMFCAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_DeviceDlg = new DeviceManagerDlg();
}

CCEFDlgMFCAppDlg::~CCEFDlgMFCAppDlg()
{
	delete  m_DeviceDlg;
} 


void CCEFDlgMFCAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCEFDlgMFCAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCEFDlgMFCAppDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCEFDlgMFCAppDlg 消息处理程序

BOOL CCEFDlgMFCAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		//bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		//ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	m_MainRect.left = 0;
	m_MainRect.top = 0;
	m_MainRect.right = m_MainRect.left + cx;
	m_MainRect.bottom = m_MainRect.top + cy + 25;
	MoveWindow(0, 0, m_MainRect.Width(), m_MainRect.Height());

	CefSettings settings;
	CefSettingsTraits::init(&settings);
	settings.log_severity = cef_log_severity_t::LOGSEVERITY_VERBOSE;
	settings.multi_threaded_message_loop = true;
	settings.single_process = false;

	CefString(&settings.locale) = CefString("zh-CN");
	CefString(&settings.cache_path) = CefString("C:/cefLocalStorage");

	CefEnableHighDPISupport();
	void* sandbox_info = NULL;

	CefRefPtr<CSimpleApp> app;
	app = new CSimpleApp();

	CefMainArgs main_args;
	main_args = CefMainArgs(theApp.m_hInstance);

	int exit_code = CefExecuteProcess(main_args, app.get(), sandbox_info);
	if (exit_code >= 0)
	{
		return exit_code;
	}

#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif
	//CString csUrl = _T("file:///D:/gsoft/GiMAP/BFICPM/app/index.html";);
	CString csUrl = _T("http://www.baidu.com";);
	CefInitialize(main_args, settings, app.get(), NULL);

	CefBrowserSettings browser_settings;
	CefRefPtr<CSimpleClient> handler1(new CSimpleClient());
	m_SimpleClient = handler1;
	// Information used when creating the native window.
	CefWindowInfo window_info;
	window_info.SetAsChild(m_hWnd, m_MainRect);
	CefBrowserHost::CreateBrowser(window_info, handler1, CefString(csUrl), browser_settings, NULL);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_SimpleClient->SetDataObserver(this);




	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCEFDlgMFCAppDlg::global_GJavascriptToCplus(LPCTSTR strMessage)
{
	string strMsg = CW2A(strMessage);
	//GStoneMsgOperate(strMsg);
}


void CCEFDlgMFCAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCEFDlgMFCAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCEFDlgMFCAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCEFDlgMFCAppDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	CRect m_Rect;
	m_Rect.left = 20;
	m_Rect.top = 0;
	m_Rect.right = m_Rect.left + cx;
	m_Rect.bottom = m_Rect.top + cy + 25;
	//MoveWindow(0, 0, m_MainRect.Width(), m_MainRect.Height());

	//CRect rectWndPos(m_Rect.left, m_Rect.top, m_Rect.right - m_Rect.left, m_Rect.bottom - m_Rect.top);
	m_DeviceDlg->SetRect(m_Rect);

	m_DeviceDlg->MoveWindow(m_Rect.left, m_Rect.top, m_Rect.right - m_Rect.left, m_Rect.bottom - m_Rect.top);
	
	m_DeviceDlg->Create(IDD_DLG_DEVICEMANAGER); //创建一个非模态对话框  
	m_DeviceDlg->ShowWindow(SW_SHOW);
	//m_DeviceDlg->ShowWindow(SW_SHOWNORMAL);
}
