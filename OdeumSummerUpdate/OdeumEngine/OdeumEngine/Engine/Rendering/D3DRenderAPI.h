#ifndef D3DRENDERAPI_H
#define D3DRENDERAPI_H

#include "D3DCore.h"

using namespace DXGraphics;

/* D3DRenderAPI
	This is the entry point for directx graphics. An engine user will only need to access this point for graphics related work
	The goal is to thread work here, but in a non-visible way to the user
	
	The entry point will allow the user to:
		manage graphics contexts
		manage graphics resources
		allocate buffers
		submit resources
		signal drawing
*/

class D3DRenderAPI // will probably inherit from some abstract interface
{
public:

	D3DRenderAPI();
	~D3DRenderAPI();

private:

};

#endif