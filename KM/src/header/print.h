#pragma once
#include <ntifs.h>

#define DebugMessage(x, ...) DbgPrintEx(0, 0, x, __VA_ARGS__);
