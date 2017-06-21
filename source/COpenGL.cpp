#include "COpenGL.h"
#include "GLData.h"
#include <windows.h>
#include <assert.h>

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
COpenGl::COpenGl( HINSTANCE hInstance, HWND hParent, int nX, int nY, int nWidth, int nHeight, int nId, CGLData* pData )
: GLWindow(hInstance, hParent, nX, nY, nWidth, nHeight, nId )
{
	this->m_pData			 = pData;

	this->m_bGrid			 = true;
	this->m_bLighting		 = true;
	this->m_bBlending		 = false;
	this->m_bRender			 = true;
	this->m_nMouseMode		 = MOUSEMODE_CAMERA;
}

COpenGl::~COpenGl()
{
	killFont();
}

//////////////////////////////////////////////////////////////////////////
// ClassProc
// Handles all the message send to the OpenGL window
//////////////////////////////////////////////////////////////////////////
LRESULT COpenGl::ClassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	DWORD fwKeys;
	RECT rcCl, rcWin;
	float x, y;
	
	vecf coordinate = {5.0f, 1.0f, 5.0f};
	vecf pos = {0.0f, 0.0f, 0.0f};
	veci rot = {0, 0, 0};
	vecf color = {0.0f, 1.0f, 0.0f};
	
	
	switch ( uMsg )
	{

	case WM_CREATE:
		SetViewMode( VIEWMODE_PERSPECTIVE );
        CheckMenuRadioItem( m_hViewMenu, VIEWMENU_PERSPECTIVE, VIEWMENU_DOWN, VIEWMENU_PERSPECTIVE, 0 );
		break;

	case WM_SIZE:
		break;

		
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		m_bFocus = (uMsg==WM_SETFOCUS);
		::InvalidateRect( hWnd, NULL, FALSE );
		if ( m_bFocus )
			SendMessage( m_hLabel, WM_SETFONT, (WPARAM)m_hBold, (LPARAM)TRUE );
		else
			SendMessage( m_hLabel, WM_SETFONT, (WPARAM)m_hNormal, (LPARAM)TRUE );
		break;

	case WM_INITDIALOG:
		wglMakeCurrent( m_hDC, m_hRC );
		initCamara();
		initWindow();
		initFont();
		SendMessage( m_hLabel, WM_SETFONT, (WPARAM)m_hBold, (LPARAM)TRUE );
		break;

	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONDOWN:
		oldx = LOWORD(lParam);
		oldy = HIWORD(lParam);
		oldpx = panx;
		oldpy = pany;
		oldpz = panz;
		oldrx = rotx;
		oldry = roty;
		oldrz = rotz;
		if ( m_nMouseMode == MOUSEMODE_CAMERA )
		{
			::SetCursor( m_hHandCur );
			::SetFocus( hWnd );
			::GetWindowRect( m_hWnd, &rcWin );
			::GetClientRect( m_hWnd, &rcCl );
			rcCl.left	 +=		rcWin.left	+	2;
			rcCl.right	 +=		rcWin.left	-	2;
			rcCl.top	 +=		rcWin.top	+	2;
			rcCl.bottom	 +=		rcWin.top	-	2;
			::ClipCursor( &rcCl );
		}
/*
		GetCursorPos(&pt);
		TrackPopupMenu( m_hViewMenu, TPM_RIGHTALIGN, pt.x, pt.y, NULL, m_hWnd, NULL );
*/
		break;

	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_LBUTTONUP:
		::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
		::ClipCursor( NULL );
		break;

	case WM_MOUSEMOVE:
		fwKeys = wParam;
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		if ( m_bFocus )
		{
			if ( m_nMouseMode == MOUSEMODE_CAMERA )
			{
				switch (fwKeys)
				{
				case MK_LBUTTON:
					rotx = oldrx - ( oldy - y );
					roty = oldry - ( oldx - x );
					break;
				case MK_LBUTTON|MK_SHIFT:
					rotz = oldrz - ( oldx - x );
					break;

				case MK_RBUTTON:
						panz = oldpz + ( (( oldy - y ) / 10) * (panz/20) );
					if ( panz < -250 )
						panz = -249;
					if ( panz > 0 )
						panz = -0.1f;
					break;
				case MK_RBUTTON|MK_SHIFT:
					panz = oldpz + ( ( oldy - y ) / 100 );
					break;
					
				case MK_MBUTTON:
					panx = oldpx - ( ( oldx - x ) );
					pany = oldpy + ( ( oldy - y ) );
					break;
				case MK_MBUTTON|MK_SHIFT:
					panx = oldpx - ( ( oldx - x ) / 10 );
					pany = oldpy + ( ( oldy - y ) / 10 );
					break;
				default:
					::ClipCursor( NULL );
				}
				if ( fwKeys )
				{
					::SetCursor( m_hHandCur );
					::InvalidateRect( hWnd, NULL, FALSE );
				}
			}
			else if ( m_nMouseMode == MOUSEMODE_SELECTION )
			{
			/*
				::SetROP2(hDC, R2_NOT);
				::SelectObject(hDC, ::GetStockObject(NULL_BRUSH));*/
				
			}
		}
		break;

	case WM_COMMAND:
		switch ( LOWORD(wParam) )
		{
		case VIEWMENU_PERSPECTIVE:
		case VIEWMENU_FRONT:
		case VIEWMENU_BACK:
		case VIEWMENU_LEFT:
		case VIEWMENU_RIGHT:
		case VIEWMENU_TOP:
		case VIEWMENU_DOWN:
			SetViewMode( LOWORD(wParam)-VIEWMENU_PERSPECTIVE );
            CheckMenuRadioItem( m_hViewMenu, VIEWMENU_PERSPECTIVE, VIEWMENU_DOWN, LOWORD(wParam), 0 );
			break;
		}
		::InvalidateRect( hWnd, NULL, FALSE );
		break;

	case WM_KEYDOWN:
		switch ( wParam )
		{
		case 49://VK_1:
			SetRenderMode( RENDERMODE_VERTEX );
			break;
		case 50://VK_2:
			SetRenderMode( RENDERMODE_WIREFRAME );
			break;
		case 51://VK_3:
			SetRenderMode( RENDERMODE_FLATSHADE );
			break;
		case 52://VK_4:
			SetRenderMode( RENDERMODE_SMOOTHSHADE );
			break;
		case 53://VK_5:
			SetRenderMode( RENDERMODE_TEXTURED );
			break;


			
		case 65://A axis
			m_bAxis = !m_bAxis;
			break;
		case 66://B back
			SetViewMode( VIEWMODE_BACK );
			break;
		case 67://C CAMERA MOUSE MODE
			m_nMouseMode = MOUSEMODE_CAMERA;
			break;
		case 68://D under
			SetViewMode( VIEWMODE_DOWN );
			break;
		case 70://F front
			SetViewMode( VIEWMODE_FRONT );
			break;
		case 71://G grid
			m_bGrid = !m_bGrid;
			break;
		case 72://H light
			m_bLighting = !m_bLighting;
			break;
		case 76://L left
			SetViewMode( VIEWMODE_LEFT );
			break;
		case 80://P perspective
			SetViewMode( VIEWMODE_PERSPECTIVE );
			break;
		case 82://R right
			SetViewMode( VIEWMODE_RIGHT );
			break;
		case 83://S SELECTION MOUSE MODE
				m_nMouseMode = MOUSEMODE_SELECTION;
			break;
		case 84://T top
			SetViewMode( VIEWMODE_TOP );
			break;

		case 113: //F2
			GetWindowRect( m_hWnd, &rcWin );
            TrackPopupMenu( m_hViewMenu, TPM_LEFTALIGN, rcWin.left+1, rcWin.top+1, 0, m_hWnd, 0 );
			break;
		}
		::InvalidateRect( hWnd, NULL, FALSE );
		break;

	case WM_PAINT:
	
		drawstart();
		if ( m_bGrid )
			drawgrid();
		if (m_pData)
		{
			glEnable(GL_DEPTH_TEST);
			glColor3f( 1.0f, 1.0f, 1.0f );
			m_pData->Render( (VOID*)this );
			if ( m_nViewmode != VIEWMODE_PERSPECTIVE)
			{
				glPolygonMode ( GL_FRONT_AND_BACK, GL_POINT );
				glColor3f( 0.8f, 0.8f, 0.8f );
				m_pData->Render( (VOID*)this );
			}
		}
		if ( m_bAxis )
			draworigin( pos, rot );
				
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);

		drawtext( 2, 80, color, "px: %2.2f", panx );
		drawtext( 2, 70, color, "py: %2.2f", pany );
		drawtext( 2, 60, color, "pz: %2.2f", panz );
		
		drawtext( 2, 40, color, "rx: %2.2f", rotx );
		drawtext( 2, 30, color, "ry: %2.2f", roty );
		drawtext( 2, 20, color, "rz: %2.2f", rotz );
		drawend();
		
		::ValidateRect( hWnd, NULL );
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

