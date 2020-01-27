#ifndef ENGINE_H
#define ENGINE_H

#include "Renderer.h"
#include "FileSystem.h"
#include "Updater.h"
#include "TimeManager.h"
#include "Camera.h"
#include "Light.h"

class EngineFacade
{
public:
	Renderer m_Renderer;
	Updater	m_Updater;
	TimeManager m_Time;
	Camera m_Camera;
	Light m_Light;

	EngineFacade();

	void Run();
};

extern EngineFacade Engine;

#endif