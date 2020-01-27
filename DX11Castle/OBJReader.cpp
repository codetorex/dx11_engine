#include "stdafx.h"
#include "OBJReader.h"
#include <fstream>
#include "FileSystem.h"

using namespace std;

class LineParser
{
public:
	const wstring& str;
	int index;

	wchar_t breakChars[16];
	int breakCharCount;

	LineParser( const wstring& line ) : str( line )
	{
		breakCharCount = 0;
		index = 0;
		addBreakChar( L' ' );
		addBreakChar( L'\t' );
	}

	int getCountOf( wchar_t chr )
	{
		int result = 0;
		for ( int i = 0; i < str.length(); i++ )
		{
			if ( str[i] == chr )
			{
				result++;
			}
		}
		return result;
	}

	void addBreakChar( wchar_t chr )
	{
		breakChars[breakCharCount++] = chr;
	}

	bool isBreakCharacter( wchar_t chr )
	{
		for ( int i = 0; i < breakCharCount; i++ )
		{
			if ( chr == breakChars[i] )
			{
				return true;
			}
		}
		return false;
	}

	void skipWhiteSpace()
	{
		for ( int i = index; i < str.length(); i++ )
		{
			if ( !isBreakCharacter( str[i] ) )
			{
				index = i;
				break;
			}
		}
	}

	void skip()
	{
		wchar_t result[32];
		parse( result );
	}

	void parse( wchar_t* result )
	{
		int k = 0;
		bool notfinished = false;
		for ( int i = index; i < str.length(); i++, k++ )
		{
			if ( isBreakCharacter( str[i] ) )
			{
				result[k] = 0;
				index = i;
				notfinished = true;
				break;
			}
			result[k] = str[i];
		}

		if ( notfinished )
		{
			skipWhiteSpace();
		}
		else
		{
			result[k] = 0;
			index = str.length();
		}
	}

	wstring parseString()
	{
		wchar_t result[32];
		parse( result );
		return wstring( result );
	}

	int ParseInt()
	{
		wchar_t result[32];
		parse( result );
		return _wtoi( result );
	}

	float parseFloat()
	{
		wchar_t result[32];
		parse( result );
		return std::wcstod( result, NULL );
	}
};

void OBJReader::ParseVertexLine( LineParser& ln )
{
	ln.skip();

	Vector3f v;
	v.x = ln.parseFloat();
	v.y = ln.parseFloat();
	v.z = ln.parseFloat();

	if ( BoundingMax.x < v.x ) BoundingMax.x = v.x;
	if ( BoundingMax.y < v.y ) BoundingMax.y = v.y;
	if ( BoundingMax.z < v.z ) BoundingMax.z = v.z;
	if ( v.x < BoundingMin.x ) BoundingMin.x = v.x;
	if ( v.y < BoundingMin.y ) BoundingMin.y = v.y;
	if ( v.z < BoundingMin.z ) BoundingMin.z = v.z;

	Vertices.push_back( v );
}

void OBJReader::ParseTextureLine( LineParser& ln )
{
	ln.skip();

	Vector2f v;
	v.x = ln.parseFloat();
	v.y = 1.0f - ln.parseFloat();

	TextureCoordinates.push_back( v );
}

void OBJReader::ParseNormalLine( LineParser& ln )
{
	ln.skip();

	Vector3f v;
	v.x = ln.parseFloat();
	v.y = ln.parseFloat();
	v.z = ln.parseFloat();

	Normals.push_back( v );
}

void OBJReader::ParseFaceLine( LineParser& ln )
{
	ln.skip();

	ln.addBreakChar( '/' );

	int slashCount = ln.getCountOf( '/' );

	if ( slashCount == 6 )
	{
		if ( ln.str.find( L"//" ) > -1 )
		{
			throw exception( "texture coordinates are missing" );
		}

		int v1 = ln.ParseInt();
		int t1 = ln.ParseInt();
		int n1 = ln.ParseInt();

		int v2 = ln.ParseInt();
		int t2 = ln.ParseInt();
		int n2 = ln.ParseInt();

		int v3 = ln.ParseInt();
		int t3 = ln.ParseInt();
		int n3 = ln.ParseInt();

		OBJFace* result = new OBJFace();
		result->Material = curMtl;
		result->Vertices[0].Position = Vertices[v1 - 1];
		result->Vertices[0].TexUV = TextureCoordinates[t1 - 1];
		result->Vertices[0].Normal = Normals[n1 - 1];

		result->Vertices[1].Position = Vertices[v2 - 1];
		result->Vertices[1].TexUV = TextureCoordinates[t2 - 1];
		result->Vertices[1].Normal = Normals[n2 - 1];

		result->Vertices[2].Position = Vertices[v3 - 1];
		result->Vertices[2].TexUV = TextureCoordinates[t3 - 1];
		result->Vertices[2].Normal = Normals[n3 - 1];

		curGroup->Faces.push_back( result );
	}
	else
	{
		throw exception( "Unsupported obj format" );
	}
}

