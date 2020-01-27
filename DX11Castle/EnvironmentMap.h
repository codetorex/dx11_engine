#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include "OBJRenderer.h"
#include "EnvironmentShader.h"
#include "Entity.h"

class EnvironmentMap : public Entity
{
public:
	OBJRenderer* m_Renderer;
	Texture* m_Environment;


	EnvironmentMap( Texture* environment )
	{
		m_Environment = environment;
		m_Renderer = new OBJRenderer();
		m_Renderer->Load( L"Media\\Bunny\\bunny.obj" );
	}

	void Render()
	{
		Render( m_Matrix );
	}

	void Render( XMMATRIX& objMat )
	{
		EnvironmentVertexShader* m_VS = EnvironmentVertexShader::Instance;
		EnvironmentPixelShader* m_PS = EnvironmentPixelShader::Instance;

		EnvironmentVertexShader::ProjectionData pd;
		pd.matProjection = XMMatrixTranspose( Engine.m_Renderer.m_Viewport.m_Projection );

		//XMMATRIX matCubeWorld = XMMatrixIdentity();

		XMMATRIX matWVP = objMat * Engine.m_Camera.View * Engine.m_Renderer.m_Viewport.m_Projection;

		EnvironmentVertexShader::ViewData vd;

		XMMATRIX cpy = XMMATRIX( objMat );
		XMVECTOR tmpVec;

		vd.matWorld = cpy;
		vd.matWorldViewProjection = XMMatrixTranspose( matWVP );
		vd.WorldInverseTranspose = XMMatrixTranspose( XMMatrixInverse( &tmpVec, objMat ) );
		vd.cameraPos = Engine.m_Camera.Eye;

		for ( int i = 0; i < m_Renderer->m_VB.m_Parts.size(); i++ )
		{
			auto curPart = m_Renderer->m_VB.m_Parts[i];
			//m_PS->m_Texture = *curPart->m_Texture;
			m_PS->m_CubeMapTexture = *m_Environment;
			m_PS->Render();

			m_VS->m_Vertices = curPart->m_Buffer;
			m_VS->m_Projection = pd;
			m_VS->m_View = vd;
			m_VS->Render();
		}
	}
};


#endif