////////////////////////////////////////////////////////////
// Initialize Camara
// Set default values for rotating, translate
////////////////////////////////////////////////////////////
void COpenGl::initCamara( void )
{
	this->rotx				 = 0.0f;
	this->roty				 = 0.0f;
	this->rotz				 = 0.0f;

	this->panx				 = 0.0f;
	this->pany				 = 0.0f;
	this->panz				 = -10.0f;

	this->oldx				 = -1.0f;
	this->oldy				 = -1.0f;

	this->oldrx				 = -1.0f;
	this->oldry				 = -1.0f;
	this->oldrz				 = -1.0f;

	this->oldpx				 = -1.0f;
	this->oldpy				 = -1.0f;
	this->oldpz				 = -1.0f;
}

////////////////////////////////////////////////////////////
// Initialize OpenGL Window
// Set default values for rendermode and background color
////////////////////////////////////////////////////////////
void COpenGl::initWindow( void )
{
	SetRenderMode( RENDERMODE_TEXTURED );
	SetViewMode( VIEWMODE_PERSPECTIVE );

	this->bgcolor[0]		 = 0.0f;
	this->bgcolor[1]		 = 0.0f;
	this->bgcolor[2]		 = 0.0f;

	this->LightAmbient[0]	 = 0.3f;
	this->LightAmbient[1]	 = 0.3f;
	this->LightAmbient[2]	 = 0.3f;
	this->LightAmbient[3]	 = 1.0f;

	this->LightDiffuse[0]	 = 0.05f;
	this->LightDiffuse[1]	 = 0.05f;
	this->LightDiffuse[2]	 = 0.05f;
	this->LightDiffuse[3]	 = 1.0f;

	this->LightSpecular[0]	 = 0.1f;
	this->LightSpecular[1]	 = 0.1f;
	this->LightSpecular[2]	 = 0.1f;
	this->LightSpecular[3]	 = 1.0f;

	this->LightPosition[0]	 = 0.0f;
	this->LightPosition[1]	 = 0.0f;
	this->LightPosition[2]	 = 1.0f;
	this->LightPosition[3]	 = 1.0f;

	this->LightShininess[0]	 = 32.0f;

 
	glEnable( GL_CULL_FACE );
}

