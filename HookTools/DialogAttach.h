#pragma once


// CDialogAttach 对话框

class CDialogAttach : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogAttach)

public:
	CDialogAttach(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogAttach();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ATTACH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_processList;
	virtual BOOL OnInitDialog();
	void ShowProcList();
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuRefresh();
	afx_msg void OnMenuSelect();

	int listSelect; //右键时选中的条目
	CString m_pid; //选中的pid
};
