#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#include "../../pch.h"

class ContextManager
{

};

class GraphicsContext
{
public:

	static GraphicsContext* Initialize(std::string name_ = "");

	GraphicsContext() {}
	~GraphicsContext();

private:
};

#endif