// DX11Castle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Engine.h"

#include "OBJEntity.h"
#include "Terrain.h"
#include "CubeMapShader.h"
#include "NormalShader.h"
#include "EnvironmentShader.h"
#include "WaterShader.h"
#include "CubeMap.h"
#include "Box.h"
#include "Tree.h"
#include "NormalMap.h"
#include "EnvironmentMap.h"
#include "Water.h"
#include "Castle.h"
#include "FireEffect.h"
#include "Buffer.h"
#include "Shader.h"
#include "LightPointer.h"
#include "Carriage.h"
#include "GlowShader.h"
#include "ScreenSpaceQuad.h"
#include "Glow.h"

class TreeManager : public IUpdateable, public IRenderable
{
public:

	vector< Tree* > trees;
	int treeCount;

	TreeManager()
	{
		treeCount = 0;
	}

	void CreateTree( Vector3f hpos )
	{
		Tree* h = new Tree( hpos.x, hpos.y, hpos.z );
		h->Update();
		trees.push_back( h );
	}

	virtual void Update()
	{
		for ( int i = 0; i < trees.size(); i++ )
		{
			trees[i]->Update();
		}
	}

	virtual void Render()
	{
		Engine.m_Renderer.EnableBlending( D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA );

		for ( int i = 0; i < trees.size(); i++ )
		{
			trees[i]->Render();
		}

		Engine.m_Renderer.DisableBlending();
	}

};

std::wstring filteringMode = L"Linear";

class FPSUpdater : public IUpdateable
{
public:

	virtual void Update()
	{
		std::wstringstream stringStream;
		stringStream << L" Rendering Techniques - FPS:";
		stringStream << Engine.m_Renderer.FPS;
		stringStream << L" Mode:";
		stringStream << filteringMode;

		Engine.m_Renderer.m_Window.Text( stringStream.str() );
	}
};


class LightUpdater : public IUpdateable
{
public:

	bool m_Enabled;
	float m_Range;
	float m_Height;

	LightUpdater()
	{
		m_Range = 500.0f;
		m_Height = 300.0f;
		m_Enabled = true;
	}

	void Update()
	{
		if ( !m_Enabled ) return;

		float factor = Engine.m_Time.m_Time / 4.0f;

		float lightX = sin( factor ) * m_Range;
		float lightZ = cos( factor ) * m_Range;

		Engine.m_Light.m_Position.Set( lightX, m_Height, lightZ );
	}
} g_LightUpdater;


class TextureSamplerChanger : public IUpdateable
{
public:
	float LastTime;

	PSSampler* sampler;

	TextureSamplerChanger()
	{
		LastTime = 0;
		sampler = NULL;
	}

	void SetShaderSamplers()
	{
		TexturedPixelShader::Instance->m_TextureSampler.m_Sampler = sampler;
		CubeMapPixelShader::Instance->m_TextureSampler.m_Sampler = sampler;
		ParticlePixelShader::Instance->m_TextureSampler.m_Sampler = sampler;
		NormalPixelShader::Instance->m_TextureSampler.m_Sampler = sampler;
		EnvironmentPixelShader::Instance->m_TextureSampler.m_Sampler = sampler;
		WaterPixelShader::Instance->m_TextureSampler.m_Sampler = sampler;
		LastTime = Engine.m_Time.m_Time;
	}

	virtual void Update()
	{
		if ( Engine.m_Time.m_Time - LastTime < 1.0f )
			return;

		if ( sampler == NULL )
		{
			sampler = new PSSampler();
		}

		if ( Engine.m_Renderer.m_Window.m_Keys[VK_F1] )
		{
			sampler->Create( D3D11_FILTER_MIN_MAG_MIP_POINT );
			filteringMode = L"Point";
			SetShaderSamplers();
		}
		else if ( Engine.m_Renderer.m_Window.m_Keys[VK_F2] )
		{
			sampler->Create( D3D11_FILTER_MIN_MAG_MIP_LINEAR );
			filteringMode = L"Linear";
			SetShaderSamplers();
		}
		else if ( Engine.m_Renderer.m_Window.m_Keys[VK_F3] )
		{
			sampler->Create( D3D11_FILTER_ANISOTROPIC, 2 );
			filteringMode = L"2x Anisotropic";
			SetShaderSamplers();
		}
		else if ( Engine.m_Renderer.m_Window.m_Keys[VK_F4] )
		{
			sampler->Create( D3D11_FILTER_ANISOTROPIC, 4 );
			filteringMode = L"4x Anisotropic";
			SetShaderSamplers();
		}
		else if ( Engine.m_Renderer.m_Window.m_Keys['1'] )
		{
			Engine.m_Light.m_Power += 10000.0f;
			LastTime = Engine.m_Time.m_Time;
		}
		else if ( Engine.m_Renderer.m_Window.m_Keys['2'] )
		{
			Engine.m_Light.m_Power -= 10000.0f;
			LastTime = Engine.m_Time.m_Time;
		}
		else if ( Engine.m_Renderer.m_Window.m_Keys[VK_F5] )
		{
			g_LightUpdater.m_Range *= 2.0f;
			g_LightUpdater.m_Height *= 2.0f;
			Engine.m_Light.m_Power *= 4.0f;
			LastTime = Engine.m_Time.m_Time;
		}
		else if ( Engine.m_Renderer.m_Window.m_Keys[VK_F6] )
		{
			g_LightUpdater.m_Range /= 2.0f;
			g_LightUpdater.m_Height /= 2.0f;
			Engine.m_Light.m_Power /= 4.0f;
			LastTime = Engine.m_Time.m_Time;
		}
		else if ( Engine.m_Renderer.m_Window.m_Keys[VK_F7] )
		{
			g_LightUpdater.m_Enabled = !g_LightUpdater.m_Enabled;
			LastTime = Engine.m_Time.m_Time;
		}
	}
};



