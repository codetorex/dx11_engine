#ifndef HOUSE_H
#define HOUSE_H

#include "OBJEntity.h"

class Tree : public OBJEntity
{
public:

	static OBJRenderer* globalTree;

	Tree( float x, float y, float z ) : OBJEntity( nullptr )
	{
		if ( globalTree == nullptr )
		{
			globalTree = new OBJRenderer();
			globalTree->Load( L"Media\\Tree\\tree.obj" );
		}

		m_Renderer = globalTree;

		m_Position = XMVectorSet( x, y, z, 0.0f );
	}

	void Update()
	{

		this->Entity::Update();
		//this->m_Matrix._12 = cos( Engine.m_Time.m_Time ) / 15.0f;
		float factor = Engine.m_Time.m_Time / 2.0f;

		this->m_Matrix._13 = cos( factor ) / 15.0f;
		this->m_Matrix._23 = sin( factor ) / 10.0f;
		//this->m_Matrix._32 = sin( Engine.m_Time.m_Time ) / 7.0f;
	}
};

#endif // !HOUSE_H

