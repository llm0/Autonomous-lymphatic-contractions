// AnimateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Animate.h"
#include "AnimateDlg.h"
#include "Common.h"
#include "InputStepDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
UINT CAnimateDlg::m_Update_Msg;
/////////////////////////////////////////////////////////////////////////////
// CAnimateDlg dialog

CAnimateDlg::CAnimateDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAnimateDlg::IDD, pParent)
, m_ReverCount(FALSE)
, m_SaveCa(FALSE)
{
	//{{AFX_DATA_INIT(CAnimateDlg)
	m_File = FileStr;
	m_Time = 50;
	m_Skip = 0;
	iSkip = m_Skip;
	m_Cycle = TRUE;
	iLine = 2;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAnimateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimateDlg)
	DDX_Control(pDX, IDC_TIME_SLIDER, m_TimeCtrl);
	DDX_Control(pDX, IDC_SKIP_SLIDER, m_SkipCtrl);
	DDX_Text(pDX, IDC_FILE_EDIT, m_File);
	DDX_Slider(pDX, IDC_TIME_SLIDER, m_Time);
	DDX_Slider(pDX, IDC_SKIP_SLIDER, m_Skip);
	DDX_Check(pDX, IDC_CYCLE_CHECK, m_Cycle);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_REVCOU_CHECK, m_ReverCount);
	DDX_Check(pDX, IDC_CHECK_SAVECA, m_SaveCa);
}

BEGIN_MESSAGE_MAP(CAnimateDlg, CDialog)
	//{{AFX_MSG_MAP(CAnimateDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DRAW_BUTTON, OnDrawButton)
	ON_BN_CLICKED(IDC_FILE_BUTTON, OnFileButton)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_TIME_SLIDER, OnReleasedcaptureTimeSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SKIP_SLIDER, OnReleasedcaptureSkipSlider)
	ON_BN_CLICKED(IDC_CYCLE_CHECK, OnCycleCheck)
	ON_BN_CLICKED(IDC_PAUSE_BUTTON, OnPauseButton)
	ON_REGISTERED_MESSAGE(m_Update_Msg, OnUpdateNow)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CUT_BUTTON, &CAnimateDlg::OnBnClickedCutButton)
	ON_BN_CLICKED(IDC_REVCOU_CHECK, &CAnimateDlg::OnBnClickedRevcouCheck)
	ON_BN_CLICKED(IDC_CHECK_SAVECA, &CAnimateDlg::OnBnClickedCheckSaveca)
	ON_BN_CLICKED(IDCANCEL, &CAnimateDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimateDlg message handlers

BOOL CAnimateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	pParent = this;
	pAniDlg = this;
	lPosition = new __int64[lNumber];
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAnimateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAnimateDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

void CAnimateDlg::OnDrawButton()
{
	int im;
	DWORD dwSizeLow = 0;
	DWORD dwSizeHigh = 0;
	CDC* pDC;
	CString strText;
	CFile cf;
	CFileStatus status;
	CRect rect1;
	CBrush BlueBrush(RGB(255, 255, 255));
	UpdateData(TRUE);
	bReverCount = m_ReverCount;
	bSaveCa = m_SaveCa;
	sre = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	iStep = 0;
	iTime = m_Time;
	FileStr = m_File;
	CRect rect;
	rect.left = -8;
	rect.top = 10;
	rect.right = 2500;//1382;
	rect.bottom = 1500;// 460;
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFOHEADER));
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = 0;//BI_RGB;  
	bmpInfo.bmiHeader.biSizeImage = 0;
	bmpInfo.bmiHeader.biXPelsPerMeter = 3000;
	bmpInfo.bmiHeader.biYPelsPerMeter = 3000;
	bmpInfo.bmiHeader.biClrUsed = 0;
	bmpInfo.bmiHeader.biClrImportant = 0;
	if (pDrawWnd == NULL)
	{

		FileIn.open((LPCTSTR)FileStr, ios::in | ios::binary);
		pDrawWnd->InitData(&FileIn); 
		FileR.open((LPCTSTR)(FileName + "\\R.txt"), ios::out);
		if (FileIn.is_open())
		{
			GetDlgItem(IDC_FILE_BUTTON)->EnableWindow(FALSE);
			pDrawWnd = new MapWindow;
		}
		else
		{
			return;
		}
		pDrawWnd->CreateEx(0, AfxRegisterWndClass(
			CS_HREDRAW | CS_VREDRAW, 0, (HBRUSH)(GrayBsh.m_hObject)),
			"画图窗口", WS_OVERLAPPEDWINDOW,
			rect, NULL, 0);
		pDrawWnd->ShowWindow(SW_SHOW);
		pDrawWnd->GetClientRect(&rect1);
		pDrawWnd->SetWindowPos(&wndTop, rect.left, rect.top, rect.right, rect.bottom,
			SWP_DRAWFRAME);
		pDC = pDrawWnd->GetDC();
		pDrawWnd->GetClientRect(&MapRect);
		MapRect.bottom = MapRect.bottom - 40;//减少40给状态条
		DrawRect = MapRect;
		ih = int(double(MapRect.right - MapRect.left) / WIDTH*HEIGHT);
		/*if(MapRect.right- MapRect.left>(MapRect.bottom- MapRect.top)/double(HEIGHT)*WIDTH)
		{
		MapRect.right=int(MapRect.left+(MapRect.bottom- MapRect.top)/double(HEIGHT)*WIDTH);
		}
		else
		{
		MapRect.bottom=int(MapRect.top+(MapRect.right- MapRect.left)/double(WIDTH)*HEIGHT);
		}*/
		bmpInfo.bmiHeader.biWidth = DrawRect.right - DrawRect.left + 1;//(MapRect.right- MapRect.left+1);  
		bmpInfo.bmiHeader.biHeight = DrawRect.bottom - DrawRect.top + 1;//(MapRect.bottom- MapRect.top+1);  
		pDrawWnd->m_DrawDC.CreateCompatibleDC(pDC);
		pDrawWnd->m_hBmp = CreateDIBSection(*pDC, &bmpInfo, DIB_RGB_COLORS, (void **)&pData, NULL, 0);
		if (pData == NULL) Show(1, "a");
		pDrawWnd->m_DrawDC.SelectObject(pDrawWnd->m_hBmp);
		/*pDrawWnd->m_DrawDC.BitBlt(0,0,
					MapRect.right-MapRect.left+1,MapRect.bottom-MapRect.top+1
					,pDC,0,0,SRCCOPY);*/
		pDrawWnd->m_DrawDC.BitBlt(0, 0,
			DrawRect.right - DrawRect.left + 1, DrawRect.bottom - DrawRect.top + 1
			, pDC, 0, 0, SRCCOPY);
		nLnBytes = ((DrawRect.right - DrawRect.left + 2) * 3) / 4 * 4;
		pDrawWnd->m_DrawDC.SelectObject(&RedBsh);
		pDrawWnd->m_DrawDC.SetTextColor(RGB(100, 200, 255));
		pDrawWnd->m_DrawDC.SetBkColor(RGB(0, 150, 150));
		SetDlgItemText(IDC_DRAW_BUTTON, "关闭绘图");
		ReleaseDC(pDC); pDrawWnd->ReadForAnimate(&FileIn);
		if (m_Time <= 50)
		{
			im = 100 - m_Time * 2;
			iSkip = 1;
		}
		else
		{
			im = 0;
			iSkip = (m_Time - 50) / 5;
		}
		pDrawWnd->SetTimer(1001, (im)* 2 + 1, NULL);
		bDrawing = TRUE;
		GetDlgItem(IDC_PAUSE_BUTTON)->EnableWindow(TRUE);
	}
	else
	{
		pDrawWnd->SendMessage(WM_CLOSE);
		GetDlgItem(IDC_PAUSE_BUTTON)->EnableWindow(FALSE);
		SetDlgItemText(IDC_PAUSE_BUTTON, "暂停");
	}
	BlueBrush.DeleteObject();
}

