#ifndef SHADER_H
#define SHADER_H

#include "stdafx.h"
#include "Interfaces.h"
#include "FileSystem.h"

#include "Texture.h"

class Shader : public IResource, public IRenderable
{
public:
	ID3DBlob* m_Blob;

	void CompileBlob( const string& source, const string& entry, const string& target );
	virtual void Release() = 0;
	virtual void Render() = 0;
	virtual void Compile( const string& source, const string& aFunction ) = 0;
	virtual void CompileFromFile( const wstring& path, const string& aFunction )
	{
		std::string conents = File::ReadAllText( path );
		Compile( conents, aFunction );
	}
};

class PixelShader : public Shader
{
public:
	ID3D11PixelShader* m_PixelShader;
	PixelShader()
	{
		m_PixelShader = NULL;
	}

	void Release()
	{
		m_PixelShader->Release();
	}

	void Compile( const string& source, const string& aFunction );

	void Render();
};

class GeometryShader : public Shader
{
public:
	ID3D11GeometryShader* m_GeometryShader;
	ID3D11InputLayout* m_VertexLayout;

	GeometryShader()
	{
		m_GeometryShader = NULL;
		m_VertexLayout = NULL;
	}

	void Release()
	{
		m_GeometryShader->Release();
	}

	virtual void CompileInputLayout() = 0;
	void Compile( const string& source, const string& aFunction );

	void Render();
};

class VertexShader : public Shader
{
public:
	ID3D11VertexShader* m_VertexShader;
	ID3D11InputLayout* m_VertexLayout;

	VertexShader()
	{
		m_VertexShader = NULL;
		m_VertexLayout = NULL;
	}

	void Initialize()
	{

	}

	void Release()
	{
		m_VertexShader->Release();
	}

	void Render();

	virtual void CompileInputLayout() = 0;
	void Compile( const string& source, const string& aFunction );
};

template<int Slot>
class PSTextureSlot : public IRenderable
{
public:
	Texture* m_Texture;

	PSTextureSlot<Slot>& operator = (Texture& texture)
	{
		m_Texture = &texture;
		return *this;
	}

	void Render()
	{
		Engine.m_Renderer.m_Context->PSSetShaderResources( Slot, 1, &m_Texture->m_ResourceView );
	}
};

class PSSampler
{
public:
	ID3D11SamplerState* m_Sampler;

	PSSampler()
	{
		m_Sampler = NULL;
	}

	void Create( D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
				 UINT MaxAnisotropy = 0,
				 D3D11_TEXTURE_ADDRESS_MODE u = D3D11_TEXTURE_ADDRESS_WRAP,
				 D3D11_TEXTURE_ADDRESS_MODE v = D3D11_TEXTURE_ADDRESS_WRAP )
	{
		if ( m_Sampler != NULL )
		{
			m_Sampler->Release();
			m_Sampler = NULL;
		}

		// create the sample state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory( &sampDesc, sizeof(sampDesc) );
		sampDesc.Filter = filter;
		//sampDesc.Filter = D3D11_FILTER_COMPARISON_ANISOTROPIC;
		sampDesc.AddressU = u;
		sampDesc.AddressV = v;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MaxAnisotropy = MaxAnisotropy;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		HRESULT hr = Engine.m_Renderer.m_Device->CreateSamplerState( &sampDesc, &m_Sampler );
		if ( FAILED( hr ) )
			throw exception( "create sampler state failed" );
	}
};

template <int Slot>
class PSSamplerSlot : public IRenderable
{
public:
	PSSampler* m_Sampler;

	void Render()
	{
		Engine.m_Renderer.m_Context->PSSetSamplers( Slot, 1, &m_Sampler->m_Sampler );
	}
};


#endif