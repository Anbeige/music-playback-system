// music playback system.h : PROJECT_NAME Application's main header file
//

#pragma once

#ifndef __AFXWIN_H__
#error "Include "stdafx.h" before including this file to generate a PCH file"
#endif

#include "resource.h"		// primary symbol


// CMPSApp: 
// For an implementation of this class, see music playback system.cpp
//

class CMPSApp : public CWinApp
{
public:
	CMPSApp();

	// rewrite
public:
	virtual BOOL InitInstance();

	// accomplish

	DECLARE_MESSAGE_MAP()
};

extern CMPSApp theApp;
