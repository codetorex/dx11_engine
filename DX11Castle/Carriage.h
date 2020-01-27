#ifndef CARRIAGE_H
#define CARRIAGE_H

#include "OBJEntity.h"
#include "GlowShader.h"

class Carriage : public OBJEntity
{
public:
	Carriage() : OBJEntity( L"Media\\Carriage\\carriage.obj" )
	{
	}
};

#endif
