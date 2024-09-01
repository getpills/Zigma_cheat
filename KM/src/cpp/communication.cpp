#include "../header/communication.h"
#include "../header/print.h"
#include "../header/memory.h"

NTSTATUS driver::create(PDEVICE_OBJECT device_object, PIRP irp)
{
	UNREFERENCED_PARAMETER(device_object);

	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return irp->IoStatus.Status;
}

NTSTATUS driver::close(PDEVICE_OBJECT device_object, PIRP irp)
{
	UNREFERENCED_PARAMETER(device_object);

	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return irp->IoStatus.Status;
}

NTSTATUS driver::device_control(PDEVICE_OBJECT device_object, PIRP irp)
{
	UNREFERENCED_PARAMETER(device_object);

	DebugMessage("[+] Device control called.\n");

	NTSTATUS status = STATUS_UNSUCCESSFUL;

	// We need this to determine which code was passed through.
	PIO_STACK_LOCATION stack_irp = IoGetCurrentIrpStackLocation(irp);

	// Access the request object sent from user mode.
	auto request = reinterpret_cast<Request*>(irp->AssociatedIrp.SystemBuffer);

	if (stack_irp == nullptr || request == nullptr) {
		IoCompleteRequest(irp, IO_NO_INCREMENT);
		return status;
	}

	// The target process we want access to.
	static PEPROCESS targer_process = nullptr;

	const ULONG control_code = stack_irp->Parameters.DeviceIoControl.IoControlCode;
	switch (control_code)
	{
	case codes::attach:
		status = PsLookupProcessByProcessId(request->process_id, &targer_process);
		break;

	case codes::read:
		if (targer_process != nullptr)
			status = MmCopyVirtualMemory(targer_process, request->target,
				PsGetCurrentProcess(), request->buffer,
				request->size, KernelMode, &request->return_size);
		break;

	case codes::write:
		if (targer_process != nullptr)
			status = MmCopyVirtualMemory(PsGetCurrentProcess(), request->buffer,
				targer_process, request->target,
				request->size, KernelMode, &request->return_size);
		break;

	default:
		break;
	}

	irp->IoStatus.Status = status;
	irp->IoStatus.Information = sizeof(Request);

	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return status;
}