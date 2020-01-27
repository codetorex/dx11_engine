#ifndef GLOWSHADER_H
#define GLOWSHADER_H

#include "Shader.h"
#include "Shader.h"
#include "Buffer.h"

#include "VertexFormats.h"



class GlowPixelShaderVPS : public PixelShader
{
public:
	static GlowPixelShaderVPS* Instance;

	PSTextureSlot<0> m_Texture;
	PSSamplerSlot<0> m_TextureSampler;

	RenderTarget GlowRenderTarget;

	GlowPixelShaderVPS()
	{
		CompileFromFile( L"GlowPixelShaderV.hlsl", "main" );

		m_TextureSampler.m_Sampler = new PSSampler();
		m_TextureSampler.m_Sampler->Create( D3D11_FILTER_MIN_MAG_MIP_LINEAR, 0,
											D3D11_TEXTURE_ADDRESS_BORDER, D3D11_TEXTURE_ADDRESS_BORDER );

		Engine.m_Renderer.CreateRenderTarget( Engine.m_Renderer.m_Viewport.m_Width,
											  Engine.m_Renderer.m_Viewport.m_Height
											  , GlowRenderTarget );
	}

	void Render()
	{
		m_Texture = *GlowRenderTarget.m_Texture;

		this->PixelShader::Render();
		m_Texture.Render();
		m_TextureSampler.Render();
	}
};

class GlowPixelShaderHPS : public PixelShader
{
public:
	static GlowPixelShaderHPS* Instance;

	PSTextureSlot<0> m_Texture;
	PSSamplerSlot<0> m_TextureSampler;

	RenderTarget GlowRenderTarget;

	GlowPixelShaderHPS()
	{
		CompileFromFile( L"GlowPixelShaderH.hlsl", "main" );

		m_TextureSampler.m_Sampler = new PSSampler();
		m_TextureSampler.m_Sampler->Create( D3D11_FILTER_MIN_MAG_MIP_LINEAR, 0,
											D3D11_TEXTURE_ADDRESS_BORDER, D3D11_TEXTURE_ADDRESS_BORDER );

		Engine.m_Renderer.CreateRenderTarget( Engine.m_Renderer.m_Viewport.m_Width,
											  Engine.m_Renderer.m_Viewport.m_Height
											  , GlowRenderTarget );
	}

	void Render()
	{
		m_Texture = *GlowRenderTarget.m_Texture;

		this->PixelShader::Render();
		m_Texture.Render();
		m_TextureSampler.Render();
	}
};

class GlowVertexShader : public VertexShader
{
public:
	D3D_PRIMITIVE_TOPOLOGY m_Topology;

	static GlowVertexShader* Instance;

	VertexBufferSlot<VertexPosition, 0> m_Vertices;

	void CompileInputLayout();

	void Render()
	{
		this->VertexShader::Render();

		Engine.m_Renderer.m_Context->IASetInputLayout( m_VertexLayout );
		m_Vertices.Render();

		Engine.m_Renderer.m_Context->IASetPrimitiveTopology( m_Topology );

		Engine.m_Renderer.m_Context->GSSetShader( NULL, NULL, 0 );
		Engine.m_Renderer.m_Context->Draw( m_Vertices.m_Buffer->m_VertexCount, 0 );
	}

	GlowVertexShader()
	{
		CompileFromFile( L"GlowVertexShader.hlsl", "main" );
		m_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
};

#endif
