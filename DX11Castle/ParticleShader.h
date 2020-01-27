#ifndef PARTICLESHADER_H
#define PARTICLESHADER_H

#include "Shader.h"
#include "Buffer.h"
#include "VertexFormats.h"

class ParticlePixelShader : public PixelShader
{
public:
	static ParticlePixelShader* Instance;

	PSTextureSlot<0> m_ParticleTexture;
	PSSamplerSlot<0> m_TextureSampler;

	ParticlePixelShader()
	{
		CompileFromFile( L"ParticlePixelShader.hlsl", "main" );

		m_TextureSampler.m_Sampler = new PSSampler();
		m_TextureSampler.m_Sampler->Create();
	}

	void Render()
	{
		this->PixelShader::Render();
		m_ParticleTexture.Render();
		m_TextureSampler.Render();
	}
};

class ParticleGeometryShader : public GeometryShader
{
public:
	static ParticleGeometryShader* Instance;

	struct ViewData
	{
		XMVECTOR gEyePos;
		XMMATRIX MatWorldViewProjection;
	};

	GSConstantBuffer<ViewData, 0> m_ViewData;

	ParticleGeometryShader()
	{
		CompileFromFile( L"ParticleGeometryShader.hlsl", "main" );
	}

	void CompileInputLayout()
	{

	}

	void Render()
	{
		m_ViewData.m_State.gEyePos = Engine.m_Camera.Eye;
		m_ViewData.Update();
		m_ViewData.Render();

		this->GeometryShader::Render();
	}
};

class ParticleVertexShader : public VertexShader
{
public:
	D3D_PRIMITIVE_TOPOLOGY m_Topology;

	static ParticleVertexShader* Instance;

	struct ViewData
	{
		XMMATRIX matWorldViewProjection;
	};
	VSConstantBuffer<ViewData, 0> m_WorldViewProjection;

	VertexBufferSlot<VertexParticle, 0> m_Vertices;
	IndexBufferSlot m_Indexes;

	void CompileInputLayout();

	void Render()
	{
		this->VertexShader::Render();

		m_WorldViewProjection.Render();

		Engine.m_Renderer.m_Context->IASetInputLayout( m_VertexLayout );
		m_Vertices.Render();

		Engine.m_Renderer.m_Context->IASetPrimitiveTopology( m_Topology );
		m_Indexes.Render();
		Engine.m_Renderer.m_Context->DrawIndexed( m_Indexes.m_Buffer->m_IndexCount, 0, 0 );
	}

	ParticleVertexShader()
	{
		CompileFromFile( L"ParticleVertexShader.hlsl", "main" );
		m_Topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	}
};

#endif // CUBEMAPSHADER_H
