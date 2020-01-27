#ifndef CASTLE_H
#define CASTLE_H

#include "OBJEntity.h"

class Castle : public OBJEntity
{
public:
	Castle() : OBJEntity( L"Media\\Castle\\Castle.obj" )
	{
	}
};

#endif
