#include "framework.h"
#include "hsProcess.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    hsProcess* pc = new hsProcess(hInstance, 1024, 768);
    pc->Init();
    pc->Update();
    pc->Finalize();
    delete pc;
    return 0;
}
