#include <Windows.h>
#include <ShellAPI.h>

extern __declspec(dllexport) void LaunchCalc();
void LaunchCalc() {
    ShellExecute(NULL, "open", "calc.exe", NULL, NULL, SW_SHOWDEFAULT);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    
	switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
		LaunchCalc();
		break;
	case DLL_THREAD_ATTACH:
			break;
	case DLL_THREAD_DETACH:
			break;
	case DLL_PROCESS_DETACH:
			break;
    }
	
    return TRUE;
}