void CAnimateDlg::OnFileButton()
{
	int i, iPos;
	CString tempstr, tempstr1;
	char chrIdentity;
	if (pDrawWnd == NULL)
	{
		CFileDialog FileDlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT, Filter, this);
		fstream dirfile;
		dirfile.open((LPCTSTR)CurPath, ios::in);
		if (dirfile.is_open())
		{
			dirfile >> strDir;
			dirfile.close();
		}
		FileDlg.m_ofn.hwndOwner = m_hWnd;
		FileDlg.m_ofn.lpstrFilter = "数据文件\0*.txt\0所有文件\0*.*\0";
		FileDlg.m_ofn.nMaxFile = sizeof(szFile);
		FileDlg.m_ofn.lpstrFile = szFile;
		FileDlg.m_ofn.lpstrFileTitle = szFileTitle;
		FileDlg.m_ofn.nMaxFileTitle = sizeof(szFileTitle);
		FileDlg.m_ofn.Flags = OFN_ENABLEHOOK | OFN_LONGNAMES;
		FileDlg.m_ofn.lpstrInitialDir = strDir;;
		if (FileDlg.DoModal() == IDOK)
		{
			m_File = FileDlg.GetPathName();
			FileName = m_File;
			iPos = FileName.ReverseFind('\\');
			tempstr1 = FileName.Right(FileName.GetLength() - iPos);
			tempstr = tempstr1.SpanExcluding("0123456789");
			tempstr = tempstr1.Right(tempstr1.GetLength() - tempstr.GetLength());
			tempstr = tempstr.SpanIncluding("0123456789");
			iUnit = _ttoi(tempstr);
			if (iUnit == 0) iUnit = 1;
			if (pAniData != NULL)
			{
				delete []pAniData;
			}
			pAniData = new AnimateData[iUnit]; 
			dirfile.open((LPCTSTR)CurPath, ios::out);
			if (dirfile.is_open())
			{
				FileName = FileName.Left(FileName.ReverseFind('\\'));
				dirfile << FileName;
				dirfile.close();
			}
			UpdateData(FALSE);
		}
		if (pPress != NULL)
		{
			delete pPress;
			delete pPressLumen;
			delete rMax;
		}
		pPress = new double[iUnit*WIDTH];
		pPressLumen = new double[iUnit*WIDTH];
		rMax = new double[iUnit*WIDTH];
		for (i = 0; i < iUnit*WIDTH; i++)
		{
			pPress[i] = 0;
			pPressLumen[i] = 0;
			rMax[i] = 0;
		}
	}
	else
	{
		fstream file;
		file.open((LPCTSTR)(FileName + "\\pp.dat"), ios::out);
		file.precision(18);
		for (i = 0; i < iParStyle; i++)
		{
			for (;;)
			{
				do
				{
					chrIdentity = FileIn.peek();
					if (chrIdentity == 32 || chrIdentity == 'R' || chrIdentity == '\n'
						|| chrIdentity == '	')
					{
						FileIn.read(&chrIdentity, sizeof(chrIdentity));
					}
				} while (chrIdentity == 32 || chrIdentity == '	');
				if (chrIdentity == 'R' || chrIdentity == '\n' || FileIn.eof())
				{
					break;
				}
			}//for(;
			if (chrIdentity == '\n') break;
			if (FileIn.eof())
			{
				break;
			}
		}//i
		file.close();
	}
}

