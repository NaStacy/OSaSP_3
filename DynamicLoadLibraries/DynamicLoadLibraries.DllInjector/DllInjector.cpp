#include <Windows.h>
#include <stdio.h>

int main()
{
	DWORD dwProcessId;
	printf_s("Enter process ID: ");
	scanf_s("%d", &dwProcessId);

	HANDLE hInjectedProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hInjectedProcess == NULL)
	{
		printf("Error opening process with ID %d\n", dwProcessId);
	}
	else
	{
		LPCSTR lpsDllName = "DynamicLoadLibraries.InjectionDll.dll";
		int iDllNameLength = lstrlen(lpsDllName);
		LPVOID lpInjectionLibConatiner = VirtualAllocEx(hInjectedProcess, NULL, (iDllNameLength + 1) * sizeof(CHAR), MEM_COMMIT, PAGE_READWRITE);

		if (lpInjectionLibConatiner == NULL)
		{
			printf("Error creating memory page in injected process\n");
		}
		else
		{
			if (WriteProcessMemory(hInjectedProcess, lpInjectionLibConatiner, lpsDllName, (iDllNameLength + 1) * sizeof(CHAR), NULL))
			{
				if (CreateRemoteThread(hInjectedProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, lpInjectionLibConatiner, 0, NULL) == NULL)
				{
					printf("Error creating remote thread\n");
				}
			}
			else
			{
				printf("Error writing in remote process memory page\n");
			}
		}
		CloseHandle(hInjectedProcess);
	}

	system("pause");
	return 0;
}
