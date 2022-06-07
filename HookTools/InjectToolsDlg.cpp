
// InjectToolsDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "InjectTools.h"
#include "InjectToolsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "Setting.h"
#include "Inject.h"

CSetting g_setting;

// CInjectToolsDlg 对话框
CInjectToolsDlg::CInjectToolsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_InjectTools_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInjectToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_types);
	DDX_Control(pDX, IDC_LIST3, m_modules);
	DDX_Control(pDX, IDC_EDIT_PATH, m_edit_path);
	DDX_Control(pDX, IDC_EDIT_PROC, m_edit_proc);
	DDX_Control(pDX, IDC_BTN_OPEN, m_btn_open);
	DDX_Control(pDX, IDC_BTN_ATTACH, m_btn_attach);
}

BEGIN_MESSAGE_MAP(CInjectToolsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CInjectToolsDlg::OnCbnSelchangeComboType)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CInjectToolsDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_INJECT, &CInjectToolsDlg::OnBnClickedBtnInject)
END_MESSAGE_MAP()


// CInjectToolsDlg 消息处理程序

BOOL CInjectToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//加载配置文件
	g_setting.Load();
	for (std::string& type : g_setting.m_InjectType)
	{
		m_types.AddString(type.c_str());
	}
	m_types.SetCurSel(0);
	//初始化list control
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(m_modules.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;

	SetWindowLongPtr(m_modules.m_hWnd, GWL_STYLE, lStyle);

	DWORD dStyle = m_modules.GetExtendedStyle();
	//设置整行选中
	dStyle |= LVS_EX_FULLROWSELECT;
	//显示网格线
	dStyle |= LVS_EX_GRIDLINES;
	//设置复选框
	dStyle |= LVS_EX_CHECKBOXES;
	m_modules.SetExtendedStyle(dStyle);

	//设置列表头
	m_modules.InsertColumn(0, "序号", 0, 70);
	m_modules.InsertColumn(1, "模块名称", 0, 150);
	m_modules.InsertColumn(2, "模块路径", 0, 400);

	for (int i = 0; i < g_setting.m_modules.size(); i++)
	{
		m_modules.InsertItem(i, std::to_string(i + 1).c_str());
		m_modules.SetItemText(i, 1, g_setting.m_modules[i].moduleName.c_str());
		m_modules.SetItemText(i, 2, g_setting.m_modules[i].modulePath.c_str());
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInjectToolsDlg::OnPaint()
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
HCURSOR CInjectToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CInjectToolsDlg::OnCbnSelchangeComboType()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_types.GetCurSel();
	switch (nIndex)
	{
	case 0://入口点注入
	{
		m_edit_path.EnableWindow(TRUE);
		m_btn_open.EnableWindow(TRUE);
		m_edit_proc.EnableWindow(FALSE);
		m_btn_attach.EnableWindow(FALSE);
		break;
	}
	case 1://远程线程注入
	{
		m_edit_path.EnableWindow(FALSE);
		m_btn_open.EnableWindow(FALSE);
		m_edit_proc.EnableWindow(TRUE);
		m_btn_attach.EnableWindow(TRUE);
		break;
	}
	default:
		break;
	}
}


void CInjectToolsDlg::OnBnClickedBtnOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilters[] = _T("可执行文件 (*.exe)|*.exe|All Files (*.*)|*.*|");
	CFileDialog dialog(TRUE, NULL, NULL, 6, szFilters);
	if (dialog.DoModal() == IDOK)
	{
		m_edit_path.SetWindowText(dialog.GetPathName());
		UpdateData(FALSE);
	}
}


void CInjectToolsDlg::OnBnClickedBtnInject()
{
	// TODO: 在此添加控件通知处理程序代码
	int len = m_modules.GetItemCount();
	int nIndex = m_types.GetCurSel();
	switch (nIndex)
	{
	case 0: //入口点注入
	{
		for (int i = 0; i < len; i++)
		{
			if (m_modules.GetCheck(i))
			{
				CString path = m_modules.GetItemText(i, 2);
				//注入
				CInject inject;
				CString proc_path;
				m_edit_path.GetWindowTextA(proc_path);
				inject.StartProcess(proc_path.GetBuffer());
				inject.InjectByOEP(path.GetBuffer());
				m_modules.SetCheck(i, false);
			}
		}
	}
	case 1: //远程线程注入
	{
		for (int i = 0; i < len; i++)
		{
			if (m_modules.GetCheck(i))
			{
				CString path = m_modules.GetItemText(i, 2);
				//注入
				CInject inject;
				CString pid;
				m_edit_proc.GetWindowTextA(pid);
				inject.AttachProcess(atoi(pid));
				inject.InjectByRemoteThread(path.GetBuffer());
				m_modules.SetCheck(i, false);
			}
		}
	}
	default:
		break;
	}
	
}