void CAnimateDlg::OnReleasedcaptureTimeSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int im;
	if (m_Time <= 50)
	{
		im = 100 - m_Time * 2;
		iSkip = 1;
	}
	else
	{
		im = 0;
		iSkip = (m_Time - 50) / 5;
	}
	if (pDrawWnd != NULL)
	{
		pDrawWnd->SetTimer(1001, (im)* 2 + 1, NULL);
	}
	*pResult = 0;
}

void CAnimateDlg::OnReleasedcaptureSkipSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);
	iposition = m_Skip;
	*pResult = 0;
	if (pDrawWnd != NULL)
	{
		lOldTime = 0;
		pDrawWnd->MoveTo(&FileIn);
	}
}

void CAnimateDlg::OnCycleCheck()
{
	UpdateData(TRUE);
	if (m_Cycle)
	{
		iLine = 2;
	}
	else
	{
		iLine = -1;
	}
}

void CAnimateDlg::OnPauseButton()
{
	if (pDrawWnd != NULL)
	{
		if (bDrawing)
		{
			pDrawWnd->KillTimer(1001);
			bDrawing = FALSE;
			SetDlgItemText(IDC_PAUSE_BUTTON, "播放");
			GetDlgItem(IDC_FILE_BUTTON)->EnableWindow(TRUE);
			SetDlgItemText(IDC_FILE_BUTTON, "输出数据");
		}
		else
		{
			UpdateData(TRUE);
			if (pDrawWnd != NULL)
			{
				pDrawWnd->SetTimer(1001, (100 - m_Time) * 2 + 1, NULL);
			}
			bDrawing = TRUE;
			SetDlgItemText(IDC_PAUSE_BUTTON, "暂停");
			GetDlgItem(IDC_FILE_BUTTON)->EnableWindow(FALSE);
			SetDlgItemText(IDC_FILE_BUTTON, "...");
		}
	}
}

LRESULT CAnimateDlg::OnUpdateNow(WPARAM wParam, LPARAM lParam)
{
	return UpdateData(FALSE);
}



void CAnimateDlg::OnBnClickedCutButton()
{
	// TODO:  在此添加控件通知处理程序代码
	InputStepDialog dig;
	if (dig.DoModal() == IDOK)
	{
		lCut = dig.m_step;
	}
}


void CAnimateDlg::OnBnClickedRevcouCheck()
{
	UpdateData(TRUE);
}


void CAnimateDlg::OnBnClickedCheckSaveca()
{
	UpdateData(TRUE);
}


void CAnimateDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	if (pPress != NULL)
	{
		delete pPress;
	}
	if (pPressLumen != NULL)
	{
		delete pPressLumen;
	}
	if (rMax != NULL)
	{
		delete rMax;
	}
	CDialog::OnCancel();
}
