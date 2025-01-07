#include "StartUp.h"
#include <crtdbg.h>
#include "GameApp.h"


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,_In_ LPSTR cmdLine, _In_ int showCmd)
{

    UNREFERENCED_PARAMETER(prevInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    UNREFERENCED_PARAMETER(showCmd);

	//Run test of MemoryLeak
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_ASSERT(_CrtCheckMemory());
#endif


	// Create Window
	GameApp::Get()->InitDX(hInstance, L"My Blender", static_cast<int>(WIN_WIDTH), static_cast<int>(WIN_HEIGHT));

	// Init GameApp
	if (!GameApp::Get()->Init())
		return 0;

	return GameApp::Get()->Run();

	

}
