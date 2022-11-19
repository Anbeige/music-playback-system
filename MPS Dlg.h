// MPS Dlg.h : HEAD FILE
//

#pragma once
#include<mmsystem.h>//Multimedia Device Interface
#include<Digitalv.h>//handle volume
#include "afxcmn.h"
#pragma comment(lib,"Winmm.lib")//Link static library files


// CMPS Dlg dialog box
class CMPS Dlg : public CDialogEx
{
	// structure
public:
	CMPS Dlg(CWnd* pParent = NULL);	// standard constructor

	// dialog data
	enum { IDD = IDD_MY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// accomplish
protected:
	HICON m_hIcon;
	MCIDEVICEID m_DeviceID;//record music

	// The generated message map function
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	CSliderCtrl m_slider;
	int m_nVolume;
	afx_msg void OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
};