int _tmain( int argc, _TCHAR* argv[] )
{
	Engine.m_Renderer.Initialize( 800, 600 );

	Engine.m_Camera.Set( 400.0f, 100.0f, 400.0f, 0.0f, 25.0f, 0.0f, 0.0f, 1.0f, 0.0f );

	TexturedVertexShader::Instance = new TexturedVertexShader();
	TexturedPixelShader::Instance = new TexturedPixelShader();

	CubeMapVertexShader::Instance = new CubeMapVertexShader();
	CubeMapPixelShader::Instance = new CubeMapPixelShader();

	ParticleVertexShader::Instance = new ParticleVertexShader();
	ParticleGeometryShader::Instance = new ParticleGeometryShader();
	ParticlePixelShader::Instance = new ParticlePixelShader();

	NormalVertexShader::Instance = new NormalVertexShader();
	NormalPixelShader::Instance = new NormalPixelShader();

	EnvironmentVertexShader::Instance = new EnvironmentVertexShader();
	EnvironmentPixelShader::Instance = new EnvironmentPixelShader();

	WaterVertexShader::Instance = new WaterVertexShader();
	WaterPixelShader::Instance = new WaterPixelShader();

	GlowVertexShader::Instance = new GlowVertexShader();
	GlowPixelShaderVPS::Instance = new GlowPixelShaderVPS();
	GlowPixelShaderHPS::Instance = new GlowPixelShaderHPS();

	ScreenSpaceQuad::Instance = new ScreenSpaceQuad();

	CameraTPSController g_MainCameraController( Engine.m_Camera );
	CameraTPSControllerInput caminput( g_MainCameraController, Engine.m_Renderer.m_Window.m_Keys );



	Terrain terrain;
	CubeMap cubeMap;
	NormalMap normalMapTest;
	EnvironmentMap environmentMapTest( cubeMap.m_Renderer->m_VB.m_Parts[0]->m_Texture );
	Water water( environmentMapTest.m_Environment );
	Castle castle;
	FireEffect fireEffect( 271, 108, -231 );
	FPSUpdater fpsUpdater;
	TextureSamplerChanger textureSamplerChanger;
	LightPointer lightPointer;
	Carriage carriage;
	Glow glow;

	TreeManager treeManager;
	treeManager.CreateTree( Vector3f( -9.516f, 30.616f, 168.496f ) );
	treeManager.CreateTree( Vector3f( 37.928f, 31.611f, 228.864f ) );
	treeManager.CreateTree( Vector3f( -87.975f, 54.139f, 277.448f ) );
	treeManager.CreateTree( Vector3f( -33.819f, 35.741f, 223.925f ) );

	glow.AddGlowEntity( &carriage );
	glow.AddGlowEntity( &castle );

	Engine.m_Updater.AddUpdatable( caminput );
	Engine.m_Updater.AddUpdatable( g_MainCameraController );
	Engine.m_Updater.AddUpdatable( g_LightUpdater );

	Engine.m_Updater.AddUpdatable( cubeMap );
	Engine.m_Updater.AddUpdatable( normalMapTest );
	Engine.m_Updater.AddUpdatable( environmentMapTest );
	Engine.m_Updater.AddUpdatable( terrain );
	Engine.m_Updater.AddUpdatable( castle );
	Engine.m_Updater.AddUpdatable( water );
	Engine.m_Updater.AddUpdatable( fireEffect );
	Engine.m_Updater.AddUpdatable( treeManager );
	Engine.m_Updater.AddUpdatable( fpsUpdater );
	Engine.m_Updater.AddUpdatable( textureSamplerChanger );
	Engine.m_Updater.AddUpdatable( lightPointer );
	Engine.m_Updater.AddUpdatable( carriage );
	Engine.m_Updater.AddUpdatable( glow );

	// render
	Engine.m_Renderer.AddRenderable( cubeMap );
	Engine.m_Renderer.AddRenderable( fireEffect );
	Engine.m_Renderer.AddRenderable( terrain );
	Engine.m_Renderer.AddRenderable( normalMapTest );
	Engine.m_Renderer.AddRenderable( environmentMapTest );
	Engine.m_Renderer.AddRenderable( castle );
	Engine.m_Renderer.AddRenderable( water );
	Engine.m_Renderer.AddRenderable( treeManager );
	Engine.m_Renderer.AddRenderable( lightPointer );
	Engine.m_Renderer.AddRenderable( carriage );
	Engine.m_Renderer.AddRenderable( glow );

	Engine.Run();

	return 0;
}

