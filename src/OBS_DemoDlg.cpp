
// OBS_DemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OBS_Demo.h"
#include "OBS_DemoDlg.h"
#include "afxdialogex.h"

#include"obs.h"

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


// COBS_DemoDlg 对话框



COBS_DemoDlg::COBS_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OBS_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COBS_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COBS_DemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &COBS_DemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COBS_DemoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &COBS_DemoDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// COBS_DemoDlg 消息处理程序

BOOL COBS_DemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COBS_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COBS_DemoDlg::OnPaint()
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
HCURSOR COBS_DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COBS_DemoDlg::monitor_capture()
{
	if (!m_oadapter.Obs_startup("en-US", nullptr))
	{
		throw "Obs_startup failed.";
	}
	
	m_oadapter.Obs_load_all_modules();
	
	if (!m_oadapter.Obs_reset_video(GetDlgItem(IDC_STATIC)->m_hWnd))
	{
		throw "Obs_reset_video failed.";
	}
	if (!m_oadapter.Obs_create_source("monitor_capture"))
	{
		throw "Obs_create_source failed";
	}
	if (!m_oadapter.Obs_create_scene("test scene"))
	{
		throw "Obs_create_scene failed.";
	}

	if (!m_oadapter.Obs_add_raw_video_callback())
	{
		throw "Obs_add_raw_video_callback failed.";
	}

	if (!m_oadapter.Obs_start_capture())
	{
		throw "Obs_start_capture failed.";
	}
}

//monitor capture start
void COBS_DemoDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	monitor_capture();
}

//picture start
void COBS_DemoDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

}

//handle capture start
void COBS_DemoDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
}
