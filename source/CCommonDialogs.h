#ifndef COPENSAVEDLG_H
#define COPENSAVEDLG_H

#include <windows.h>
#include <shlobj.h>
#include <string>
using namespace std;

class CCommonDialogs
{
private:
	HWND				m_hOwner;
	HWND				m_hWnd;
	HINSTANCE			m_hInstance;

public:
	CCommonDialogs( HINSTANCE, HWND );
	~CCommonDialogs( void );

    const char *open( const char *, const char *, const char * );
    const char *save( const char *, const char *, const char * );
    const char *folder( const char *title );
};

#endif
