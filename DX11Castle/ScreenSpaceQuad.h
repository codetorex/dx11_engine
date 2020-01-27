#ifndef SCREENSPACEQUAD_H
#define SCREENSPACEQUAD_H

#include "Buffer.h"
#include "VertexFormats.h"

class ScreenSpaceQuad
{
public:
	static ScreenSpaceQuad* Instance;

	VertexBuffer<VertexPosition> m_Buffer;

	ScreenSpaceQuad()
	{
		Initialize();
	}

	void Initialize();

};

#endif