////////////////////////////////////////////////////////////
// Initialize font
// Initialize a font for text drawing
////////////////////////////////////////////////////////////
void COpenGl::initFont( void )
{
	m_dGlFont = glGenLists( 255 );

    HFONT verdana = CreateFont( 12, 0, 0, 0, FW_MEDIUM, FALSE,
                    FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS,
                    CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                    FF_DONTCARE | DEFAULT_PITCH, "Verdana" );

    SelectObject( m_hDC, verdana );

    wglUseFontBitmaps( m_hDC, 0, 255, m_dGlFont );
}

////////////////////////////////////////////////////////////
// Kill font
// Removes the created font from the memory
////////////////////////////////////////////////////////////
void COpenGl::killFont( void )
{
	DeleteObject( SelectObject(m_hDC, GetStockObject(DEVICE_DEFAULT_FONT)) );
}

////////////////////////////////////////////////////////////
// Open draw sequence
// Always call this function first before starting to draw
////////////////////////////////////////////////////////////
void COpenGl::drawstart()
{
    wglMakeCurrent( m_hDC, m_hRC );

	glClearColor( bgcolor[0], bgcolor[1], bgcolor[2], 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	SetupLighting();
	SetupRenderMode();
	SetupViewMode();
	
	// MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	
	SetupTranslation();
	SetupBlending();

	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
}

////////////////////////////////////////////////////////////
// Close draw sequence
// Always call this function after drawing
////////////////////////////////////////////////////////////
void COpenGl::drawend()
{
	glPopMatrix();
	SwapBuffers( m_hDC );
}

////////////////////////////////////////////////////////////
// Draw an origin axis
// Draws a red/green/blue origin axis on the given coordinate
////////////////////////////////////////////////////////////
void COpenGl::draworigin( vecf origin, veci rotation )
{	
	GLboolean settings[2];

	settings[0] = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	settings[1] = glIsEnabled(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);

	glLineWidth( 1.4f );

	glPushMatrix();
	
	if ( origin != 0 )
		glTranslatef( origin[0], origin[1], origin[2] );

	if ( rotation != 0 ) {
		glRotatef( (float)rotation[0], 1.0f, 0.0f, 0.0f );
		glRotatef( (float)rotation[1], 0.0f, 1.0f, 0.0f );
		glRotatef( (float)rotation[2], 0.0f, 0.0f, 1.0f );
	}

	glBegin(GL_LINES);

		// X-AS
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0, 0, 0); glVertex3f(8.0f, 0, 0);

		// Y-AS
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0, 0, 0); glVertex3f(0, 8.0f, 0);

		// Z-AS
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0, 0, 0); glVertex3f(0, 0, 8.0f);
	glEnd();

	glPopMatrix();
	if ( settings[0] )
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if ( settings[1] )
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

