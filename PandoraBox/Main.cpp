#include "PandoraBox.h"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    PandoraBox pb;

    pb.CreateRegistryValues();
    pb.ShowPopupBox();

    return 0;
}