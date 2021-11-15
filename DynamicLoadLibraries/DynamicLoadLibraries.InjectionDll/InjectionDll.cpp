#include <Windows.h>
#include "../DynamicLoadLibraries.StringReplaceDll/StringReplaceDll.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		ReplaceString("will be", "is now ");
	}
	return TRUE;
}
