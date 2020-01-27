#ifndef TEXTUREDSHADER_H
#define TEXTUREDSHADER_H

#include "Shader.h"
#include "Buffer.h"
#include "VertexFormats.h"

class TexturedPixelShader : public PixelShader
{
public:
	static TexturedPixelShader* Instance;

#pragma pack(push)
#pragma pack(16)
	struct LightData
	{
		XMFLOAT4 materialColour;
		XMVECTOR lightPos;
		float lightPower;
		bool lightUseDist;
	};
#pragma pack(pop)

	PSConstantBuffer<LightData, 0> m_Light;
	PSTextureSlot<0> m_Texture;
	PSSamplerSlot<0> m_TextureSampler;

	TexturedPixelShader()
	{
		CompileFromFile( L"TexturedPixelShader.hlsl", "main" );

		LightData ld;
		memset( &ld, 0, sizeof(LightData) );
		ld.materialColour = XMFLOAT4( 1, 1, 1, 1 );
		ld.lightPos = XMVectorSet( Engine.m_Light.m_Position.x, Engine.m_Light.m_Position.y, Engine.m_Light.m_Position.z, 1 );
		ld.lightPower = Engine.m_Light.m_Power;

		m_Light = ld;

		if ( sizeof(LightData) % 16 != 0 )
			throw exception( "Constant buffer is not aligned" );

		m_TextureSampler.m_Sampler = new PSSampler();
		m_TextureSampler.m_Sampler->Create();
	}

	void Render()
	{
		m_Light.m_State.lightUseDist = Engine.m_Light.m_UseDistance;
		m_Light.m_State.lightPower = Engine.m_Light.m_Power;
		m_Light.m_State.lightPos.m128_f32[0] = Engine.m_Light.m_Position.x;
		m_Light.m_State.lightPos.m128_f32[1] = Engine.m_Light.m_Position.y;
		m_Light.m_State.lightPos.m128_f32[2] = Engine.m_Light.m_Position.z;
		m_Light.Update();

		this->PixelShader::Render();
		m_Light.Render();
		m_Texture.Render();
		m_TextureSampler.Render();
	}
};

class TexturedVertexShader : public VertexShader
{
public:
	D3D_PRIMITIVE_TOPOLOGY m_Topology;

	static TexturedVertexShader* Instance;

	struct ProjectionData
	{
		XMMATRIX matProjection;
	};

	struct ViewData
	{
		XMMATRIX matWorld;
		XMMATRIX matWorldViewProjection;
	};

	VSConstantBuffer<ProjectionData, 0> m_Projection;
	VSConstantBuffer<ViewData, 1> m_View;

	VertexBufferSlot<VertexNormalTextured, 0> m_Vertices;

	void CompileInputLayout();

	void Render()
	{
		this->VertexShader::Render();

		m_Projection.Render();
		m_View.Render();

		Engine.m_Renderer.m_Context->IASetInputLayout( m_VertexLayout );
		m_Vertices.Render();

		Engine.m_Renderer.m_Context->IASetPrimitiveTopology( m_Topology );

		Engine.m_Renderer.m_Context->GSSetShader( NULL, NULL, 0 );
		Engine.m_Renderer.m_Context->Draw( m_Vertices.m_Buffer->m_VertexCount, 0 );
	}

	TexturedVertexShader()
	{
		CompileFromFile( L"TexturedVertexShader.hlsl", "main" );
		m_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
};

#endif // TEXTUREDSHADER_H
