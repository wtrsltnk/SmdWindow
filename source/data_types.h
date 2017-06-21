typedef float vec_f;
typedef vec_f vec3_f[3];	// x,y,z
typedef vec_f vec4_f[4];	// x,y,z,w

#define MAX_BONE		128
#define MAX_TRIANGLES	512
#define MAX_TEXTURES	512
#define MAX_MESHES		512


// NODE, de beschrijving van een bone, zonder positie
// en rotatie
typedef struct smd_node
{
	char			name[64];			// Naam van de NODE
	int				index;				// Index of the item
	int				parent;				// Ouder van de NODE in de boom structuur van NODES
	smd_node*		pnext;				// Pointer to the next item in the linked list
	
} smd_node;


// BONE, de beschrijving van een NODE in een bepaalde frame
// met positie en rotatie
typedef struct smd_bone
{
	vec3_f			pos;				// Positie van de NODE
	vec3_f			rot;				// Rotatie van de NODE
	int				index;				// Index of the item
	smd_bone*		pnext;				// Pointer to the next item in the linked list

} smd_bone;


// TEXTURE, de beschrijving van een TEXTURE met bestandsnaam
typedef struct smd_texture
{
	char			name[64];			// Bestandsnaam van de TEXTURE
	int				index;				// Index of the item
	smd_texture*	pnext;				// Pointer to the next item in the linked list

} smd_texture;


// VERTEX, beschrijving van een VERTEX, met de BONE index en
// originele positie van de VERTEX
typedef struct smd_vertex
{
	int				bone;				// BONE waaronder deze vertex valt
	vec3_f			vertpos;			// Originele positie van de vertex
	vec3_f			normpos;			// Originele positie van de normal
	float			u, v;				// UV positie van de VERTEX op de TEXTURE

} smd_vertex;


// TRIANGLE, bescrijving van de TRIANGLE door drie pointers naar
// VERTEXES
typedef struct smd_triangle
{
	smd_vertex		vertexes[3];		// Index van de drie VERTEXES waar de TRIANGLE uit bestaat
	int				index;				// Index of the item
	smd_triangle*	pnext;				// Pointer to the next item in the linked list

} smd_triangle;


// MESH, beschrijving van de MESH met alle bijbehorende TRIANGLES en
// TEXTURE
typedef struct smd_mesh
{
	int				texture;			// Index van de TEXTURE die bij de mesh hoort

	int				numtriangles;		// Aantal TRIANGLES die bij deze mesh horen
	smd_triangle*	start_triangle;		// Pointer naar de eerste TRIANGLES
	smd_triangle*	end_triangle;		// Pointer naar de laatste TRIANGLES

	int				index;				// Index of the item
	smd_mesh*		pnext;				// Pointer to the next item in the linked list
	
} smd_mesh;


// MODEL, beschrijving van een model met verschillende MESHES,
// TEXTURES, NODES en BONES
typedef struct smd_model
{
	int				numtextures;		// Aantal textures in de model
	smd_texture*	start_texture;		// Pointer naar de eerste TEXTURES
	smd_texture*	end_texture;		// Pointer naar de laatste TEXTURES

	int				nummeshes;			// Aantal MESHES in de model
	smd_mesh*		start_mesh;			// Pointer naar de eerste MESHES
	smd_mesh*		end_mesh;			// Pointer naar de laatste MESHES

	int				numnodes;			// Aantal NODES in de model
	smd_node*		start_node;			// Pointer naar de eerste NODES
	smd_node*		end_node;			// Pointer naar de laatste NODES
	smd_bone*		start_bone;			// Pointer naar de eerste BONES
	smd_bone*		end_bone;			// Pointer naar de laatste BONES
	
} smd_model;