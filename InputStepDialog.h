#pragma once


// InputStepDialog 对话框

class InputStepDialog : public CDialogEx
{
	DECLARE_DYNAMIC(InputStepDialog)

public:
	InputStepDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~InputStepDialog();

// 对话框数据
	enum { IDD = IDD_INPUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	long m_step;
};
