#pragma once
#include "../src/include.h"

void createConsole()
{

	AllocConsole();

	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	system("title ZIGma Cheats");

	printf("ZIGma Cheats\n");

}

