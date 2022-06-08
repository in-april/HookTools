#pragma once


// CDialogAddModule 对话框

class CDialogAddModule : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogAddModule)

public:
	CDialogAddModule(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogAddModule();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD_MODULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit_path;
	CEdit m_edit_name;
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnSave();
};