////////////////////////////////////////////////////////////
// Draw grid
// draws a nice grid, according to the zoom
////////////////////////////////////////////////////////////
void COpenGl::drawgrid()
{
	GLboolean settings[2];

	settings[0] = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	settings[1] = glIsEnabled(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	
	glPushMatrix();
	glLineWidth( 1.0f );

	float stap, begin, eind;
	stap = 8;
	begin = -512;
	eind = 512;

	switch ( this->m_nViewmode )
	{
	case VIEWMODE_FRONT:
	case VIEWMODE_BACK:
		glRotatef( 90,1,0,0 );
		glRotatef( 0,0,1,0 );
		glRotatef( 0,0,0,1 );
		glDisable(GL_DEPTH_TEST);
		break;
	case VIEWMODE_LEFT:
	case VIEWMODE_RIGHT:
		glRotatef( 0,1,0,0 );
		glRotatef( 0,0,1,0 );
		glRotatef( 90,0,0,1 );
		glDisable(GL_DEPTH_TEST);
		break;
	case VIEWMODE_TOP:
	case VIEWMODE_DOWN:
		glRotatef( 0,1,0,0 );
		glRotatef( 90,0,1,0 );
		glRotatef( 0,0,0,1 );
		glDisable(GL_DEPTH_TEST);
		break;
	}

	glScalef( 0.5f, 0.5f, 0.5f );
	for ( float i = begin; i <= eind; i+=stap )
	{
		if ( (int)i%32 == 0 )
			glColor3f(0.3f, 0.3f, 0.3f);
		else
			glColor3f(0.2f, 0.2f, 0.2f);

		glBegin(GL_LINES);
			glVertex3f((float)i, 0.0f, (float)begin);
			glVertex3f((float)i, 0.0f, (float)eind);
		glEnd();

		glBegin(GL_LINES);
			glVertex3f((float)begin, 0.0f, (float)i);
			glVertex3f((float)eind, 0.0f, (float)i);
		glEnd();
	}

	glPopMatrix();
	if ( settings[0] )
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if ( settings[1] )
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

////////////////////////////////////////////////////////////
// Draw cross
// draws a cros to point out a coordinate in the scene
////////////////////////////////////////////////////////////
void COpenGl::drawcross( vecf coordinate )
{
	GLboolean settings[2];

	settings[0] = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	settings[1] = glIsEnabled(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);

	glLineWidth( 1.0f );

	glPushMatrix();
	
	if ( coordinate != 0 )
		glTranslatef( coordinate[0], coordinate[1], coordinate[2] );

	glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1, 0, 0); glVertex3f(1.0f, 0, 0);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0, -1.0f, 0); glVertex3f(0, 1.0f, 0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0, 0, -1.0f); glVertex3f(0, 0, 1.0f);
	glEnd();

	glPopMatrix();
	if ( settings[0] )
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if ( settings[1] )
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

//////////////////////////////////////////////////////////////////////////
// Draw text
// Draw a text at the specified location on the screen in the
// specified color
//////////////////////////////////////////////////////////////////////////
void COpenGl::drawtext( int x, int y, vecf color, const char* txt, ... )
{
	char		text[1024];
	va_list		ap;
	GLboolean settings[2];

	if (txt == NULL)
		return;

	settings[0] = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	settings[1] = glIsEnabled(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);

	va_start(ap, txt);							
	::vsprintf(text, txt, ap);
	va_end(ap);
	
    glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glPushMatrix();

	gluOrtho2D( 0.0, (GLdouble) m_nWidth, 0.0, (GLdouble) m_nHeight );

	glDisable (GL_TEXTURE_2D);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glColor3f( color[0], color[1], color[2] );
    glRasterPos2i( x, y );
    glListBase( m_dGlFont );
    glCallLists( strlen( text ), GL_BYTE, text );
	
	glPopMatrix();
	if ( settings[0] )
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if ( settings[1] )
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

////////////////////////////////////////////////////////////
// Rotate viewpoint
// Call this function to rotate the viewpoint
////////////////////////////////////////////////////////////
void COpenGl::rotate( float fRotX, float fRotY, float fRotZ )
{
	this->rotx += fRotX;
	this->roty += fRotY;
	this->rotz += fRotZ;
}

////////////////////////////////////////////////////////////
// Translate viewpoint
// Call this function to translate the viewpoint
////////////////////////////////////////////////////////////
void COpenGl::move( float fPanX, float fPanY, float fPanZ )
{
	this->panx += fPanX;
	this->pany += fPanY;
	this->panz += fPanZ;
}

////////////////////////////////////////////////////////////
// Setup rendermode
// Modes are RENDERMODE_VERTEX, _WIREFRAME, _FLATSHADE,
// _SMOOTHSHADE & _TEXTURED
////////////////////////////////////////////////////////////
void COpenGl::SetupRenderMode( void )
{
	switch ( m_nRendermode )
	{
	case RENDERMODE_VERTEX:
		glPointSize( 1 );

		glPolygonMode ( GL_FRONT_AND_BACK, GL_POINT );
		glDisable (GL_TEXTURE_2D);
		glShadeModel ( GL_SMOOTH );
		glDisable(GL_LIGHTING);
		break;
	case RENDERMODE_WIREFRAME:
		glPointSize( 3 );
		glLineWidth( 1 );

		glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
		glDisable (GL_TEXTURE_2D);
		glShadeModel ( GL_SMOOTH );
		glDisable( GL_CULL_FACE );
		glDisable( GL_DEPTH_TEST );
		glDisable(GL_LIGHTING);
		break;
	case RENDERMODE_FLATSHADE:

		glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
		glDisable (GL_TEXTURE_2D);
		glShadeModel ( GL_FLAT );
		glEnable(GL_LIGHTING);
		break;
	case RENDERMODE_SMOOTHSHADE:

		glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
		glDisable (GL_TEXTURE_2D);
		glShadeModel ( GL_SMOOTH );
		glEnable(GL_LIGHTING);
		break;
	case RENDERMODE_TEXTURED:

		glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
		glEnable (GL_TEXTURE_2D);
		glShadeModel ( GL_SMOOTH );
		glEnable(GL_LIGHTING);
		break;
	}
}

////////////////////////////////////////////////////////////
// Set rendermode
// Change the rendermode from outside the class
////////////////////////////////////////////////////////////
void COpenGl::SetRenderMode( int nMode )
{
	if ( nMode <= RENDERMODE_TEXTURED && nMode >= RENDERMODE_VERTEX )
	{
		this->m_nRendermode = nMode;
	}
}

////////////////////////////////////////////////////////////
// Setup viewmode
// Setup the viewmode for the next draw
////////////////////////////////////////////////////////////
void COpenGl::SetupViewMode( void )
{
	float ratio = (float)m_nHeight/(float)m_nWidth;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	switch ( this->m_nViewmode )
	{
	case VIEWMODE_PERSPECTIVE:
		gluPerspective(45.0f,(GLfloat)m_nWidth/(GLfloat)m_nHeight,0.01f,300.0f);
		break;

	default:
		glOrtho (-1, 1, -ratio, ratio, -300.0f, 300.0f );
		break;
	}
}

////////////////////////////////////////////////////////////
// Set viewmode
// Change the viewode from outside the class
////////////////////////////////////////////////////////////
void COpenGl::SetViewMode( int nMode )
{
	this->m_nViewmode = nMode;
	initCamara();

	switch ( this->m_nViewmode )
	{
	case VIEWMODE_PERSPECTIVE:
		strcpy(m_pstrTitle, "Perspective view");
		m_bGrid = false;
		SetRenderMode( RENDERMODE_TEXTURED );
		break;
	case VIEWMODE_BACK:
		strcpy(m_pstrTitle, "Back view");
		m_bGrid = true;
		SetRenderMode( RENDERMODE_WIREFRAME );
		break;
	case VIEWMODE_FRONT:
		strcpy(m_pstrTitle, "Front view");
		m_bGrid = true;
		SetRenderMode( RENDERMODE_WIREFRAME );
		break;
	case VIEWMODE_LEFT:
		strcpy(m_pstrTitle, "Left view");
		m_bGrid = true;
		SetRenderMode( RENDERMODE_WIREFRAME );
		break;
	case VIEWMODE_RIGHT:
		strcpy(m_pstrTitle, "Right view");
		m_bGrid = true;
		SetRenderMode( RENDERMODE_WIREFRAME );
		break;
	case VIEWMODE_TOP:
		strcpy(m_pstrTitle, "Top view");
		m_bGrid = true;
		SetRenderMode( RENDERMODE_WIREFRAME );
		break;
	case VIEWMODE_DOWN:
		strcpy(m_pstrTitle, "Down view");
		m_bGrid = true;
		SetRenderMode( RENDERMODE_WIREFRAME );
		break;
	}

	SendMessage( m_hLabel, WM_SETTEXT, 0, (LPARAM)m_pstrTitle );
}

////////////////////////////////////////////////////////////
// Set mousemode
// Change the mouseode from outside the class
////////////////////////////////////////////////////////////
void COpenGl::SetMouseMode( int nMode )
{
	this->m_nMouseMode = nMode;
}

////////////////////////////////////////////////////////////
// Setup Translation
// Setup translations to the projection
////////////////////////////////////////////////////////////
void COpenGl::SetupTranslation( void )
{
	float tempz;
	
	tempz = -panz;

	switch ( this->m_nViewmode )
	{
	case VIEWMODE_PERSPECTIVE:
		glTranslatef( panx, pany, panz );
		glRotatef( rotx,1,0,0 );
		glRotatef( roty,0,1,0 );
		glRotatef( rotz,0,0,1 );
		glScalef( 0.06f, 0.06f, 0.06f );
		break;
	case VIEWMODE_FRONT:
		glRotatef( 0,1,0,0 );
		glRotatef( 0,0,1,0 );
		glRotatef( 0,0,0,1 );

		glScalef( 0.1f/tempz, 0.1f/tempz, 0.1f/tempz );
		glTranslatef( panx*2, pany*2, 0.0f );
		break;
	case VIEWMODE_BACK:
		glRotatef( 0,1,0,0 );
		glRotatef( 180,0,1,0 );
		glRotatef( 0,0,0,1 );

		glScalef( 0.1f/tempz, 0.1f/tempz, 0.1f/tempz );
		glTranslatef( -panx*2, pany*2, 0.0f );
		break;
	case VIEWMODE_LEFT:
		glRotatef( 0,1,0,0 );
		glRotatef( -90,0,1,0 );
		glRotatef( 0,0,0,1 );

		glScalef( 0.1f/tempz, 0.1f/tempz, 0.1f/tempz );
		glTranslatef( 0.0f, pany*2, -panx*2 );
		break;
	case VIEWMODE_RIGHT:
		glRotatef( 0,1,0,0 );
		glRotatef( 90,0,1,0 );
		glRotatef( 0,0,0,1 );

		glScalef( 0.1f/tempz, 0.1f/tempz, 0.1f/tempz );
		glTranslatef( 0.0f, pany*2, panx*2 );
		break;
	case VIEWMODE_TOP:
		glRotatef( 90,1,0,0 );
		glRotatef( 0,0,1,0 );
		glRotatef( 0,0,0,1 );

		glScalef( 0.1f/tempz, 0.1f/tempz, 0.1f/tempz );
		glTranslatef( panx*2, 0.0f, -pany*2 );
		break;
	case VIEWMODE_DOWN:
		glRotatef( -90,1,0,0 );
		glRotatef( 0,0,1,0 );
		glRotatef( 0,0,0,1 );

		glScalef( 0.1f/tempz, 0.1f/tempz, 0.1f/tempz );
		glTranslatef( panx*2, 0.0f, pany*2 );
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Setup Lighting
// Setup lighting with predefined colors and enable them when nessesary
//////////////////////////////////////////////////////////////////////////
void COpenGl::SetupLighting( void )
{
	if ( m_bLighting && m_nViewmode == VIEWMODE_PERSPECTIVE )
	{
		glLightfv( GL_LIGHT1, GL_AMBIENT, LightAmbient );
		glLightfv( GL_LIGHT1, GL_DIFFUSE, LightDiffuse );
		glLightfv( GL_LIGHT1, GL_SPECULAR, LightSpecular );
		glLightfv( GL_LIGHT1, GL_POSITION, LightPosition );
		glEnable( GL_LIGHT1 );

		glMaterialfv(GL_FRONT, GL_SPECULAR, LightSpecular);
		glMaterialfv(GL_FRONT, GL_SHININESS, LightShininess);
		glEnable( GL_LIGHTING );
	}
	else
		glDisable( GL_LIGHTING );
}

//////////////////////////////////////////////////////////////////////////
// Setup Blending
// Setup the opacity of teh objects
//////////////////////////////////////////////////////////////////////////
void COpenGl::SetupBlending( void )
{
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	if ( m_bBlending )
		glEnable( GL_BLEND );
	else
		glDisable( GL_BLEND );
}

//////////////////////////////////////////////////////////////////////////
// Set Error
// Show an Error message in the debug message window
//////////////////////////////////////////////////////////////////////////
void COpenGl::SetError( char *txt, ...)
{
	char		text[1024];
	va_list		ap;

	if (txt == NULL)
		return;

	va_start(ap, txt);							
	::vsprintf(text, txt, ap);
	va_end(ap);

	::OutputDebugString("\n");
	::OutputDebugString(text);
}
