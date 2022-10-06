// InputStepDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Animate.h"
#include "InputStepDialog.h"
#include "afxdialogex.h"


// InputStepDialog �Ի���

IMPLEMENT_DYNAMIC(InputStepDialog, CDialogEx)

InputStepDialog::InputStepDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(InputStepDialog::IDD, pParent)
	, m_step(0)
{

}

InputStepDialog::~InputStepDialog()
{
}

void InputStepDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STEP_EDIT, m_step);
}


BEGIN_MESSAGE_MAP(InputStepDialog, CDialogEx)
END_MESSAGE_MAP()


// InputStepDialog ��Ϣ�������
