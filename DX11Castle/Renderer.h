#ifndef RENDERER_H
#define RENDERER_H

#include "Interfaces.h"
#include "Viewport.h"

#include "Window.h"
#include "Color.h"

#include <D3DX11.h>
#include "RenderTarget.h"

class DiretX11Window : public Window
{
public:
	bool m_Keys[256];

	LRESULT Procedure( UINT msg, WPARAM wParam, LPARAM lParam );

};

class Renderer : public IResource
{
private:
	float LastFrameMeasurement;
	int Frame;

protected:
	void RenderList();

public:

	Viewport m_Viewport;
	vector<IRenderable*> m_Renderables;
	DiretX11Window m_Window;
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_Context;
	IDXGISwapChain* m_SwapChain;
	D3D_FEATURE_LEVEL m_FeatureLevel;
	D3D_DRIVER_TYPE m_DriverType;

	ID3D11RenderTargetView* g_pRenderTargetView;
	ID3D11Texture2D* g_pDepthStencil;
	ID3D11DepthStencilView* g_pDepthStencilView;
	int FPS;

	void AddRenderable( IRenderable& item )
	{
		m_Renderables.push_back( &item );
	}

	void RemoveRenderable( IRenderable& item )
	{
		removeFromArray( m_Renderables, &item );
	}


	Color4f ClearColor;

	Renderer()
	{
		m_Device = NULL;
		m_Context = NULL;
		m_SwapChain = NULL;
		m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
		m_DriverType = D3D_DRIVER_TYPE_NULL;
		g_pRenderTargetView = NULL;
		g_pDepthStencil = NULL;
		g_pDepthStencilView = NULL;

		LastFrameMeasurement = 0.0f;
		FPS = 0;
		Frame = 0;

		ClearColor.Set( 0.0f, 0.125f, 0.3f, 1.0f );
	}

	void Release()
	{
		if ( m_Context ) m_Context->ClearState();

		if ( g_pDepthStencil ) g_pDepthStencil->Release();
		if ( g_pDepthStencilView ) g_pDepthStencilView->Release();
		if ( g_pRenderTargetView ) g_pRenderTargetView->Release();
		if ( m_SwapChain ) m_SwapChain->Release();
		if ( m_Context ) m_Context->Release();
		if ( m_Device ) m_Device->Release();
	}

	void EnableBlending( D3D11_BLEND src, D3D11_BLEND dst );
	void DisableBlending();

	ID3D11RasterizerState*  CreateRasterizerState( bool cull, bool wireframe );
	bool CreateRenderTarget( int width, int height, RenderTarget& result );

	ID3D11DepthStencilView* GetDepthStencilView()
	{
		return g_pDepthStencilView;
	}

	void SetBackBufferRenderTarget()
	{
		// Bind the render target view and depth stencil buffer to the output render pipeline.
		m_Context->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );
	}

	void Initialize( int width, int height );
	void Render();
};

#endif