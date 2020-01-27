#include "stdafx.h"
#include "ScreenSpaceQuad.h"
#include "Engine.h"

ScreenSpaceQuad* ScreenSpaceQuad::Instance;

void ScreenSpaceQuad::Initialize()
{
	VertexPosition vp;

	vp.Position.Set( -1.0f, -1.0f, 0.0f );
	m_Buffer.AddVertex( vp );

	vp.Position.Set( -1.0f, +1.0f, 0.0f );
	m_Buffer.AddVertex( vp );

	vp.Position.Set( +1.0f, +1.0f, 0.0f );
	m_Buffer.AddVertex( vp );

	vp.Position.Set( -1.0f, -1.0f, 0.0f );
	m_Buffer.AddVertex( vp );

	vp.Position.Set( +1.0f, +1.0f, 0.0f );
	m_Buffer.AddVertex( vp );

	vp.Position.Set( +1.0f, -1.0f, 0.0f );
	m_Buffer.AddVertex( vp );

	m_Buffer.Create();
}