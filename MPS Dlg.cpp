// MPS Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "music playback system.h"
#include "MPS Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C MPS Dlg dialog box



CMPS Dlg::CMPS Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMPS Dlg::IDD, pParent)
	, m_nVolume(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMPS Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Text(pDX, IDC_EDIT1, m_nVolume);
}

BEGIN_MESSAGE_MAP(CMPS Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CMPS Dlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_PLAY, &CMPS Dlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_PAUSE, &CMPS Dlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_STOP, &CMPS Dlg::OnBnClickedStop)
	ON_WM_DROPFILES()

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMPS Dlg::OnCustomdrawSlider1)
END_MESSAGE_MAP()


// CMPS Dlg message handler

BOOL CMPS Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Sets the icon for this dialog. The framework will do this automatically when the application main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// set large icons
	SetIcon(m_hIcon, FALSE);		// set small icon

	// TODO:  Add additional initialization code here

	m_slider.SetRange(0, 1000);
	m_slider.SetPos(500);

	return TRUE;  // Returns TRUE unless focus is set to the control
}

// If you add a minimize button to a dialog, you need the code below
// to draw the icon. For MFC applications using document/view models,
// This will be done automatically by the framework.

void CMPS Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // the device context used for drawing

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center the icon in the workspace rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// draw icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		CPaintDC dc(this);
		//Get client width
		CRect rect;
		GetClientRect(&rect);

		//load an image
		CBitmap bmp;
		bmp.LoadBitmap(IDB_MAIN_BJ);


		//Get image size
		BITMAP logBmp;
		bmp.GetBitmap(&logBmp);

		//Create memory DC
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		memDC.SelectObject(&bmp);
		//draw to interface
		//dc.BitBlt(0, 0, rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);
		dc.SetStretchBltMode(HALFTONE);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0,
			logBmp.bmWidth, logBmp.bmHeight, SRCCOPY);
		CDialogEx::OnPaint();
	}
}

//When the user drags the minimized window, the system calls this function to get the cursor
//show.
HCURSOR CMPS Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



////////////////////////////////////////////////////
////OPEN
void CMPS Dlg::OnBnClickedOpen()
{
	// TODO:  Add your control notification handler code here
	CFileDialog dlg(TRUE);
	dlg.DoModal();

	//Get the path of the selected file
	CString strmusicPath = dlg.GetPathName();

	//1. close previous music
//Multimedia device interface (MCI)
	mciSendCommand(m_DeviceID, MCI_CLOSE, 0, 0);


	//2. open music file
	MCI_OPEN_PARMS mciopenparms;
	mciopenparms.lpstrElementName = strmusicPath;
	MCIERROR mciError;
	mciError = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, (DWORD)&mciopenparms);
	if (mciError)//When mciError is true, it means the opening failed
	{
		wchar_t szErrorMsg[256];
		mciGetErrorString(mciError, szErrorMsg, 256);//get error message
		AfxMessageBox(szErrorMsg);//popup error message
		return;
	}
	m_DeviceID = mciopenparms.wDeviceID;//
}


////////////////////////////////////////////////////
////PLAY
void CMPS Dlg::OnBnClickedPlay()
{
	// TODO:  Add your control notification handler code here
	MCI_PLAY_PARMS mcipalyparms;
	mcipalyparms.dwCallback = NULL;
	mcipalyparms.dwFrom = 0;//Play from the beginning
	mciSendCommand(m_DeviceID, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD)&mcipalyparms);
}


////////////////////////////////////////////////////
////Pause/Continue
void CMPS Dlg::OnBnClickedPause()
{
	// TODO:  Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_PAUSE, str);
	if (str == L"Pause")
	{
		mciSendCommand(m_DeviceID, MCI_PAUSE, 0, 0);
		SetDlgItemText(IDC_PAUSE, L"Continue");
	}
	else if (str == L"Continue")
	{
		mciSendCommand(m_DeviceID, MCI_RESUME, 0, 0);
		SetDlgItemText(IDC_PAUSE, L"Pause");
	}
}


////////////////////////////////////////////////////
////stop
void CMPS Dlg::OnBnClickedStop()
{
	// TODO:  Add your control notification handler code here
	mciSendCommand(m_DeviceID, MCI_STOP, 0, 0);
	mciSendCommand(m_DeviceID, MCI_CLOSE, 0, 0);
}



void CMPS Dlg::OnDropFiles(HDROP hDropInfo)
{
	// Get the path of the dragged file
	wchar_t szMusicPath[256];
	DragQueryFile(hDropInfo, 0, szMusicPath, 256);
	//AfxMessageBox(szMusicPath);


	//1. close previous music
//Multimedia device interface (MCI)
	mciSendCommand(m_DeviceID, MCI_CLOSE, 0, 0);


	//2. open music file
	MCI_OPEN_PARMS mciopenparms;
	mciopenparms.lpstrElementName = szMusicPath;
	MCIERROR mciError;
	mciError = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, (DWORD)&mciopenparms);
	if (mciError)//When mciError is true, it means the opening failed
	{
		wchar_t szErrorMsg[256];
		mciGetErrorString(mciError, szErrorMsg, 256);//get error message
		AfxMessageBox(szErrorMsg);//popup error message
		return;
	}
	m_DeviceID = mciopenparms.wDeviceID;//

	//PLAY
	OnBnClickedPlay();

	CDialogEx::OnDropFiles(hDropInfo);
}




void CMPS Dlg::OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  Add your control notification handler code here
// Get the current value of the sliding space
	m_nVolume = m_slider.GetPos();
	UpdateData(FALSE);

	MCI_DGV_SETAUDIO_PARMS mciSetvolume;
	mciSetvolume.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciSetvolume.dwValue = m_nVolume;

	mciSendCommand(m_DeviceID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)&mciSetvolume);

	*pResult = 0;
}