#ifndef ENVIRONMENTSHADER_H
#define ENVIRONMENTSHADER_H

#include "Shader.h"
#include "Shader.h"
#include "Buffer.h"

#include "VertexFormats.h"

class EnvironmentPixelShader : public PixelShader
{
public:
	static EnvironmentPixelShader* Instance;

	struct LightData
	{
		XMFLOAT4 materialColour;
		XMVECTOR vecLight;
	};

	PSConstantBuffer<LightData, 0> m_Light;
	PSTextureSlot<0> m_CubeMapTexture;
	PSSamplerSlot<0> m_TextureSampler;

	EnvironmentPixelShader()
	{
		CompileFromFile( L"EnvironmentPixelShader.hlsl", "main" );

		LightData ld;
		ld.materialColour = XMFLOAT4( 1, 1, 1, 1 );
		ld.vecLight = XMVector3Normalize( XMVectorSet( -500, 500, 500, 0 ) );
		m_Light = ld;

		m_TextureSampler.m_Sampler = new PSSampler();
		m_TextureSampler.m_Sampler->Create();
	}

	void Render()
	{
		this->PixelShader::Render();
		m_Light.Render();
		m_CubeMapTexture.Render();
		m_TextureSampler.Render();
	}
};

class EnvironmentVertexShader : public VertexShader
{
public:
	D3D_PRIMITIVE_TOPOLOGY m_Topology;

	static EnvironmentVertexShader* Instance;

	struct ProjectionData
	{
		XMMATRIX matProjection;
	};

	struct ViewData
	{
		XMMATRIX matWorld;
		XMMATRIX matWorldViewProjection;
		XMMATRIX WorldInverseTranspose;
		XMVECTOR cameraPos;
	};

	VSConstantBuffer<ProjectionData, 0> m_Projection;
	VSConstantBuffer<ViewData, 1> m_View;

	VertexBufferSlot<VertexNormalTextured, 0> m_Vertices;

	void CompileInputLayout();

	void Render()
	{
		this->VertexShader::Render();

		m_View.m_State.cameraPos = Engine.m_Camera.Eye;
		m_View.Update();

		m_Projection.Render();
		m_View.Render();

		Engine.m_Renderer.m_Context->IASetInputLayout( m_VertexLayout );
		m_Vertices.Render();

		Engine.m_Renderer.m_Context->IASetPrimitiveTopology( m_Topology );

		Engine.m_Renderer.m_Context->GSSetShader( NULL, NULL, 0 );
		Engine.m_Renderer.m_Context->Draw( m_Vertices.m_Buffer->m_VertexCount, 0 );
	}

	EnvironmentVertexShader()
	{
		CompileFromFile( L"EnvironmentVertexShader.hlsl", "main" );
		m_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
};

#endif
