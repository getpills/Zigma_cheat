#include <ntifs.h>


#include "../src/header/memory.h"
#include "../src/header/print.h"
#include "../src/header/communication.h"

NTSTATUS driver_main(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{
	UNREFERENCED_PARAMETER(registry_path);

	UNICODE_STRING device_name = {};
	// random name to our device
	RtlInitUnicodeString(&device_name, L"\\Device\\NtQueryCompositionSurfaceStaticsAbout");

	// Create driver device obj.
	PDEVICE_OBJECT device_object = nullptr;
	NTSTATUS status = IoCreateDevice(driver_object, 0, &device_name, FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN, FALSE, &device_object);

	if (status != STATUS_SUCCESS) {
		DebugMessage("[-] Failed to create driver device\n");
		return status;
	}

	DebugMessage("[+] Driver device successfully created.\n");
	
	UNICODE_STRING symbolic_link = {};
	RtlInitUnicodeString(&symbolic_link, L"\\DosDevices\\NtQueryCompositionSurfaceStaticsInfo");

	status = IoCreateSymbolicLink(&symbolic_link, &device_name);
	if (status != STATUS_SUCCESS) {
		DebugMessage("[-] Failed to establish symbolic link.\n");
		return status;
	}

	DebugMessage("[+] Driver symbolic link successfully established.\n");

	// Allow us to send small amounts of data between um/km.
	SetFlag(device_object->Flags, DO_BUFFERED_IO);

	driver_object->MajorFunction[IRP_MJ_CREATE] = driver::create;
	driver_object->MajorFunction[IRP_MJ_CLOSE] = driver::close;
	driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = driver::device_control;

	// We have initialized our device.
	ClearFlag(device_object->Flags, DO_DEVICE_INITIALIZING);

	DebugMessage("[+] Driver initialized successfully.\n");

	return status;
}


// // KdMapper will call this "entry point" but params will be null.
NTSTATUS DriverEntry()
{
	
	DebugMessage("[+]Kernel Hello!\n");

	UNICODE_STRING driver_name = {};
	// lyboe imya moshet bit'
	RtlInitUnicodeString(&driver_name, L"\\Driver\\NtQueryCompositionSurfaceStaticsInfo");

	return IoCreateDriver(&driver_name, &driver_main);
}