#include "CCommonDialogs.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////

CCommonDialogs::CCommonDialogs( HINSTANCE hInstance, HWND hWnd )
{
	this->m_hOwner		= hWnd;
	this->m_hInstance	= hInstance;
}

CCommonDialogs::~CCommonDialogs( void )
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

const char *CCommonDialogs::open( const char *title, const char *szFilter, const char *szDefExt )
{
	char *result;
	OPENFILENAME ofn = {0};
	char szFile[MAX_PATH+1] =	{0};
	
	//specificeer de dialog box zo volledig mogelijk
	ofn.Flags             = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
	ofn.hwndOwner         = m_hOwner;
	ofn.lpstrCustomFilter = 0;
	ofn.lpstrFile         = szFile;
	ofn.lpstrFileTitle    = 0;
	ofn.lpstrFilter       = szFilter;
	ofn.lpstrDefExt		  = szDefExt;
	ofn.lpstrInitialDir   = 0;
	ofn.lpstrTitle        = title;
	ofn.lStructSize       = sizeof( OPENFILENAME );
	ofn.nMaxFile          = MAX_PATH;


	//open de dialoog
	if( GetOpenFileName( &ofn ) != 0 )
	{
		result = new char[strlen( ofn.lpstrFile )+1];
        int i;
        for (i = 0; i < strlen( ofn.lpstrFile ); i++ )
		{
			result[i] = ofn.lpstrFile[i];
			if ( ofn.lpstrFile[i] == '\\' )
				result[i] = '/';
		}
		result[i]='\0';
		return result;
	}
	//als het openen van de dialoog niet gelukt is, geeft een lege string terug
	return NULL;
}

const char *CCommonDialogs::save( const char *title, const char *szFilter, const char *szDefExt )
{
	char *result;
	OPENFILENAME ofn = {0};
	char szFile[MAX_PATH+1] =	{0};
	
	//specificeer de dialog box zo volledig mogelijk
	ofn.Flags             = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
	ofn.hwndOwner         = m_hOwner;
	ofn.lpstrCustomFilter = 0;
	ofn.lpstrFile         = szFile;
	ofn.lpstrFileTitle    = 0;
	ofn.lpstrFilter       = szFilter;
	ofn.lpstrDefExt		  = szDefExt;
	ofn.lpstrInitialDir   = 0;
	ofn.lpstrTitle        = title;
	ofn.lStructSize       = sizeof( OPENFILENAME );
	ofn.nMaxFile          = MAX_PATH;


	//open de dialoog
	if( GetSaveFileName( &ofn ) != 0 )
	{
		result = new char[strlen( ofn.lpstrFile )+1];
        int i;
        for ( i = 0; i < strlen( ofn.lpstrFile ); i++ )
		{
			result[i] = ofn.lpstrFile[i];
			if ( ofn.lpstrFile[i] == '\\' )
				result[i] = '/';
		}
		result[i]='\0';
		return result;
	}
	//als het openen van de dialoog niet gelukt is, geeft een lege string terug
	return NULL;
}

const char *CCommonDialogs::folder( const char *title )
{
	BROWSEINFO bi;
    ITEMIDLIST *pidl;
    char mydir[MAX_PATH];
    char *result;

    bi.hwndOwner = m_hOwner;
    bi.pidlRoot = 0;
    bi.pszDisplayName = mydir;
    bi.lpszTitle = title;
    bi.ulFlags = BIF_RETURNONLYFSDIRS;
    bi.lpfn = 0;
    bi.lParam = 0;

    //Call the directory browse dialog and assign it to a ITEMIDLIST
    pidl=SHBrowseForFolder(&bi);

    //Return the selected path
    if ( SHGetPathFromIDList(pidl, mydir) )
	{
		result = new char[strlen( mydir )+1];
        int i;
        for ( i = 0; i < strlen( mydir ); i++ )
		{
			result[i] = mydir[i];
			if ( mydir[i] == '\\' )
				result[i] = '/';
		}
		result[i]='\0';
		return result;
	}
	
	return NULL;
}

