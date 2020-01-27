#ifndef OBJREADER_H
#define OBJREADER_H

#include <vector>
#include <string>
#include "Math.h"

#include "TexturedShader.h"
typedef VertexNormalTextured OBJVertex;


class OBJMaterial
{
public:
	std::wstring Name;
	std::wstring DiffuseMap;
};

class OBJFace
{
public:
	OBJMaterial* Material;
	OBJVertex Vertices[3];
};

class OBJGroup
{
public:
	std::wstring Name;
	std::vector< OBJFace* > Faces;
};

class LineParser;

class OBJReader
{
protected:
	OBJGroup* curGroup;
	OBJMaterial* curMtl;
	bool loaded;

public:
	OBJReader()
	{
		loaded = false;
	}
	std::wstring ObjFolder;

	std::vector< Vector3f > Vertices;
	std::vector< Vector3f > Normals;
	std::vector< Vector2f > TextureCoordinates;
	std::vector< OBJGroup* > Groups;
	std::vector< OBJMaterial* > Materials;
	Vector3f BoundingMax;
	Vector3f BoundingMin;

	void LoadMtl( const std::wstring& path );
	void LoadObj( const std::wstring& path );

	OBJMaterial* GetMaterial( std::wstring& name );

protected:
	void ParseLine( const std::wstring& line );
	void ParseVertexLine( LineParser& ln );
	void ParseTextureLine( LineParser& ln );
	void ParseNormalLine( LineParser& ln );
	void ParseFaceLine( LineParser& ln );
	void ParseGroupLine( LineParser& ln );
	void ParseMtlLibLine( LineParser& ln );
	void ParseUseMtlLine( LineParser& ln );

	void ParseMtlLine( const std::wstring& line );
	void ParseNewMtlLine( LineParser& ln );
	void ParseMapKdLine( LineParser& ln );
};

#endif