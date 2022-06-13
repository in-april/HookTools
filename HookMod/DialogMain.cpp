// DialogMain.cpp: 实现文件
//

#include "pch.h"
#include "HookMod.h"
#include "DialogMain.h"
#include "afxdialogex.h"
#include "FuncData.h"
#include "HookTool.h"
#include "Logger.h"

CFuncData g_funcData;
CHookTool g_hookTool;
CDialogMain* g_dlgMain;
// CDialogMain 对话框

IMPLEMENT_DYNAMIC(CDialogMain, CDialog)

CDialogMain::CDialogMain(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_MAIN, pParent)
	, m_content(_T(""))
{

}

CDialogMain::~CDialogMain()
{
}

void CDialogMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FUNC, m_listFunc);
	DDX_Text(pDX, IDC_EDIT1, m_content);
	DDX_Control(pDX, IDC_EDIT_DATA, m_edit_data);
}


BEGIN_MESSAGE_MAP(CDialogMain, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FUNC, &CDialogMain::OnNMClickListFunc)
	ON_BN_CLICKED(IDC_BTN_SETHOOK, &CDialogMain::OnBnClickedBtnSethook)
	ON_MESSAGE(WM_SHOW_DATA, &CDialogMain::OnShowData)
END_MESSAGE_MAP()


// CDialogMain 消息处理程序


BOOL CDialogMain::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	g_dlgMain = this;

	//初始化list control
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(m_listFunc.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;

	SetWindowLongPtr(m_listFunc.m_hWnd, GWL_STYLE, lStyle);

	DWORD dStyle = m_listFunc.GetExtendedStyle();
	//设置整行选中
	dStyle |= LVS_EX_FULLROWSELECT;
	//显示网格线
	dStyle |= LVS_EX_GRIDLINES;
	//设置复选框
	dStyle |= LVS_EX_CHECKBOXES;
	m_listFunc.SetExtendedStyle(dStyle);

	//设置列表头
	m_listFunc.InsertColumn(0, "", 0, 50); // 记录对象地址，方便修改
	m_listFunc.InsertColumn(1, "序号", 0, 50);
	m_listFunc.InsertColumn(2, "函数名", 0, 100);
	m_listFunc.InsertColumn(3, "函数地址", 0, 100);

	g_funcData.Load();
	ShowFuncList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDialogMain::ShowFuncList()
{
	m_listFunc.DeleteAllItems();
	std::vector<FuncItem*> list = g_funcData.GetFuncList();
	for (int i = 0; i < list.size(); i++)
	{
		m_listFunc.InsertItem(i, std::to_string((DWORD)list[i]).c_str());
		m_listFunc.SetItemText(i, 1, std::to_string(i + 1).c_str());
		m_listFunc.SetItemText(i, 2, list[i]->funcName.c_str());
		CString tmp;
		tmp.Format("0x%08X", list[i]->address);
		m_listFunc.SetItemText(i, 3, tmp);
	}
}


void CDialogMain::OnNMClickListFunc(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int select = pNMItemActivate->iItem;
	if (select == -1) return;

	BOOL bCheckStatus = m_listFunc.GetCheck(select);
	m_listFunc.SetCheck(select, !bCheckStatus);
	CString addrStr = m_listFunc.GetItemText(select, 0);
	FuncItem* addr = (FuncItem*)atoi(addrStr.GetBuffer());
	addr->enable = !bCheckStatus;

}

//设置hook点
void CDialogMain::OnBnClickedBtnSethook()
{
	// TODO: 在此添加控件通知处理程序代码
	std::vector<FuncItem*> list = g_funcData.GetFuncList();
	for (int i = 0; i < list.size(); i++)
	{
		FuncItem* func = list[i];
		if (!func->isSet)
		{
			g_hookTool.SetHookOneByte(func);
			func->isSet = true;
		}
	}
	AfxMessageBox("设置完成");
}

LRESULT CDialogMain::OnShowData(WPARAM wParam, LPARAM lParam)
{
	std::string* pStr = (std::string*)wParam;
	CString curData;
	m_edit_data.GetWindowText(curData);
	curData += pStr->c_str();
	m_edit_data.SetWindowText(curData);
	return LRESULT();
}
