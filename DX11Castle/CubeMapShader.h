#ifndef CUBEMAPSHADER_H
#define CUBEMAPSHADER_H

#include "Shader.h"
#include "Shader.h"
#include "Buffer.h"

#include "VertexFormats.h"

class CubeMapPixelShader : public PixelShader
{
public:
	static CubeMapPixelShader* Instance;

	PSTextureSlot<0> m_CubeMapTexture;
	PSSamplerSlot<0> m_TextureSampler;

	CubeMapPixelShader()
	{
		CompileFromFile( L"CubeMapPixelShader.hlsl", "main" );

		m_TextureSampler.m_Sampler = new PSSampler();
		m_TextureSampler.m_Sampler->Create();
	}

	void Render()
	{
		this->PixelShader::Render();
		m_CubeMapTexture.Render();
		m_TextureSampler.Render();
	}
};

class CubeMapVertexShader : public VertexShader
{
public:
	D3D_PRIMITIVE_TOPOLOGY m_Topology;

	static CubeMapVertexShader* Instance;

	struct ViewData
	{
		XMMATRIX matWorld;
		XMMATRIX matWorldViewProjection;
	};

	VSConstantBuffer<ViewData, 0> m_View;
	VertexBufferSlot<VertexNormalTextured, 0> m_Vertices;

	void CompileInputLayout();

	void Render()
	{
		this->VertexShader::Render();

		m_View.Render();

		Engine.m_Renderer.m_Context->IASetInputLayout( m_VertexLayout );
		m_Vertices.Render();

		Engine.m_Renderer.m_Context->IASetPrimitiveTopology( m_Topology );
		Engine.m_Renderer.m_Context->GSSetShader( NULL, NULL, 0 );
		Engine.m_Renderer.m_Context->Draw( m_Vertices.m_Buffer->m_VertexCount, 0 );
	}

	CubeMapVertexShader()
	{
		CompileFromFile( L"CubeMapVertexShader.hlsl", "main" );
		m_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
};

#endif // CUBEMAPSHADER_H
