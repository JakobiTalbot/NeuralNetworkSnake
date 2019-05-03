#include "Application2D.h"
#include <crtdbg.h>

int main() 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// allocation
	auto app = new Application2D();

	// initialise and loop
	app->run("AIE", 768, 768, false);

	// deallocation
	delete app;

	return 0;
}