#pragma once

#include "ntifs.h"

namespace driver {
	namespace codes {
		// Used to set up driver.
		constexpr ULONG attach =
			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x5595, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
		// Read process memory.
		constexpr ULONG read =
			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x5596, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
		// Write process memory.
		constexpr ULONG write =
			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x5597, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);

	}//codes

	// Shared between user mode & kernel mode.
	struct Request {
		HANDLE process_id;

		PVOID target;
		PVOID buffer;

		SIZE_T size;
		SIZE_T return_size;
	};

	NTSTATUS create(PDEVICE_OBJECT device_object, PIRP irp);
	

	NTSTATUS close(PDEVICE_OBJECT device_object, PIRP irp);
	
	
	NTSTATUS device_control(PDEVICE_OBJECT device_object, PIRP irp);
	

}//driver