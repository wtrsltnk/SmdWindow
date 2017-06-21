#ifndef CMAINFRAME_H
#define CMAINFRAME_H

#include "CWin.h"
#include "COpenGl.h"
#include "GLData.h"
#include "..\resource\resource.h"

#include "CCommonDialogs.h"

class CMainFrame : public CWin
{
private:
	COpenGl			*m_pOpengl;
	COpenGl			*m_pOpengl1;
	COpenGl			*m_pOpengl2;
	COpenGl			*m_pOpengl3;
	CGLData			m_Data;

	CCommonDialogs	*m_CommonDlg;


public:
	CMainFrame();
	~CMainFrame();

	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
};

#endif
