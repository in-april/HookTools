// CDialogAttach.cpp: 实现文件
//

#include "pch.h"
#include "InjectTools.h"
#include "DialogAttach.h"
#include "afxdialogex.h"
#include "ProcTool.h"


// CDialogAttach 对话框

IMPLEMENT_DYNAMIC(CDialogAttach, CDialogEx)

CDialogAttach::CDialogAttach(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ATTACH, pParent)
{

}

CDialogAttach::~CDialogAttach()
{
}

void CDialogAttach::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_processList);
}


BEGIN_MESSAGE_MAP(CDialogAttach, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDialogAttach::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDialogAttach::OnNMRClickList1)
	ON_COMMAND(ID_ROOT_32771, &CDialogAttach::OnMenuRefresh)
	ON_COMMAND(ID_ROOT_32772, &CDialogAttach::OnMenuSelect)
END_MESSAGE_MAP()


// CDialogAttach 消息处理程序


BOOL CDialogAttach::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化list control
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(m_processList.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;

	SetWindowLongPtr(m_processList.m_hWnd, GWL_STYLE, lStyle);

	DWORD dStyle = m_processList.GetExtendedStyle();
	//设置整行选中
	dStyle |= LVS_EX_FULLROWSELECT;
	//显示网格线
	dStyle |= LVS_EX_GRIDLINES;
	m_processList.SetExtendedStyle(dStyle);

	//设置列表头
	m_processList.InsertColumn(0, "序号", 0, 70);
	m_processList.InsertColumn(1, "PID", 0, 70);
	m_processList.InsertColumn(2, "程序名", 0, 300);

	ShowProcList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDialogAttach::ShowProcList()
{
	m_processList.DeleteAllItems();
	std::vector<ProcessInfo> procList = CProcTool::GetProcessList();
	int size = procList.size();
	for (int i = 0; i < size; i++)
	{
		m_processList.InsertItem(i, std::to_string(i + 1).c_str());
		m_processList.SetItemText(i, 1, std::to_string(procList[i].pid).c_str());
		m_processList.SetItemText(i, 2, procList[i].procName.c_str());
	}
}


void CDialogAttach::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem != -1)
	{
		listSelect = pNMItemActivate->iItem;
		m_pid = m_processList.GetItemText(listSelect, 1);
		CDialog::OnOK();
	}
}


void CDialogAttach::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	listSelect = pNMItemActivate->iItem;
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	CMenu* popMenu = menu.GetSubMenu(0);
	if (pNMItemActivate->iItem == -1)
	{
		popMenu->DeleteMenu(1, MF_BYPOSITION);
	}
	popMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}


void CDialogAttach::OnMenuRefresh()
{
	// TODO: 在此添加命令处理程序代码
	ShowProcList();
}


void CDialogAttach::OnMenuSelect()
{
	// TODO: 在此添加命令处理程序代码
	m_pid = m_processList.GetItemText(listSelect, 1);
	CDialog::OnOK();
}
