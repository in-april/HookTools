
// InjectToolsDlg.h: 头文件
//

#pragma once


// CInjectToolsDlg 对话框
class CInjectToolsDlg : public CDialogEx
{
// 构造
public:
	CInjectToolsDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_InjectTools_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void ShowModules();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_types;
	CListCtrl m_modules;
	afx_msg void OnCbnSelchangeComboType();
	CEdit m_edit_path;
	CEdit m_edit_proc;
	CButton m_btn_open;
	CButton m_btn_attach;
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnInject();
	afx_msg void OnBnClickedBtnAttach();
	afx_msg void OnBnClickedBtnAddModule();
	afx_msg void OnMenuUpdate();
	afx_msg void OnMenuDel();
	afx_msg void OnNMRClickList3(NMHDR* pNMHDR, LRESULT* pResult);
	int listSelect;
	afx_msg void OnBnClickedBtnQuit();
	virtual void OnOK();
};
