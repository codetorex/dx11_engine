#ifndef NORMALMAP_H
#define NORMALMAP_H

#include "OBJEntity.h"
#include "NormalShader.h"

class NormalMap : public Entity
{
public:
	NormalModel* model;

	NormalMap()
	{
		model = new NormalModel();

		OBJRenderer* objr = new OBJRenderer();
		objr->Load( L"Media\\Horse\\horse.obj" );

		model->CreateFromObjRenderer( objr );
		model->m_Parts[0]->m_Normal = new Texture();
		model->m_Parts[0]->m_Normal->Create( L"Media\\Horse\\bump01.dds" );

		delete objr;
	}

	void Render()
	{
		model->Render( m_Matrix );
		RenderChilds();
	}
};

#endif // !BOX_H
