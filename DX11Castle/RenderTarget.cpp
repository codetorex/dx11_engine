#include "stdafx.h"
#include "RenderTarget.h"
#include "Engine.h"

void RenderTarget::Begin( bool useDepth )
{

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	if ( useDepth )
	{
		Engine.m_Renderer.m_Context->OMSetRenderTargets( 1, &m_renderTargetView, Engine.m_Renderer.GetDepthStencilView() );
	}
	else
	{
		Engine.m_Renderer.m_Context->OMSetRenderTargets( 1, &m_renderTargetView, NULL );
	}
}

void RenderTarget::End()
{
	Engine.m_Renderer.SetBackBufferRenderTarget();
}

void RenderTarget::Clear( float r, float g, float b, float a, bool useDepth )
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	// Clear the back buffer.
	Engine.m_Renderer.m_Context->ClearRenderTargetView( m_renderTargetView, color );

	if ( useDepth )
	{
		// Clear the depth buffer.
		Engine.m_Renderer.m_Context->ClearDepthStencilView( Engine.m_Renderer.GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
	}
}
