// InputStepDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Animate.h"
#include "InputStepDialog.h"
#include "afxdialogex.h"


// InputStepDialog 对话框

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


// InputStepDialog 消息处理程序
