#include "StartUp.h"

#include <crtdbg.h>

#include "GampApp.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,_In_ LPSTR cmdLine, _In_ int showCmd)
{

   
    UNREFERENCED_PARAMETER(prevInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    UNREFERENCED_PARAMETER(showCmd);
  

	//Run test of MemoryLeak
	#if defined(DEBUG) | defined(_DEBUG)
	    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	GameApp theApp(hInstance, L"DirectX11 Initialization", 1280, 720);

	if (!theApp.Init())
            return 0;

	return theApp.Run();

	
}
