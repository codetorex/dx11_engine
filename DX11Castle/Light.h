#ifndef LIGHT_H
#define LIGHT_H

#include "Math.h"

class Light
{
public:
	Vector3f m_Position;
	float m_Power;
	bool m_UseDistance;

	Light()
	{
		m_Position.Set( -500, 500, 500 );
		m_Power = 500000.0f;
		m_UseDistance = true;
	}


};

#endif