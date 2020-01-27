#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Engine.h"
#include "Math.h"
#include <vector>
#include "ParticleShader.h"

struct Particle
{
	XMVECTOR m_Position;
	XMVECTOR m_Direction;
	float m_Speed;
	float m_Size;
	float m_Alpha;
};

class ParticleEmitter : public IUpdateable, public IRenderable
{
public:
	std::vector<Particle> m_Particles;

	static ID3D11RasterizerState* ParticleState;


	XMMATRIX m_Orientation;
	XMVECTOR m_Position;

	float m_EmitCone;

	float m_EmissionTime;
	float m_ParticleStartingSize;
	float m_ParticleStartingSpeed;

	bool m_Emitting;

	float m_AlphaChangePerSecond;
	float m_SizeChangePerSecond;

	Texture m_Texture;
	VertexBuffer<VertexParticle> m_VertexBuffer;
	IndexBuffer m_IndexBuffer;

	D3D11_DEPTH_STENCIL_DESC m_StencilDescription;
	ID3D11DepthStencilState* m_DepthStencil;

	ParticleEmitter() : m_UntilEmissionTime( 0.0f ), m_AlphaChangePerSecond( 0.0f ), m_SizeChangePerSecond( 0.0f )
	{
		m_Orientation = XMMatrixIdentity();
		m_Position = XMVectorZero();

		m_VertexBuffer.AddVertex( VertexParticle() );
		m_VertexBuffer.Create();
		m_IndexBuffer.AddIndex( 0 );
		m_IndexBuffer.Create();
		m_Emitting = false;
		m_EmitCone = (float)0;
		ParticleState = NULL;

		ZeroMemory( &m_StencilDescription, sizeof(D3D11_DEPTH_STENCIL_DESC) );
		m_StencilDescription.DepthEnable = FALSE;
		Engine.m_Renderer.m_Device->CreateDepthStencilState( &m_StencilDescription, &m_DepthStencil );
	}

	void Update()
	{

		if ( ParticleState == NULL )
			ParticleState = Engine.m_Renderer.CreateRasterizerState( false, false );

		const float diffTime = Engine.m_Time.m_Diff;
		m_UntilEmissionTime -= diffTime;

		if ( m_Emitting )
		{
			if ( m_UntilEmissionTime <= 0.0f )
			{
				m_UntilEmissionTime = m_EmissionTime;
				EmittParticle();
			}
		}

		for ( int i = m_Particles.size() - 1; i > -1; --i )
		{
			Particle& currentParticle = m_Particles[i];
			currentParticle.m_Alpha += m_AlphaChangePerSecond * diffTime;

			if ( currentParticle.m_Alpha <= 0.0f || currentParticle.m_Size < 0.0f )
			{
				m_Particles.erase( m_Particles.begin() + i );
				continue;
			}
			currentParticle.m_Size += m_SizeChangePerSecond * diffTime;
			currentParticle.m_Position.m128_f32[0] += currentParticle.m_Direction.m128_f32[0] * currentParticle.m_Speed * diffTime;
			currentParticle.m_Position.m128_f32[1] += currentParticle.m_Direction.m128_f32[1] * currentParticle.m_Speed * diffTime;
			currentParticle.m_Position.m128_f32[2] += currentParticle.m_Direction.m128_f32[2] * currentParticle.m_Speed * diffTime;
		}

		//m_VertexBuffer.Clear();
		//m_IndexBuffer.Clear();
		if ( m_Particles.size() )
		{
			m_VertexBuffer.Reset();
			m_IndexBuffer.Reset();

			//Updating subresources
			for ( int i = 0; i < m_Particles.size(); ++i )
			{
				Particle& currentParticle = m_Particles[i];
				VertexParticle newVertex;
				newVertex.Position.x = currentParticle.m_Position.m128_f32[0];
				newVertex.Position.y = currentParticle.m_Position.m128_f32[1];
				newVertex.Position.z = currentParticle.m_Position.m128_f32[2];
				newVertex.Alpha = currentParticle.m_Alpha;
				newVertex.Size = currentParticle.m_Size;
				m_VertexBuffer.AddVertex( newVertex );
				m_IndexBuffer.AddIndex( i );
			}

			m_VertexBuffer.Create();
			m_IndexBuffer.Create();
		}
		//m_VertexBuffer.Update();
		//m_IndexBuffer.Update();
	}

