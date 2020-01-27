#ifndef GLOW_H
#define GLOW_H

#include "Entity.h"
#include "ScreenSpaceQuad.h"
#include "GlowShader.h"

class Glow : public Entity
{
public:
	std::vector< Entity* > m_GlowEntities;

	void AddGlowEntity( Entity* e )
	{
		m_GlowEntities.push_back( e );
	}

	void Render()
	{

		GlowPixelShaderVPS::Instance->GlowRenderTarget.Begin( true );
		GlowPixelShaderVPS::Instance->GlowRenderTarget.Clear( true );

		for ( auto& e : m_GlowEntities )
		{
			e->Render();
		}

		GlowPixelShaderVPS::Instance->GlowRenderTarget.End();

		GlowVertexShader* m_VS = GlowVertexShader::Instance;
		m_VS->m_Vertices = ScreenSpaceQuad::Instance->m_Buffer;

		GlowPixelShaderVPS* m_PS = GlowPixelShaderVPS::Instance;
		GlowPixelShaderHPS* m_PS0 = GlowPixelShaderHPS::Instance;

		m_PS0->GlowRenderTarget.Begin( false );
		m_PS0->GlowRenderTarget.Clear( true );

		m_PS->Render();
		m_VS->Render();

		m_PS0->GlowRenderTarget.End();


		Engine.m_Renderer.EnableBlending( D3D11_BLEND_ONE, D3D11_BLEND_ONE );

		m_PS0->Render();
		m_VS->Render();

		Engine.m_Renderer.DisableBlending();
	}
};

#endif