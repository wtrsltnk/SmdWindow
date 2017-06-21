#include "GLWindow.h"

//////////////////////////////////////////////////////////////////
// Koppeling WIN32 -> Klasse
//////////////////////////////////////////////////////////////////
LRESULT CALLBACK GLProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	GLWindow *wnd = 0; 

	if(uMsg == WM_NCCREATE) 
	{ 
		// retrieve Window instance from window creation data and associate 
		wnd = reinterpret_cast<GLWindow *>(((LPCREATESTRUCT)lParam)->lpCreateParams); 
		::SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<long>(wnd));

		wnd->SetHWND( hWnd );
	} 
	else 
		// retrieve associated Window instance 
		wnd = reinterpret_cast<GLWindow *>(::GetWindowLong(hWnd, GWL_USERDATA)); 

	// call the windows message handler 
	return wnd->ClassProc(hWnd, uMsg, wParam, lParam); 
}

LRESULT GLWindow::ClassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

GLWindow::GLWindow( HINSTANCE hInstance, HWND hParent, int nX, int nY, int nWidth, int nHeight, int nId)
{
	this->m_nId				 = nId;
	this->m_hInstance		 = hInstance;
	this->m_hParent			 = hParent;
	this->m_nX				 = nX;
	this->m_nY				 = nY;
	this->m_nWidth			 = nWidth;
	this->m_nHeight			 = nHeight;

	this->m_hWnd			 = NULL;
	this->m_dwWindowStyle	 = WS_VISIBLE | WS_CHILD | WS_BORDER;
	this->m_dwExWindowStyle	 = WS_EX_STATICEDGE;
	this->m_dwCreationFlags  = SW_SHOW;
	this->m_hbrWindowColor	 = (HBRUSH)(COLOR_WINDOW);
	this->m_hIcon			 = LoadIcon( m_hInstance, (LPCTSTR)IDI_APPLICATION );
	this->m_strWindowTitle	 = "Title";
	this->m_hHandCur		 = LoadCursorFromFile( "hand.cur" );
	this->m_bFocus			 = TRUE;

	WNDCLASS	wc;						// Windows Class Structure
	
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC) GLProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
	wc.hIcon			= m_hIcon;
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground	= m_hbrWindowColor;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "GLWindow";

	RegisterClass(&wc);

	m_hViewMenu = CreatePopupMenu();
	AppendMenu(m_hViewMenu, MF_STRING, VIEWMENU_PERSPECTIVE,	"Perspective view");
	AppendMenu(m_hViewMenu, MF_SEPARATOR,						0,0);
	AppendMenu(m_hViewMenu, MF_STRING, VIEWMENU_FRONT,			"Front view");
	AppendMenu(m_hViewMenu, MF_STRING, VIEWMENU_BACK,			"Back view");
	AppendMenu(m_hViewMenu, MF_SEPARATOR,						0,0);
	AppendMenu(m_hViewMenu, MF_STRING, VIEWMENU_LEFT,			"Left view");
	AppendMenu(m_hViewMenu, MF_STRING, VIEWMENU_RIGHT,			"Right view");
	AppendMenu(m_hViewMenu, MF_SEPARATOR,						0,0);
	AppendMenu(m_hViewMenu, MF_STRING, VIEWMENU_TOP,			"Top view");
	AppendMenu(m_hViewMenu, MF_STRING, VIEWMENU_DOWN,			"Down view");

}

GLWindow::~GLWindow()
{
    kill();
}

HWND GLWindow::create()
{
	int		PixelFormat;

	if (!(m_hWnd=CreateWindowEx(	this->m_dwExWindowStyle,
								"GLWindow",
								"",
								this->m_dwWindowStyle |
								WS_CLIPSIBLINGS |
								WS_CLIPCHILDREN,
								m_nX, m_nY,
								m_nWidth,
								m_nHeight,
								m_hParent,
								NULL,
								m_hInstance,
								this)))
	{
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		kill();
		return FALSE;
	}

	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		16,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	
	if (!(m_hDC=GetDC(m_hWnd)))
	{
		kill();
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(PixelFormat=ChoosePixelFormat(m_hDC,&pfd)))
	{
		kill();
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if(!SetPixelFormat(m_hDC,PixelFormat,&pfd))
	{
		kill();
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(m_hRC=wglCreateContext(m_hDC)))
	{
		kill();
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if(!wglMakeCurrent(m_hDC,m_hRC))
	{
		kill();
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	ShowWindow(m_hWnd,SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	create_label();

    SendMessage( m_hWnd, WM_INITDIALOG, 0, 0 );

	return m_hWnd;
}

HWND GLWindow::create_label()
{
	m_hLabel = CreateWindowEx( WS_EX_DLGMODALFRAME,
		 "STATIC", 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE |
		 SS_NOPREFIX, 
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 12, 
		 m_hParent, 
		 (HMENU)0, 
		 NULL, 
		 NULL );

	m_hNormal = CreateFont( 
					12, 0, 0, 0, 
					FW_NORMAL, 
					FALSE, FALSE, FALSE, 
					DEFAULT_CHARSET, 
					OUT_DEFAULT_PRECIS, 
					CLIP_DEFAULT_PRECIS, 
					DEFAULT_QUALITY, 
					DEFAULT_PITCH, 
					"Verdana" );

	m_hBold = CreateFont( 
					12, 0, 0, 0, 
					FW_BOLD, 
					FALSE, FALSE, FALSE, 
					DEFAULT_CHARSET, 
					OUT_DEFAULT_PRECIS, 
					CLIP_DEFAULT_PRECIS, 
					DEFAULT_QUALITY, 
					DEFAULT_PITCH, 
					"Verdana" );
	return m_hLabel;
}

void GLWindow::kill()
{
	if (m_hRC)
	{
		if (!wglMakeCurrent(NULL,NULL))
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(m_hRC))
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		m_hRC=NULL;
	}

	if (m_hDC && !ReleaseDC(m_hWnd,m_hDC))
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hDC=NULL;
	}

	if (m_hWnd && !DestroyWindow(m_hWnd))
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hWnd=NULL;
	}

	if (!UnregisterClass("OpenGL",m_hInstance))
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hInstance=NULL;
	}
}

void GLWindow::resize( int nX, int nY, int nWidth, int nHeight )
{
	this->m_nX = nX;
	this->m_nY = nY;
	this->m_nWidth = nWidth;
	this->m_nHeight = nHeight;

	MoveWindow( m_hWnd, m_nX, m_nY+18, m_nWidth, m_nHeight-18, true );
	resize_label();
	
    wglMakeCurrent( m_hDC, m_hRC );

	glViewport(0, 0, m_nWidth, m_nHeight);
}

void GLWindow::resize_label()
{
	MoveWindow( m_hLabel, m_nX, m_nY, m_nWidth, 18, true );
}
