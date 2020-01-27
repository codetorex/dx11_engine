#ifndef VERTEXFORMATS_H
#define VERTEXFORMATS_H

#include "Math.h"

class VertexPosition
{
public:
	Vector3f Position;
};

class VertexParticle
{
public:
	Vector3f Position;
	float Size;
	float Alpha;
};

class VertexNormalTextured
{
public:
	Vector3f Position;
	Vector3f Normal;
	Vector2f TexUV;
};

class VertexNormalTexturedTangentBinormal
{
public:
	Vector3f Position;
	Vector3f Normal;
	Vector2f TexUV;
	Vector3f Tangent;
	Vector3f BiNormal;

};

#endif