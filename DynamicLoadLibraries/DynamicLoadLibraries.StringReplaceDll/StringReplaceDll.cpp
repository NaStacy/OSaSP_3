#include "StringReplaceDll.h"
#include <Windows.h>

__declspec(dllexport) INT64 ReplaceString(LPCSTR lpsToReplace, LPCSTR lpsNewString)
{
	int iReplacementLength = lstrlen(lpsToReplace);
	if ((iReplacementLength == 0) || (iReplacementLength != lstrlen(lpsNewString)))
	{
		return -1;
	}

	HANDLE hCurrentProcess = GetCurrentProcess();
	SYSTEM_INFO siSystemInfo;
	GetSystemInfo(&siSystemInfo);

	CHAR *pcPage = (CHAR *)calloc(siSystemInfo.dwPageSize, sizeof(CHAR));
	DWORD dwLastPageSymbol = siSystemInfo.dwPageSize - iReplacementLength;

	MEMORY_BASIC_INFORMATION mbiPageInfo;
	BOOL bIsStringFound, bShouldWritePage;
	INT64 iReplacedCount = 0;

	for (LPVOID lpPageAddress = siSystemInfo.lpMinimumApplicationAddress; lpPageAddress < siSystemInfo.lpMaximumApplicationAddress; lpPageAddress = (LPVOID)((ULONGLONG)lpPageAddress + siSystemInfo.dwPageSize))
	{
		VirtualQuery(lpPageAddress, &mbiPageInfo, sizeof(MEMORY_BASIC_INFORMATION));

		if ((mbiPageInfo.State == MEM_COMMIT) && (mbiPageInfo.Protect & PAGE_READWRITE))
		{
			if (ReadProcessMemory(hCurrentProcess, lpPageAddress, pcPage, siSystemInfo.dwPageSize, NULL))
			{
				bShouldWritePage = FALSE;

				for (DWORD dwCurPageChar = 0; dwCurPageChar < dwLastPageSymbol; ++dwCurPageChar)
				{
					bIsStringFound = TRUE;
					for (int iCurStrChar = 0; (iCurStrChar < iReplacementLength) && bIsStringFound; ++iCurStrChar)
					{
						bIsStringFound = pcPage[dwCurPageChar + iCurStrChar] == lpsToReplace[iCurStrChar];
					}

					if (bIsStringFound)
					{
						for (int sCurStrChar = 0; sCurStrChar < iReplacementLength; ++sCurStrChar)
						{
							pcPage[dwCurPageChar + sCurStrChar] = lpsNewString[sCurStrChar];
						}
						++iReplacedCount;
						bShouldWritePage = TRUE;
					}
				}

				if (bShouldWritePage)
				{
					WriteProcessMemory(hCurrentProcess, lpPageAddress, pcPage, siSystemInfo.dwPageSize, NULL);
				}
			}
		}
	}

	free(pcPage);
	return iReplacedCount;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}
