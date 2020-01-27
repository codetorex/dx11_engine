#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "OBJEntity.h"

class CubeMap : public OBJEntity
{
public:
	D3D11_DEPTH_STENCIL_DESC m_StencilDescription;
	ID3D11DepthStencilState* m_DepthStencil;

	CubeMap() : OBJEntity( L"Media\\Other\\cubemap.obj" )
	{
		ZeroMemory( &m_StencilDescription, sizeof(D3D11_DEPTH_STENCIL_DESC) );
		m_StencilDescription.DepthEnable = FALSE;
		Engine.m_Renderer.m_Device->CreateDepthStencilState( &m_StencilDescription, &m_DepthStencil );
	}

	void Update()
	{
		m_Position = Engine.m_Camera.Eye;
		this->OBJEntity::Update();
	}

	void Render()
	{
		CubeMapVertexShader::ViewData vd;
		vd.matWorld = XMMatrixTranspose( m_Matrix );
		vd.matWorldViewProjection = XMMatrixTranspose( m_Matrix * Engine.m_Camera.View * Engine.m_Renderer.m_Viewport.m_Projection );

		auto curPart = m_Renderer->m_VB.m_Parts[0];
		CubeMapPixelShader::Instance->m_CubeMapTexture = *curPart->m_Texture;
		CubeMapPixelShader::Instance->Render();

		CubeMapVertexShader::Instance->m_Vertices = curPart->m_Buffer;
		CubeMapVertexShader::Instance->m_View = vd;

		ID3D11DepthStencilState* oldDepthStencil = NULL;
		UINT depthStencilRef = 0;
		Engine.m_Renderer.m_Context->OMGetDepthStencilState( &oldDepthStencil, &depthStencilRef );
		Engine.m_Renderer.m_Context->OMSetDepthStencilState( m_DepthStencil, 0 );
		CubeMapVertexShader::Instance->Render();
		Engine.m_Renderer.m_Context->OMSetDepthStencilState( oldDepthStencil, depthStencilRef );
	}
};

#endif