	void Render()
	{
		if ( !m_Particles.size() )
			return;

		//ID3D11DepthStencilState* oldDepthStencil = NULL;
		//UINT depthStencilRef = 0;
		//Engine.m_Renderer.m_Context->OMGetDepthStencilState( &oldDepthStencil, &depthStencilRef );
		//Engine.m_Renderer.m_Context->OMSetDepthStencilState( m_DepthStencil, 0 );
		// 

		ID3D11DepthStencilState* oldDepthStencil = NULL;
		UINT depthStencilRef = 0;
		Engine.m_Renderer.m_Context->OMGetDepthStencilState( &oldDepthStencil, &depthStencilRef );
		Engine.m_Renderer.m_Context->OMSetDepthStencilState( m_DepthStencil, 0 );

		Engine.m_Renderer.EnableBlending( D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA );
		Engine.m_Renderer.m_Context->RSSetState( ParticleState );
		//Engine.m_Renderer.EnableBlending(D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_ONE);
		//Engine.m_Renderer.EnableBlending( D3D11_BLEND_ONE, D3D11_BLEND_ONE );
		//Engine.m_Renderer.EnableBlending(D3D11_BLEND_DEST_COLOR, D3D11_BLEND_ONE);
		ParticlePixelShader::Instance->m_ParticleTexture = m_Texture;
		ParticlePixelShader::Instance->Render();

		ParticleGeometryShader::ViewData vdwvp;
		vdwvp.MatWorldViewProjection = XMMatrixTranspose( m_Orientation * Engine.m_Camera.View * Engine.m_Renderer.m_Viewport.m_Projection );
		vdwvp.gEyePos = Engine.m_Camera.Eye;
		ParticleGeometryShader::Instance->m_ViewData = vdwvp;
		ParticleGeometryShader::Instance->Render();



		//		DirectX11ParticleVertexShader::Instance->m_WorldViewProjection = vdwvp;
		ParticleVertexShader::Instance->m_Vertices = m_VertexBuffer;
		ParticleVertexShader::Instance->m_Indexes = m_IndexBuffer;

		ParticleVertexShader::Instance->Render();

		Engine.m_Renderer.DisableBlending();
		Engine.m_Renderer.m_Context->OMSetDepthStencilState( oldDepthStencil, depthStencilRef );
		//Engine.m_Renderer.m_Context->OMSetDepthStencilState( oldDepthStencil, depthStencilRef );
	}

	void SetEmittingCone( float angleInDegree )
	{
		m_EmitCone = XMConvertToRadians( angleInDegree );
	}

private:
	void EmittParticle()
	{
		Particle newParticle;
		newParticle.m_Position = m_Position;
		newParticle.m_Direction = XMVectorSet( 0, 1, 0, 0 );
		if ( m_EmitCone > 0.0f )
		{
			float k1 = (float)(rand() % 65536) / 65536.0f;
			float k2 = (float)(rand() % 65536) / 65536.0f;
			float ang1 = (k1 - 0.5f) * m_EmitCone;
			float ang2 = (k2 - 0.5f) * m_EmitCone;


			float theta = acos( newParticle.m_Direction.m128_f32[2] );
			float phi = atan2( newParticle.m_Direction.m128_f32[1], newParticle.m_Direction.m128_f32[0] );

			theta += ang1;
			phi += ang2;

			float x = sin( theta ) * cos( phi );
			float y = sin( theta ) * sin( phi );
			float z = cos( theta );

			newParticle.m_Direction = XMVectorSet( x, y, z, 0.0f );
		}

		newParticle.m_Speed = m_ParticleStartingSpeed + (((float)(rand() % 65536) / 65536.0f) * 20.0f);
		newParticle.m_Alpha = 1.0f;
		newParticle.m_Size = m_ParticleStartingSize;
		m_Particles.push_back( newParticle );
	}

	float m_UntilEmissionTime;
};

#endif // PARTICLEEMITTER_H