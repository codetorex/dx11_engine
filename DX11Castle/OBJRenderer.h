#ifndef OBJRENDERER_H
#define OBJRENDERER_H

#include "TexturedShader.h"
#include "NormalShader.h"
#include "OBJReader.h"
#include "OBJVertexBufferBuilder.h"
#include "Texture.h"




class OBJRenderer
{
public:
	OBJVertexBufferBuilder< VertexBuffer< VertexNormalTextured>, Texture > m_VB;
	Vector3f m_BoundingMax;
	Vector3f m_BoundingMin;

	void Load( const std::wstring& path )
	{
		OBJReader reader;
		reader.LoadObj( path );

		m_VB.Build( reader );
		m_BoundingMax = reader.BoundingMax;
		m_BoundingMin = reader.BoundingMin;
	}

	void Render( XMMATRIX& objMat )
	{
		TexturedVertexShader* m_VS = TexturedVertexShader::Instance;
		TexturedPixelShader* m_PS = TexturedPixelShader::Instance;

		TexturedVertexShader::ProjectionData pd;
		pd.matProjection = XMMatrixTranspose( Engine.m_Renderer.m_Viewport.m_Projection );

		//XMMATRIX matCubeWorld = XMMatrixIdentity();

		XMMATRIX matWVP = objMat * Engine.m_Camera.View * Engine.m_Renderer.m_Viewport.m_Projection;

		TexturedVertexShader::ViewData vd;

		XMMATRIX cpy = XMMATRIX( objMat );

		vd.matWorld = XMMatrixTranspose( cpy );
		vd.matWorldViewProjection = XMMatrixTranspose( matWVP );

		for ( int i = 0; i < m_VB.m_Parts.size(); i++ )
		{
			auto curPart = m_VB.m_Parts[i];
			m_PS->m_Texture = *curPart->m_Texture;
			m_PS->Render();

			m_VS->m_Vertices = curPart->m_Buffer;
			m_VS->m_Projection = pd;
			m_VS->m_View = vd;
			m_VS->Render();
		}
	}
};

template < class vertbuf, class texture>
class NormalRenderPart
{
public:
	texture* m_Texture;
	texture* m_Normal;
	vertbuf m_Buffer;
};

class NormalModel
{
public:
	typedef NormalRenderPart< VertexBuffer<VertexNormalTexturedTangentBinormal >, Texture > NormalPart;
	std::vector<NormalPart*> m_Parts;

	void CreateFromObjRenderer( OBJRenderer* obj )
	{
		for ( auto& objpart : obj->m_VB.m_Parts )
		{
			CreatePart( objpart->m_Buffer, objpart->m_Texture, NULL ); // warning we have to set normal map manually, since mtl format doesnt supports it
		}
	}

	void CalculateTangentBinormal( VertexNormalTextured& v1, VertexNormalTextured& v2, VertexNormalTextured& v3, Vector3f& tangent, Vector3f& binormal )
	{
		CalculateTangentBinormal( v1.Position, v1.TexUV,
								  v2.Position, v2.TexUV,
								  v3.Position, v3.TexUV,
								  tangent, binormal );
	}

	void CalculateTangentBinormal( Vector3f& v1p, Vector2f& v1t,
								   Vector3f& v2p, Vector2f& v2t,
								   Vector3f& v3p, Vector2f& v3t,
								   Vector3f& tangent, Vector3f& binormal )
	{
		float vector1[3], vector2[3];
		float tuVector[2], tvVector[2];
		float den;
		float length;

		// Calculate the two vectors for this face.
		vector1[0] = v2p.x - v1p.x;
		vector1[1] = v2p.y - v1p.y;
		vector1[2] = v2p.z - v1p.z;

		vector2[0] = v3p.x - v1p.x;
		vector2[1] = v3p.y - v1p.y;
		vector2[2] = v3p.z - v1p.z;

		// Calculate the tu and tv texture space vectors.
		tuVector[0] = v2t.x - v1t.x;
		tvVector[0] = v2t.y - v1t.y;

		tuVector[1] = v3t.x - v1t.x;
		tvVector[1] = v3t.y - v1t.y;

		// Calculate the denominator of the tangent/binormal equation.
		den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

		// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
		tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
		tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
		tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

		binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
		binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
		binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

		// Calculate the length of this normal.
		length = sqrt( (tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z) );

		// Normalize the normal and then store it
		tangent.x = tangent.x / length;
		tangent.y = tangent.y / length;
		tangent.z = tangent.z / length;

		// Calculate the length of this normal.
		length = sqrt( (binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z) );

		// Normalize the normal and then store it
		binormal.x = binormal.x / length;
		binormal.y = binormal.y / length;
		binormal.z = binormal.z / length;

		return;
	}

