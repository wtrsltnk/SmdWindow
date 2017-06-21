#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#define VIEWMENU_PERSPECTIVE	0
#define VIEWMENU_FRONT			1
#define VIEWMENU_BACK			2
#define VIEWMENU_LEFT			3
#define VIEWMENU_RIGHT			4
#define VIEWMENU_TOP			5
#define VIEWMENU_DOWN			6

class GLWindow
{
public:
	GLWindow( HINSTANCE hInstance, HWND hParent, int nX, int nY, int nWidth, int nHeight, int nId );
	~GLWindow();
	
	virtual LRESULT ClassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void SetHWND( HWND hWnd ) { m_hWnd = hWnd; };

	HWND create( void );
	HWND create_label( void );
	void resize( int, int, int, int );
	void resize_label( void );
private:
protected:
	void kill();
	
	HWND								m_hWnd, m_hLabel;
	HFONT								m_hNormal, m_hBold;
	HWND								m_hParent;
	HINSTANCE							m_hInstance;
	HMENU								m_hViewMenu;
    HDC									m_hDC;
    HGLRC								m_hRC;
	int									m_nX, m_nY, m_nWidth, m_nHeight;
	HICON								m_hHandCur;
	BOOL								m_bFocus;

	DWORD								m_nId;
	DWORD								m_dwWindowStyle;
	DWORD								m_dwExWindowStyle;
 	DWORD								m_dwCreationFlags;
    LPCTSTR								m_strWindowTitle;
	HBRUSH								m_hbrWindowColor;
	HICON								m_hIcon;
};

#endif
