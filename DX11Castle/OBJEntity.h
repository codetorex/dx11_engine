#ifndef OBJENTITY_H
#define OBJENTITY_H

#include "OBJRenderer.h"
#include "Entity.h"


class OBJEntity : public Entity
{
public:
	OBJRenderer* m_Renderer;

	OBJEntity( OBJRenderer* existingModel )
	{
		m_Renderer = existingModel;
	}

	OBJEntity( const std::wstring& objPath )
	{
		m_Renderer = new OBJRenderer();
		m_Renderer->Load( objPath );
	}

	int IntersectRay( Ray3f& ray, Vector3f& intersection );
	int IntersectRay2( Ray3f& ray, Vector3f& intersection, float length );

	void Render()
	{
		m_Renderer->Render( m_Matrix );
		RenderChilds();
	}
};



class OBJEntityCollider : public ICollidable
{
public:
	OBJEntity* m_Entity;

	int IntersectRay( Ray3f& ray, Vector3f& intersection )
	{
		return m_Entity->IntersectRay( ray, intersection );
	}

};

#endif