	void CalculateNormal( Vector3f& tangent, Vector3f& binormal, Vector3f& normal )
	{
		float length;

		// Calculate the cross product of the tangent and binormal which will give the normal vector.
		normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
		normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
		normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

		// Calculate the length of the normal.
		length = sqrt( (normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z) );

		// Normalize the normal.
		normal.x = normal.x / length;
		normal.y = normal.y / length;
		normal.z = normal.z / length;
	}

	void CreatePart( VertexBuffer<VertexNormalTextured>& vertices, Texture* diffuse, Texture* normal )
	{
		NormalPart* np = new NormalPart();
		np->m_Texture = diffuse;
		np->m_Normal = normal;

		Vector3f t, b, n;

		VertexNormalTexturedTangentBinormal nv1, nv2, nv3;

		int faceCount = vertices.m_VertexCount / 3;

		for ( int i = 0; i < faceCount; i++ )
		{
			int k = i * 3;
			auto& v1 = vertices.m_Vertices[k];
			auto& v2 = vertices.m_Vertices[k + 1];
			auto& v3 = vertices.m_Vertices[k + 2];

			CalculateTangentBinormal( v1, v2, v3, t, b );
			CalculateNormal( b, t, n );

			nv1.Position = v1.Position;
			nv1.TexUV = v1.TexUV;
			nv1.Tangent = t;
			nv1.BiNormal = b;
			nv1.Normal = n;

			nv2.Position = v2.Position;
			nv2.TexUV = v2.TexUV;
			nv2.Tangent = t;
			nv2.BiNormal = b;
			nv2.Normal = n;

			nv3.Position = v3.Position;
			nv3.TexUV = v3.TexUV;
			nv3.Tangent = t;
			nv3.BiNormal = b;
			nv3.Normal = n;

			np->m_Buffer.AddVertex( nv1 );
			np->m_Buffer.AddVertex( nv2 );
			np->m_Buffer.AddVertex( nv3 );
		}

		np->m_Buffer.Create();
		m_Parts.push_back( np );
	}

	void Render( XMMATRIX& objMat )
	{
		NormalVertexShader* m_VS = NormalVertexShader::Instance;
		NormalPixelShader* m_PS = NormalPixelShader::Instance;

		NormalVertexShader::ProjectionData pd;
		pd.matProjection = XMMatrixTranspose( Engine.m_Renderer.m_Viewport.m_Projection );

		//XMMATRIX matCubeWorld = XMMatrixIdentity();

		XMMATRIX matWVP = objMat * Engine.m_Camera.View * Engine.m_Renderer.m_Viewport.m_Projection;

		NormalVertexShader::ViewData vd;

		XMMATRIX cpy = XMMATRIX( objMat );

		vd.matWorld = XMMatrixTranspose( cpy );
		vd.matWorldViewProjection = XMMatrixTranspose( matWVP );

		for ( int i = 0; i < m_Parts.size(); i++ )
		{
			auto curPart = m_Parts[i];
			m_PS->m_Texture = *curPart->m_Texture;
			m_PS->m_Normal = *curPart->m_Normal;
			m_PS->Render();

			m_VS->m_Vertices = curPart->m_Buffer;
			m_VS->m_Projection = pd;
			m_VS->m_View = vd;
			m_VS->Render();
		}
	}
};


#endif