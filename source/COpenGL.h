#ifndef COPENGL_H
#define COPENGL_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

#include <stdio.h>
#include <stdarg.h>

#include "GLWindow.h"
#include "CWin.h"

enum Rendermodes
{
	RENDERMODE_VERTEX,
	RENDERMODE_WIREFRAME,
	RENDERMODE_FLATSHADE,
	RENDERMODE_SMOOTHSHADE,
	RENDERMODE_TEXTURED,
};

enum Viewmodes
{
	VIEWMODE_PERSPECTIVE,
	VIEWMODE_FRONT,
	VIEWMODE_BACK,
	VIEWMODE_LEFT,
	VIEWMODE_RIGHT,
	VIEWMODE_TOP,
	VIEWMODE_DOWN
};

enum Mousemodes
{
	MOUSEMODE_CAMERA,
	MOUSEMODE_SELECTION
};


typedef float vecf[3];
typedef int veci[3];

class COpenGl : public GLWindow
{
	friend class CGLData;
public:

    COpenGl( HINSTANCE hInstance, HWND hParent, int nX, int nY, int nWidth, int nHeight, int nId, class CGLData* pData );
	~COpenGl();

	void drawstart( void );
	void drawend( void );

	void draworigin( vecf origin, veci rotation );
	void drawcross( vecf coordinate );
    void drawtext( int x, int y, vecf color, const char* txt, ... );

	void rotate( float, float, float );
	void move( float, float, float );
	
	LRESULT ClassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    void SetHWND( HWND hWnd ) { m_hWnd = hWnd; }
    void SetGrid( bool bGrid ) { m_bGrid = bGrid; }
    void ToggleGrid( bool bGrid ) { m_bGrid = !m_bGrid; }

	void SetRenderMode( int nMode );
	void SetViewMode( int nMode );
	void SetMouseMode( int nMode );

    void InvalidateGL() { InvalidateRect( m_hWnd, NULL, TRUE ); }

private:
	void SetError( char *txt, ... );

	void initCamara( void );
	void initWindow( void );
	void initFont( void );
	void killFont( void );

	void drawgrid( void );
	
	void SetupRenderMode( void );
	void SetupViewMode( void );
	void SetupTranslation( void );
	void SetupLighting( void );
	void SetupBlending( void );

    class CGLData*                      m_pData;

	float								oldx, oldy;
	float								oldpx, oldpy, oldpz;
	float								panx, pany, panz;
	float								oldrx, oldry, oldrz;
	float								rotx, roty, rotz;

	char								m_pstrTitle[64];
	int									m_nRendermode;
	int									m_nViewmode;
	int									m_nMouseMode;		
	bool								m_bLighting;
	bool								m_bBlending;
	bool								m_bFocus;
	bool								m_bGrid;
	bool								m_bAxis;
	bool								m_bRender;

	float								bgcolor[3];
	float								txtcolor[3];
	int									m_dGlFont;

	float								LightAmbient[4];
	float								LightDiffuse[4];
	float								LightSpecular[4];
	float								LightPosition[4];
	float								LightShininess[1];

};

#endif
