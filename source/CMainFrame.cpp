#include "CMainFrame.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CMainFrame::CMainFrame()
{
	this->m_dwCreationFlags  = 0L;
	this->m_dwWindowStyle	 = WS_OVERLAPPEDWINDOW;
	this->m_dwExWindowStyle	 = WS_EX_WINDOWEDGE;
	this->m_dwCreationFlags  = SW_SHOW;
	this->m_PosX			 = CW_USEDEFAULT;	
	this->m_PosY			 = CW_USEDEFAULT;	
	this->m_dwCreationWidth  = CW_USEDEFAULT;
	this->m_dwCreationHeight = CW_USEDEFAULT;
	this->m_hbrWindowColor	 = (HBRUSH)(COLOR_WINDOW+1);
	this->m_hIcon			 = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_CUBE));
	this->m_strWindowTitle	 = "OpenGL Window Class Test applicatie";
	this->m_hMenu			 = LoadMenu(m_hInstance, MAKEINTRESOURCE(IDR_MENU));

	this->m_pOpengl			 = NULL;
	this->m_pOpengl1		 = NULL;
	this->m_pOpengl2		 = NULL;
	this->m_pOpengl3		 = NULL;
}


CMainFrame::~CMainFrame()
{
	if ( m_pOpengl )
	{
		free(m_pOpengl);
		m_pOpengl = NULL;
	}
	if ( m_pOpengl1 )
	{
		free(m_pOpengl1);
		m_pOpengl1 = NULL;
	}
	if ( m_pOpengl2 )
	{
		free(m_pOpengl2);
		m_pOpengl2 = NULL;
	}
	if ( m_pOpengl3 )
	{
		free(m_pOpengl3);
		m_pOpengl3 = NULL;
	}
	if ( m_hIcon )
	{
		DestroyIcon( m_hIcon );
	}
	m_Data.Unload();
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

LRESULT CMainFrame::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch ( uMsg )
	{
	case WM_CREATE:
		m_CommonDlg = new CCommonDialogs( m_hInstance, m_hWnd );
		m_pOpengl = new COpenGl( m_hInstance, m_hWnd, 0, 0, 100, 100, 1, &m_Data );
		m_pOpengl->create();
		m_pOpengl->SetViewMode( VIEWMODE_DOWN );

		m_pOpengl1 = new COpenGl( m_hInstance, m_hWnd, 0, 0, 100, 100, 2, &m_Data );
		m_pOpengl1->create();
		m_pOpengl1->SetViewMode( VIEWMODE_RIGHT );

		m_pOpengl2 = new COpenGl( m_hInstance, m_hWnd, 0, 0, 100, 100, 3, &m_Data );
		m_pOpengl2->create();
		m_pOpengl2->SetViewMode( VIEWMODE_FRONT );

		m_pOpengl3 = new COpenGl( m_hInstance, m_hWnd, 0, 0, 100, 100, 4, &m_Data );
		m_pOpengl3->create();
		m_pOpengl3->SetViewMode( VIEWMODE_PERSPECTIVE );
		break;

	case WM_SIZE:
		m_pOpengl->resize(
			0, 
			0, 
			LOWORD(lParam)/2, 
			HIWORD(lParam)/2 );

		m_pOpengl2->resize( 
			0, 
			HIWORD(lParam)/2, 
			LOWORD(lParam)/2, 
			HIWORD(lParam)/2 );
		
		m_pOpengl1->resize(
			LOWORD(lParam)/2, 
			0, 
			LOWORD(lParam)/2, 
			HIWORD(lParam)/2 );
		
		m_pOpengl3->resize(
			LOWORD(lParam)/2, 
			HIWORD(lParam)/2, 
			LOWORD(lParam)/2, 
			HIWORD(lParam)/2 );
		break;

	case WM_COMMAND:
		switch ( wParam )
		{
		case ID_BESTAND_OPEN:
        {
			m_Data.Unload();
            const char* res = m_CommonDlg->open( "Open SMD file", "SMD file(*.smd)\0*.smd", "smd" );
			if ( res )
			{
				m_Data.Load( res );
				m_pOpengl->InvalidateGL();
				m_pOpengl1->InvalidateGL();
				m_pOpengl2->InvalidateGL();
				m_pOpengl3->InvalidateGL();
			}
			break;
        }
		case ID_BESTAND_SLUITEN:
			m_Data.Unload();
			m_pOpengl->InvalidateGL();
			m_pOpengl1->InvalidateGL();
			m_pOpengl2->InvalidateGL();
			m_pOpengl3->InvalidateGL();
			break;
		case ID_BESTAND_AFSLUITEN:
			PostQuitMessage(0);
			break;
		}
		break;
	}
	return CWin::MsgProc(hWnd, uMsg, wParam, lParam);
}
