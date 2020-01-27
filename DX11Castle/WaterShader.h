#ifndef WATERSHADER_H
#define WATERSHADER_H

#include "Shader.h"
#include "Buffer.h"
#include "VertexFormats.h"

class WaterPixelShader : public PixelShader
{
public:
	static WaterPixelShader* Instance;

	struct LightData
	{
		XMFLOAT4 materialColour;
		XMVECTOR vecLight;
	};

	PSConstantBuffer<LightData, 0> m_Light;
	PSTextureSlot<0> m_Normal;
	PSTextureSlot<1> m_CubeMap;
	PSSamplerSlot<0> m_TextureSampler;

	WaterPixelShader()
	{
		CompileFromFile( L"WaterPixelShader.hlsl", "main" );

		LightData ld;
		ld.materialColour = XMFLOAT4( 1, 1, 1, 1 );
		ld.vecLight = XMVector3Normalize( XMVectorSet( 500, 500, 500, 0 ) );
		m_Light = ld;

		m_TextureSampler.m_Sampler = new PSSampler();
		m_TextureSampler.m_Sampler->Create();
	}

	void Render()
	{
		this->PixelShader::Render();
		m_Light.Render();
		m_Normal.Render();
		m_CubeMap.Render();
		m_TextureSampler.Render();
	}
};

class WaterVertexShader : public VertexShader
{
public:
	D3D_PRIMITIVE_TOPOLOGY m_Topology;

	static WaterVertexShader* Instance;

	struct ProjectionData
	{
		XMMATRIX matProjection;
	};

	struct ViewData
	{
		XMMATRIX matWorld;
		XMMATRIX matWorldViewProjection;
		Vector3f gEyePos;
		float Timer;
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

	WaterVertexShader()
	{
		CompileFromFile( L"WaterVertexShader.hlsl", "main" );
		m_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
};

#endif