#pragma once


// CDialogMain 对话框

class CDialogMain : public CDialog
{
	DECLARE_DYNAMIC(CDialogMain)

public:
	CDialogMain(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogMain();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listFunc;
	CString m_content;
	void ShowFuncList();
	afx_msg void OnNMClickListFunc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnSethook();
};
