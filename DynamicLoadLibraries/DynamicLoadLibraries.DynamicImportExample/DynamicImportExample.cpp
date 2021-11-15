#include <Windows.h>
#include <stdio.h>

typedef INT64 (__stdcall Min_ptr)(INT64 x, INT64 y);

typedef INT64(__stdcall Max_ptr)(INT64 x, INT64 y);

typedef LPSTR(__stdcall GetCurrentProcessName_ptr)();

LPCSTR GetFullFunctionName(LPCSTR lpsFunctionName, size_t iArgumentsSize)
{
	UINT uiNameLength = lstrlen(lpsFunctionName) + 2;
	size_t sSizeCounter = iArgumentsSize;
	do
	{
		sSizeCounter /= 10;
		++uiNameLength;
	} while (sSizeCounter > 0);

	LPSTR result = new CHAR[uiNameLength + 1];
	if (sprintf_s(result, uiNameLength + 1, "_%s@%d", lpsFunctionName, iArgumentsSize) == -1)
	{
		delete[] result;
		return NULL;
	}
	else
	{
		return result;
	}
}

int main()
{
	LPCTSTR lpsDllPath = "DynamicLoadLibraries.DllExample.dll";
	HMODULE hLoadedDll = LoadLibrary(lpsDllPath);

	if (hLoadedDll == NULL)
	{
		printf("Error loading library %s\n", lpsDllPath);
	}
	else
	{
		LPCSTR lpsMinDecorated = GetFullFunctionName("Min", 2 * sizeof(INT64));
		LPCSTR lpsMaxDecorated = GetFullFunctionName("Max", 2 * sizeof(INT64));
		LPCSTR lpsGetCurrentProcessNameDecorated = GetFullFunctionName("GetCurrentProcessName", 0);

		Min_ptr *lpMin = (Min_ptr *)GetProcAddress(hLoadedDll, lpsMinDecorated);
		Max_ptr *lpMax = (Max_ptr *)GetProcAddress(hLoadedDll, lpsMaxDecorated);
		GetCurrentProcessName_ptr *lpGetCurrentProcessName = (GetCurrentProcessName_ptr *)GetProcAddress(hLoadedDll, lpsGetCurrentProcessNameDecorated);

		delete[] lpsMinDecorated;
		delete[] lpsMaxDecorated;
		delete[] lpsGetCurrentProcessNameDecorated;

		if ((lpMin == NULL) || (lpMax == NULL))
		{
			printf("Error loading Min or Max functions\n");
		}
		else
		{
			INT64 iX, iY;

			printf_s("Enter x: ");
			scanf_s("%I64d", &iX);
			printf_s("Enter y: ");
			scanf_s("%I64d", &iY);

			printf_s("Minimum of x and y: %I64d\nMaximum of x and y: %I64d\n", lpMin(iX, iY), lpMax(iX, iY));
		}

		if (lpGetCurrentProcessName == NULL)
		{
			printf("Error GetCurrentProcessName function\n");
		}
		else
		{
			LPSTR lpsProcessName = lpGetCurrentProcessName();

			if (lpsProcessName == NULL)
			{
				printf_s("Error getting current process name\n");
			}
			else
			{
				printf_s("Current process name: %s\n", lpsProcessName);
				delete[] lpsProcessName;
			}
		}

		FreeLibrary(hLoadedDll);
	}
	system("pause");
}
