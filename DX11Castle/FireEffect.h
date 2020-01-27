#ifndef FIREEFFECT_H
#define FIREEFFECT_H

#include "Entity.h"
#include "ParticleEmitter.h"

class FireEffect : public Entity
{
public:
	ParticleEmitter m_Emitter;

	FireEffect()
	{
		Initialize();
	}

	FireEffect( float x, float y, float z )
	{
		this->m_Position = XMVectorSet( x, y, z, 1 );
		Initialize();
	}

	void Initialize()
	{
		m_Emitter.m_Emitting = true;
		m_Emitter.m_ParticleStartingSize = 20.0f;
		m_Emitter.m_ParticleStartingSpeed = 20.0f;
		m_Emitter.m_AlphaChangePerSecond = -0.2f;
		m_Emitter.m_EmissionTime = 0.1;
		m_Emitter.m_SizeChangePerSecond = 5.0;
		m_Emitter.m_Texture.Create( L"Media\\Particles\\Fire.tif" );
		m_Emitter.SetEmittingCone( 60.0f );
	}


	void Update()
	{
		m_Emitter.m_Position = m_Position;
		m_Emitter.Update();
		this->Entity::Update();
	}

	void Render()
	{
		m_Emitter.Render();
	}
};

#endif