void OBJReader::ParseGroupLine( LineParser& ln )
{
	ln.skip();

	OBJGroup* ng = new OBJGroup();
	ng->Name = ln.parseString();
	curGroup = ng;
	Groups.push_back( ng );
}

void OBJReader::ParseMtlLibLine( LineParser& ln )
{
	ln.skip();
	wstring mtlPath = ln.parseString();
	LoadMtl( ObjFolder + mtlPath );
}

void OBJReader::ParseUseMtlLine( LineParser& ln )
{
	ln.skip();
	wstring mtlName = ln.parseString();
	curMtl = GetMaterial( mtlName );

	if ( curMtl == NULL )
	{
		throw exception( "Material not found???" );
	}
}

void OBJReader::ParseLine( const std::wstring& line )
{
	LineParser lp( line );

	if ( line.compare( 0, 2, L"v " ) == 0 )
	{
		ParseVertexLine( lp );
	}
	else if ( line.compare( 0, 3, L"vt " ) == 0 )
	{
		ParseTextureLine( lp );
	}
	else if ( line.compare( 0, 3, L"vn " ) == 0 )
	{
		ParseNormalLine( lp );
	}
	else if ( line.compare( 0, 2, L"f " ) == 0 )
	{
		ParseFaceLine( lp );
	}
	else if ( line.compare( 0, 2, L"g " ) == 0 )
	{
		ParseGroupLine( lp );
	}
	else if ( line.compare( 0, 7, L"mtllib " ) == 0 )
	{
		ParseMtlLibLine( lp );
	}
	else if ( line.compare( 0, 7, L"usemtl " ) == 0 )
	{
		ParseUseMtlLine( lp );
	}
}


void OBJReader::LoadObj( const std::wstring& path )
{
	if ( loaded )
	{
		throw exception( "Already loaded" );
	}

	loaded = true;
	std::wifstream          fileStream;
	std::wstring            line;

	fileStream.open( path );
	bool isOpen = fileStream.is_open();	

	if ( !isOpen )
	{
		fileStream.close();
		throw exception( "file not found" );
	}

	ObjFolder = Path::GetDirectoryName( path ) + L"\\";
	BoundingMin.x = 0.0f;
	BoundingMin.y = 0.0f;
	BoundingMin.z = 0.0f;

	while ( std::getline( fileStream, line ) )
	{
		ParseLine( line );
	}
}

void OBJReader::LoadMtl( const std::wstring& path )
{
	std::wifstream          fileStream;
	std::wstring            line;

	fileStream.open( path );
	bool isOpen = fileStream.is_open();

	while ( std::getline( fileStream, line ) )
	{
		ParseMtlLine( line );
	}

	curMtl = NULL;
}

void OBJReader::ParseMtlLine( const std::wstring& line )
{
	LineParser lp( line );

	if ( line.compare( 0, 7, L"newmtl " ) == 0 )
	{
		ParseNewMtlLine( lp );
	}
	else if ( line.compare( 0, 7, L"map_Kd " ) == 0 )
	{
		ParseMapKdLine( lp );
	}
}

OBJMaterial* OBJReader::GetMaterial( std::wstring& name )
{
	for ( int i = 0; i < Materials.size(); i++ )
	{
		OBJMaterial* cmtl = Materials[i];
		if ( cmtl->Name.compare( name ) == 0 )
		{
			return cmtl;
		}
	}

	return NULL;
}

void OBJReader::ParseNewMtlLine( LineParser& ln )
{
	ln.skip();

	OBJMaterial* mtl = new OBJMaterial();
	mtl->Name = ln.parseString();
	Materials.push_back( mtl );
	curMtl = mtl;
}

void OBJReader::ParseMapKdLine( LineParser& ln )
{
	ln.skip();
	curMtl->DiffuseMap = ln.parseString();
}

