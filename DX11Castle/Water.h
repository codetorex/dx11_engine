#ifndef WATER_H
#define WATER_H

#include "OBJRenderer.h"
#include "Entity.h"


class Water : public Entity
{
public:
	OBJRenderer* m_Renderer;
	Texture* m_Environment;

	Water( Texture* environment )
	{
		m_Renderer = new OBJRenderer();
		m_Environment = environment;
		m_Renderer->Load( L"Media\\Water\\water.obj" );
	}

	void Render()
	{
		Render( m_Matrix );
	}

	void RenderTextured( XMMATRIX& objMat )
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

		for ( int i = 0; i < m_Renderer->m_VB.m_Parts.size(); i++ )
		{
			auto curPart = m_Renderer->m_VB.m_Parts[i];
			m_PS->m_Texture = *curPart->m_Texture;
			m_PS->Render();

			m_VS->m_Vertices = curPart->m_Buffer;
			m_VS->m_Projection = pd;
			m_VS->m_View = vd;
			m_VS->Render();
		}
	}

	void Render( XMMATRIX& objMat )
	{
		WaterVertexShader* m_VS = WaterVertexShader::Instance;
		WaterPixelShader* m_PS = WaterPixelShader::Instance;

		WaterVertexShader::ProjectionData pd;
		pd.matProjection = XMMatrixTranspose( Engine.m_Renderer.m_Viewport.m_Projection );

		//XMMATRIX matCubeWorld = XMMatrixIdentity();

		XMMATRIX matWVP = objMat * Engine.m_Camera.View * Engine.m_Renderer.m_Viewport.m_Projection;

		WaterVertexShader::ViewData vd;

		XMMATRIX cpy = XMMATRIX( objMat );

		vd.matWorld = XMMatrixTranspose( cpy );
		vd.matWorldViewProjection = XMMatrixTranspose( matWVP );
		vd.gEyePos.Set( Engine.m_Camera.Eye );
		vd.Timer = Engine.m_Time.m_Time;

		Engine.m_Renderer.EnableBlending( D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA );

		for ( int i = 0; i < m_Renderer->m_VB.m_Parts.size(); i++ )
		{
			auto curPart = m_Renderer->m_VB.m_Parts[i];
			m_PS->m_Normal = *curPart->m_Texture;
			m_PS->m_CubeMap = *m_Environment;
			m_PS->Render();

			m_VS->m_Vertices = curPart->m_Buffer;
			m_VS->m_Projection = pd;
			m_VS->m_View = vd;
			m_VS->Render();
		}
		Engine.m_Renderer.DisableBlending();
	}
};

#endif