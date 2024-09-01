#pragma once
#include "../include.h"
#include "../back/getProcessID.h"
#include "../src/back/UMCommunication.h"

// attach
const DWORD pid = get_process_id(L"cs2.exe");

const HANDLE driv = CreateFile(L"\\\\.\\NtQueryCompositionSurfaceStaticsInfo", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

bool attach = driver::attach_to_process(driv, pid);

const uintptr_t client = get_module_base(pid, L"client.dll");

void ezAttach()
{
	if (pid == 0)
	{
		cout << "Failed to found cs2.\n";


	}


	if (driv == INVALID_HANDLE_VALUE) {
		cout << "Failed To Create Driver handle\n";


	}

	
	if (attach != true)
	{
		cout << "Failed to Attach sc2.\n";


	}

	

	if (client == 0)
	{
		cout << "Failed to get module base.\n";


	}

	if (client != 0)
	{
		cout << "Client found.\n";
	}

}

void ezDetach()
{
	CloseHandle(driv);
}