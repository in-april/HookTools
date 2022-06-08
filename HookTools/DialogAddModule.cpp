// DialogAddModule.cpp: 实现文件
//

#include "pch.h"
#include "InjectTools.h"
#include "DialogAddModule.h"
#include "afxdialogex.h"
#include "Setting.h"

extern CSetting g_setting;
// CDialogAddModule 对话框

IMPLEMENT_DYNAMIC(CDialogAddModule, CDialogEx)

CDialogAddModule::CDialogAddModule(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD_MODULE, pParent)
{
	isEdit = false;
	m_index = 0;
}

CDialogAddModule::~CDialogAddModule()
{
}

void CDialogAddModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit_path);
	DDX_Control(pDX, IDC_EDIT2, m_edit_name);
}


BEGIN_MESSAGE_MAP(CDialogAddModule, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CDialogAddModule::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDialogAddModule::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CDialogAddModule 消息处理程序


void CDialogAddModule::OnBnClickedBtnOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilters[] = _T("动态链接库 (*.dll)|*.dll|All Files (*.*)|*.*|");
	CFileDialog dialog(TRUE, NULL, NULL, 6, szFilters);
	if (dialog.DoModal() == IDOK)
	{
		m_edit_path.SetWindowText(dialog.GetPathName());
		m_edit_name.SetWindowText(dialog.GetFileName());
		UpdateData(FALSE);
	}
}


void CDialogAddModule::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tmp;
	m_edit_path.GetWindowText(tmp);
	path = tmp;
	m_edit_name.GetWindowText(tmp);
	name = tmp;
	if(isEdit)
		g_setting.UpdateModule(m_index, name, path);
	else
		g_setting.AddModule(name, path);
	CDialog::OnOK();
}



BOOL CDialogAddModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_edit_name.SetWindowText(name.c_str());
	m_edit_path.SetWindowText(path.c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
