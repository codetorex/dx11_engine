#ifndef LIGHTPOINTER_H
#define LIGHTPOINTER_H

#include "OBJEntity.h"

class LightPointer : public OBJEntity
{
public:
	LightPointer() : OBJEntity( L"Media\\Light\\light.obj" )
	{
	}

	void Update()
	{
		m_Position = Engine.m_Light.m_Position;
		m_Scale = Vector3f( 20, 20, 20 );
		this->Entity::Update();
	}

	void Render()
	{
		Engine.m_Renderer.EnableBlending( D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA );

		this->OBJEntity::Render();

		Engine.m_Renderer.DisableBlending();
	}
};

#endif
