#ifndef GLDATA_H
#define GLDATA_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "COpenGL.h"
#include "GLData.h"
#include "data_types.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")


#define VectorCopy(a,b) {(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];}


enum SMD_ERRORS
{
	ALL_OK,
	WRONG_FILE,
	WRONG_LINE,
	WRONG_VERSION,
	END_OF_FILE,
	NULL_POINTER
};
/*
//#define NO_ERROR 0
#define WRONG_FILE 1
#define WRONG_LINE 2
#define WRONG_VERSION 3
#define END_OF_FILE 4
#define NULL_POINTER 5
*/
class CGLData
{
private:
	smd_model* m_pModel;
	bool Loaded;
	char line[1024];
	int linecount;
	char texture[64];
protected:
	// FUNCTIONS
    int AddNode( const char* input, smd_model* pModel );
    int AddBone( const char* input, smd_model* pModel );
	int AddTriangle( smd_mesh* pmesh, smd_triangle* ptriangle );

    smd_mesh* GetMesh( const char* texture, smd_model* pModel );

	int LoadNodes( FILE* pfile, smd_model* pModel );
	int LoadSkeleton( FILE* pfile, smd_model* pModel );
	int LoadTriangles( FILE* pfile, smd_model* pModel );

    int LoadSMD( const char* path, smd_model* pModel );
	
	//COMMANDS
    char* StripString( const char* str );
public:
	CGLData();
	~CGLData();
	
	void Render( VOID* p );
	void RenderBones( VOID* p );
    bool Load( const char* filename );
	bool Unload();
};
#endif
