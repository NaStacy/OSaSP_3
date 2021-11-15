#include "DllExample.h"
#include <Psapi.h>

__declspec(dllexport) INT64 __stdcall Min(INT64 iX, INT64 iY)
{
	return iX < iY ? iX : iY;
}

__declspec(dllexport) INT64 __stdcall Max(INT64 iX, INT64 iY)
{
	return iX < iY ? iY : iX;
}

__declspec(dllexport) LPSTR __stdcall GetCurrentProcessName()
{
	const WORD wNameLength = MAX_PATH + 1;
	HANDLE hCurrentProcess = GetCurrentProcess();
	LPSTR lpsProcessName = new CHAR[wNameLength]{ '\0' };
	if (GetModuleBaseName(hCurrentProcess, NULL, lpsProcessName, wNameLength) == 0)
	{
		delete[] lpsProcessName;
		return NULL;
	}
	else
	{
		return lpsProcessName;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}
