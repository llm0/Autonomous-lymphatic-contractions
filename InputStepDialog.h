#pragma once


// InputStepDialog �Ի���

class InputStepDialog : public CDialogEx
{
	DECLARE_DYNAMIC(InputStepDialog)

public:
	InputStepDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~InputStepDialog();

// �Ի�������
	enum { IDD = IDD_INPUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	long m_step;
};
