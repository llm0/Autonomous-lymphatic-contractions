; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAnimateDlg
LastTemplate=CSliderCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "animate.h"
LastPage=0

ClassCount=5
Class1=CAnimateApp
Class2=CAnimateDlg
Class3=MapWindow
Class4=SaveDialog

ResourceCount=2
Resource1=IDR_MENU_OUTPUT
Class5=MySliderCtrl
Resource2=IDD_ANIMATE_DIALOG

[CLS:CAnimateApp]
Type=0
BaseClass=CWinApp
HeaderFile=Animate.h
ImplementationFile=Animate.cpp

[CLS:CAnimateDlg]
Type=0
BaseClass=CDialog
HeaderFile=AnimateDlg.h
ImplementationFile=AnimateDlg.cpp
LastObject=IDC_SKIP_SLIDER
Filter=D
VirtualFilter=dWC

[CLS:MapWindow]
Type=0
BaseClass=CWnd
HeaderFile=MapWindow.h
ImplementationFile=MapWindow.cpp

[CLS:SaveDialog]
Type=0
BaseClass=CDialog
HeaderFile=SaveDialog.h
ImplementationFile=SaveDialog.cpp

[DLG:IDD_ANIMATE_DIALOG]
Type=1
Class=CAnimateDlg
ControlCount=11
Control1=IDC_FILE_BUTTON,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_FILE_EDIT,edit,1350631552
Control5=IDC_DRAW_BUTTON,button,1342242816
Control6=IDC_TIME_SLIDER,msctls_trackbar32,1342242840
Control7=IDC_SKIP_SLIDER,msctls_trackbar32,1342242840
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_CYCLE_CHECK,button,1342242819
Control11=IDC_PAUSE_BUTTON,button,1476460544

[DLG:IDD_SAVE_DIALOG]
Type=1
Class=SaveDialog

[MNU:IDR_MENU_OUTPUT]
Type=1
Class=?
Command1=AFX_ID_PAUSE
Command2=AFX_ID_OUTPUT
Command3=ID_SAVENO
CommandCount=3

[CLS:MySliderCtrl]
Type=0
HeaderFile=MySliderCtrl.h
ImplementationFile=MySliderCtrl.cpp
BaseClass=CSliderCtrl
Filter=W
VirtualFilter=KWC
LastObject=MySliderCtrl

