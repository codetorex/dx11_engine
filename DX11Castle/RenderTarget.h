#ifndef RENDERTARGET_H
#define RENDERTARGET_H


#include <D3DX11.h>

class Texture;

class RenderTarget
{
public:
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;

	Texture* m_Texture;

	void Begin( bool useDepth = true );

	void Clear( float r, float g, float b, float a, bool useDepth );

	void Clear( bool useDepth = true )
	{
		Clear( 0, 0, 0, 0, useDepth );
	}

	void End();
};




#endif

