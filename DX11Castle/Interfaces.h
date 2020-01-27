#ifndef INTERFACES_H
#define INTERFACES_H

class IRenderable
{
public:
	virtual void Render() = 0;
};

class IUpdateable
{
public:
	virtual void Update() = 0;
};

class IResource
{
public:
	virtual void Release() = 0;
};

#endif