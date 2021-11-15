#include <Windows.h>
#include <stdio.h>

extern "C"
{
	__declspec(dllimport) INT64 __stdcall Min(INT64 iX, INT64 iY);

	__declspec(dllimport) INT64 __stdcall Max(INT64 iX, INT64 iY);

	__declspec(dllimport) LPSTR __stdcall GetCurrentProcessName();
}

int main()
{
	INT64 iX, iY;
	LPSTR lpsProcessName = GetCurrentProcessName();

	if (lpsProcessName == NULL)
	{
		printf_s("Error getting current process name\n");
	}
	else
	{
		printf_s("Current process name: %s\n", lpsProcessName);
		delete[] lpsProcessName;
	}

	printf_s("Enter x: ");
	scanf_s("%I64d", &iX);
	printf_s("Enter y: ");
	scanf_s("%I64d", &iY);

	printf_s("Minimum of x and y: %I64d\nMaximum of x and y: %I64d\n", Min(iX, iY), Max(iX, iY));
	system("pause");
}
