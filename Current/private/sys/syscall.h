#ifndef HC_SYSCALL_H
#define HC_SYSCALL_H

#include "../../public/base.h"
#include "table.h"

#define SYSCALLAPI __cdecl /* indicates that a function performs a system call within asn .asm file */

#define e(x) __asm __emit (x) /* emit an assembly byte. */

#define X64_Start_with_CS(_cs) \
	{ \
	e(0x6A) e(_cs)                  /*  push   _cs             */ \
	e(0xE8) e(0) e(0) e(0) e(0)		/*  call   $+5             */ \
	e(0x83) e(4) e(0x24) e(5)		/*  add    dword [esp], 5  */ \
	e(0xCB)                         /*  retf                   */ \
	}

#define X64_End_with_CS(_cs) \
	{ \
	e(0xE8) e(0) e(0) e(0) e(0)                         /*  call   $+5                   */ \
	e(0xC7) e(0x44) e(0x24) e(4) e(_cs) e(0) e(0) e(0)	/*  mov    dword [rsp + 4], _cs  */ \
	e(0x83) e(4) e(0x24) e(0xD)                         /*  add    dword [rsp], 0xD      */ \
	e(0xCB)                                             /*  retf                         */ \
	}

#define X64_Start() X64_Start_with_CS(0x33)
#define X64_End() X64_End_with_CS(0x23)

#define _RAX  0
#define _RCX  1
#define _RDX  2
#define _RBX  3
#define _RSP  4
#define _RBP  5
#define _RSI  6
#define _RDI  7
#define _R8   8
#define _R9   9
#define _R10 10
#define _R11 11
#define _R12 12
#define _R13 13
#define _R14 14
#define _R15 15

#define X64_Push(r) e(0x48 | ((r) >> 3)) e(0x50 | ((r) & 7))
#define X64_Pop(r) e(0x48 | ((r) >> 3)) e(0x58 | ((r) & 7))

#if defined (__cplusplus)
extern "C" {
#endif

BOOLEAN
HcIsWow64();

#pragma region Native System Call Definitions

NTSTATUS SYSCALLAPI HcQueryInformationToken(CONST IN HANDLE TokenHandle,
	CONST IN TOKEN_INFORMATION_CLASS TokenInformationClass,
	_Out_writes_bytes_(TokenInformationLength) LPVOID TokenInformation,
	CONST IN ULONG TokenInformationLength,
	OUT PULONG ReturnLength);

NTSTATUS SYSCALLAPI HcOpenProcessToken(CONST IN HANDLE hProcess,
	CONST IN ACCESS_MASK DesiredAccess,
	OUT PHANDLE TokenHandle);

NTSTATUS SYSCALLAPI HcResumeProcess(CONST IN HANDLE ProcessHandle);

NTSTATUS SYSCALLAPI HcSuspendProcess(CONST IN HANDLE ProcessHandle);

NTSTATUS SYSCALLAPI HcAllocateVirtualMemory(CONST IN HANDLE hProcess,
	IN LPVOID* UBaseAddress,
	IN ULONG_PTR ZeroBits,
	IN OUT PSIZE_T URegionSize,
	CONST IN ULONG AllocationType,
	CONST IN ULONG Protect);

NTSTATUS SYSCALLAPI HcFreeVirtualMemory(CONST IN HANDLE hProcess,
	IN LPVOID* UBaseAddress,
	IN PSIZE_T URegionSize,
	CONST IN ULONG FreeType);

NTSTATUS SYSCALLAPI HcResumeThread(CONST IN HANDLE ThreadHandle,
	OUT PULONG SuspendCount OPTIONAL);

NTSTATUS SYSCALLAPI HcOpenThread(
	OUT PHANDLE ThreadHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PCLIENT_ID ClientId OPTIONAL);

NTSTATUS SYSCALLAPI HcSuspendThread(
	IN HANDLE ThreadHandle,
	OUT PULONG PreviousSuspendCount OPTIONAL);

NTSTATUS SYSCALLAPI HcQueryInformationThread(CONST IN HANDLE ThreadHandle,
	CONST IN THREADINFOCLASS ThreadInformationClass,
	OUT LPVOID ThreadInformation,
	CONST IN ULONG ThreadInformationLength,
	OUT PULONG ReturnLength OPTIONAL);

NTSTATUS SYSCALLAPI HcCreateThread(OUT PHANDLE ThreadHandle,
	CONST IN ACCESS_MASK			DesiredAccess,
	IN POBJECT_ATTRIBUTES			ObjectAttributes OPTIONAL,
	CONST IN HANDLE					ProcessHandle,
	OUT PCLIENT_ID					ClientId,
	IN PCONTEXT						ThreadContext,
	IN PINITIAL_TEB					InitialTeb,
	CONST IN BOOLEAN				CreateSuspended);

NTSTATUS SYSCALLAPI HcFlushInstructionCache(CONST IN HANDLE ProcessHandle,
	CONST IN LPVOID BaseAddress,
	CONST IN SIZE_T NumberOfBytesToFlush);

NTSTATUS SYSCALLAPI HcOpenProcess(
	OUT			PHANDLE				ProcessHandle,
	CONST IN    ACCESS_MASK			DesiredAccess,
	CONST IN    POBJECT_ATTRIBUTES	ObjectAttributes,
	IN			PCLIENT_ID			ClientId OPTIONAL);

NTSTATUS SYSCALLAPI HcProtectVirtualMemory(CONST IN HANDLE ProcessHandle,
	IN OUT PVOID *BaseAddress,
	IN OUT PSIZE_T NumberOfBytesToProtect,
	CONST IN ULONG NewAccessProtection,
	OUT PULONG OldAccessProtection);

NTSTATUS SYSCALLAPI HcReadVirtualMemory(CONST HANDLE ProcessHandle,
	CONST LPVOID BaseAddress,
	LPVOID Buffer,
	CONST SIZE_T BufferSize,
	PSIZE_T NumberOfBytesRead);

NTSTATUS SYSCALLAPI HcWriteVirtualMemory(CONST HANDLE ProcessHandle,
	CONST LPVOID BaseAddress,
	CONST VOID *Buffer,
	CONST SIZE_T BufferSize,
	PSIZE_T NumberOfBytesWritten);

NTSTATUS SYSCALLAPI HcQueryInformationProcess(
	__in HANDLE ProcessHandle, 
	__in PROCESSINFOCLASS ProcessInformationClass,
	__out_bcount(ProcessInformationLength) LPVOID ProcessInformation,
	__in ULONG ProcessInformationLength,
	__out_opt PULONG ReturnLength);

NTSTATUS SYSCALLAPI HcQuerySystemInformation(
	__in SYSTEM_INFORMATION_CLASS SystemInformationClass,
	__out_bcount_opt(SystemInformationLength) LPVOID SystemInformation,
	__in ULONG SystemInformationLength,
	__out_opt PULONG ReturnLength);

NTSTATUS SYSCALLAPI HcClose(HANDLE hObject);

NTSTATUS SYSCALLAPI HcQueryVirtualMemory(IN HANDLE ProcessHandle,
	IN LPVOID BaseAddress,
	IN MEMORY_INFORMATION_CLASS MemoryInformationClass,
	OUT LPVOID MemoryInformation,
	IN SIZE_T MemoryInformationLength,
	OUT PSIZE_T ReturnLength);

NTSTATUS SYSCALLAPI HcAdjustPrivilegesToken(HANDLE TokenHandle,
	BOOLEAN 	DisableAllPrivileges,
	PTOKEN_PRIVILEGES 	NewState,
	DWORD 	BufferLength,
	PTOKEN_PRIVILEGES 	PreviousState,
	PDWORD 	ReturnLength);

NTSTATUS SYSCALLAPI HcSetInformationThread(IN HANDLE ThreadHandle,
	IN THREADINFOCLASS ThreadInformationClass,
	IN PVOID ThreadInformation,
	IN ULONG ThreadInformationLength);

NTSTATUS SYSCALLAPI HcOpenDirectoryObject(OUT PHANDLE DirectoryHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes);

NTSTATUS SYSCALLAPI HcCreateThreadEx(OUT PHANDLE ThreadHandle,
	IN ACCESS_MASK DesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE ProcessHandle,
	IN PVOID StartRoutine,
	_In_opt_ PVOID Argument,
	IN ULONG CreateFlags,
	_In_opt_ ULONG_PTR ZeroBits,
	_In_opt_ SIZE_T StackSize,
	_In_opt_ SIZE_T MaximumStackSize,
	_In_opt_ PVOID AttributeList);

NTSTATUS SYSCALLAPI HcWaitForSingleObject(IN HANDLE hObject,
	IN BOOLEAN bAlertable,
	IN PLARGE_INTEGER Timeout);

NTSTATUS SYSCALLAPI HcWaitForMultipleObjects(IN ULONG ObjectCount,
	IN PHANDLE HandleArray,
	IN WAIT_TYPE WaitType,
	IN BOOLEAN Alertable,
	IN PLARGE_INTEGER TimeOut OPTIONAL);

NTSTATUS SYSCALLAPI HcUnlockVirtualMemory(IN HANDLE ProcessHandle,
	IN OUT PVOID * BaseAddress,
	IN OUT PSIZE_T NumberOfBytesToUnlock,
	IN ULONG MapType);


NTSTATUS SYSCALLAPI HcLockVirtualMemory(IN HANDLE ProcessHandle,
	IN OUT PVOID * BaseAddress,
	IN OUT PSIZE_T NumberOfBytesToLock,
	IN ULONG MapType);

NTSTATUS SYSCALLAPI HcCreateFile(
	OUT    PHANDLE            FileHandle,
	IN     ACCESS_MASK        DesiredAccess,
	IN     POBJECT_ATTRIBUTES ObjectAttributes,
	OUT    PIO_STATUS_BLOCK   IoStatusBlock,
	_In_opt_ PLARGE_INTEGER     AllocationSize,
	IN     ULONG              FileAttributes,
	IN     ULONG              ShareAccess,
	IN     ULONG              CreateDisposition,
	IN     ULONG              CreateOptions,
	IN     PVOID              EaBuffer,
	IN     ULONG              EaLength
);

NTSTATUS SYSCALLAPI HcQueryInformationFile(
	IN  HANDLE                 FileHandle,
	OUT PIO_STATUS_BLOCK       IoStatusBlock,
	OUT PVOID                  FileInformation,
	IN  ULONG                  Length,
	IN  FILE_INFORMATION_CLASS FileInformationClass
);

NTSTATUS SYSCALLAPI HcQueryVolumeInformationFile(
	IN  HANDLE               FileHandle,
	OUT PIO_STATUS_BLOCK     IoStatusBlock,
	OUT PVOID                FsInformation,
	IN  ULONG                Length,
	IN  FS_INFORMATION_CLASS FsInformationClass
);

NTSTATUS SYSCALLAPI HcQueryObject(
	IN  HANDLE                   Handle OPTIONAL,
	IN  OBJECT_INFORMATION_CLASS ObjectInformationClass,
	OUT PVOID                    ObjectInformation OPTIONAL,
	IN  ULONG                    ObjectInformationLength,
	OUT PULONG                   ReturnLength OPTIONAL
);

NTSTATUS SYSCALLAPI HcDuplicateObject(
	IN HANDLE      SourceProcessHandle,
	IN HANDLE      SourceHandle,
	IN HANDLE      TargetProcessHandle OPTIONAL,
	IN PHANDLE     TargetHandle OPTIONAL,
	IN ACCESS_MASK DesiredAccess,
	IN ULONG       HandleAttributes,
	IN ULONG       Options
);

NTSTATUS SYSCALLAPI HcDelayExecution(IN BOOLEAN Alertable,
	IN PLARGE_INTEGER DelayInterval);

NTSTATUS SYSCALLAPI HcWriteFile(
	IN  HANDLE           FileHandle,
	IN	HANDLE           Event OPTIONAL,
	IN	PIO_APC_ROUTINE  ApcRoutine OPTIONAL,
	IN	PVOID            ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN  PVOID            Buffer,
	IN  ULONG            Length,
	IN	PLARGE_INTEGER   ByteOffset OPTIONAL,
	IN	PULONG           Key OPTIONAL
);

NTSTATUS SYSCALLAPI HcTerminateProcess(
	IN HANDLE   ProcessHandle OPTIONAL,
	IN NTSTATUS ExitStatus
);

NTSTATUS SYSCALLAPI HcDeviceIoControlFile(
	IN HANDLE DeviceHandle,
	IN HANDLE Event OPTIONAL,
	IN PIO_APC_ROUTINE UserApcRoutine OPTIONAL,
	IN PVOID UserApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN ULONG IoControlCode,
	IN PVOID InputBuffer,
	IN ULONG InputBufferLength OPTIONAL,
	OUT PVOID OutputBuffer,
	IN ULONG OutputBufferLength OPTIONAL);

NTSTATUS SYSCALLAPI HcFsControlFile(
	HANDLE FileHandle,
	HANDLE Event,
	PIO_APC_ROUTINE ApcRoutine,
	PVOID ApcContext,
	PIO_STATUS_BLOCK IoStatusBlock,
	ULONG FsControlCode,
	PVOID InputBuffer,
	ULONG InputBufferLength,
	PVOID OutputBuffer,
	ULONG OutputBufferLength);

NTSTATUS SYSCALLAPI HcCreateEvent(
	OUT PHANDLE            EventHandle,
	IN  ACCESS_MASK        DesiredAccess,
	IN	POBJECT_ATTRIBUTES ObjectAttributes,
	IN  EVENT_TYPE         EventType,
	IN  BOOLEAN            InitialState);

NTSTATUS SYSCALLAPI HcCreateMutant(
	OUT PHANDLE            MutantHandle,
	IN  ACCESS_MASK        DesiredAccess,
	IN	POBJECT_ATTRIBUTES ObjectAttributes,
	IN  BOOLEAN            InitialOwner);

NTSTATUS SYSCALLAPI HcOpenThreadToken(
	IN HANDLE ThreadHandle,
	IN ACCESS_MASK DesiredAccess,
	IN BOOLEAN OpenAsSelf,
	OUT PHANDLE TokenHandle);

NTSTATUS SYSCALLAPI HcSetInformationFile(
	IN  HANDLE                 FileHandle,
	OUT PIO_STATUS_BLOCK       IoStatusBlock,
	IN  PVOID                  FileInformation,
	IN  ULONG                  Length,
	IN  FILE_INFORMATION_CLASS FileInformationClass); 

NTSTATUS SYSCALLAPI HcReadFile(
	IN  HANDLE           FileHandle,
	IN  HANDLE           Event OPTIONAL,
	IN  PIO_APC_ROUTINE  ApcRoutine OPTIONAL,
	IN  PVOID            ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID            Buffer,
	IN  ULONG            Length,
	IN  PLARGE_INTEGER   ByteOffset OPTIONAL,
	IN  PULONG           Key OPTIONAL);

NTSTATUS SYSCALLAPI HcWow64QueryInformationProcess64(
	IN HANDLE ProcessHandle,
	IN PROCESSINFOCLASS ProcessInformationClass,
	OUT PVOID ProcessInformation64,
	IN ULONG ProcessInformationLength,
	OUT PULONG ReturnLength OPTIONAL);

NTSTATUS SYSCALLAPI HcWow64ReadVirtualMemory64(
	IN HANDLE ProcessHandle,
	IN PVOID64 BaseAddress OPTIONAL,
	IN PVOID Buffer,
	IN ULONG64 BufferSize,
	OUT PULONGLONG NumberOfBytesRead OPTIONAL);

NTSTATUS SYSCALLAPI HcWow64WriteVirtualMemory64(
	IN HANDLE ProcessHandle,
	IN PVOID64 BaseAddress OPTIONAL,
	IN PVOID Buffer,
	IN ULONG64 BufferSize,
	OUT PULONG64 NumberOfBytesWritten OPTIONAL);

NTSTATUS SYSCALLAPI HcWow64AllocateVirtualMemory64(
	IN  HANDLE   ProcessHandle,
	IN  PULONG64 BaseAddress,
	IN  ULONG64  ZeroBits,
	IN  PULONG64 Size,
	IN  ULONG    AllocationType,
	IN  ULONG    Protection);

NTSTATUS SYSCALLAPI HcFlushBuffersFile(
	IN HANDLE hFile, 
	OUT PIO_STATUS_BLOCK IoStatusBlock);

NTSTATUS SYSCALLAPI HcLoadDriver(
	IN PUNICODE_STRING DriverServiceName);

NTSTATUS SYSCALLAPI HcUnloadDriver(
	IN PUNICODE_STRING DriverServiceName);

NTSTATUS SYSCALLAPI HcOpenKey(
  OUT PHANDLE            KeyHandle,
  IN  ACCESS_MASK        DesiredAccess,
  IN  POBJECT_ATTRIBUTES ObjectAttributes);

NTSTATUS SYSCALLAPI HcOpenKeyEx(
  OUT PHANDLE            KeyHandle,
  IN  ACCESS_MASK        DesiredAccess,
  IN  POBJECT_ATTRIBUTES ObjectAttributes,
  IN  ULONG              OpenOptions);

NTSTATUS SYSCALLAPI HcQueryValueKey(
  IN      HANDLE                      KeyHandle,
  IN      PUNICODE_STRING             ValueName,
  IN      KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
  OUT PVOID                           KeyValueInformation OPTIONAL,
  IN      ULONG                       Length,
  OUT     PULONG                      ResultLength);

NTSTATUS SYSCALLAPI HcSetValueKey(
  IN     HANDLE          KeyHandle,
  IN     PUNICODE_STRING ValueName,
  IN 	 ULONG           TitleIndex OPTIONAL,
  IN     ULONG           Type,
  IN 	 PVOID           Data OPTIONAL,
  IN     ULONG           DataSize);

NTSTATUS SYSCALLAPI HcCreateKey(
  OUT      PHANDLE            KeyHandle,
  IN       ACCESS_MASK        DesiredAccess,
  IN       POBJECT_ATTRIBUTES ObjectAttributes,
  IN       ULONG              TitleIndex,
  IN   	   PUNICODE_STRING    Class OPTIONAL,
  IN       ULONG              CreateOptions,
  OUT  	   PULONG             Disposition OPTIONAL);

NTSTATUS 
SYSCALLAPI 
HcGetContextThread(
	HANDLE ThreadHandle, 
	PCONTEXT Context);

NTSTATUS 
SYSCALLAPI 
HcSetContextThread(
	HANDLE ThreadHandle, 
	PCONTEXT Context);

NTSTATUS 
SYSCALLAPI 
HcSetDebugFilterState(
	ULONG ComponentId, 
	ULONG Level, 
	BOOLEAN State);

NTSTATUS
SYSCALLAPI
HcCreateDebugObject(
	OUT PHANDLE DebugObjectHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN ULONG Flags);

NTSTATUS
SYSCALLAPI
HcClose64(IN PTR_64(HANDLE) hObj);

NTSTATUS
SYSCALLAPI 
HcCreateThreadEx64(OUT PTR_64(PHANDLE) PtrThreadHandle,
	IN ACCESS_MASK DesiredAccess,
	IN PTR_64(POBJECT_ATTRIBUTES) PtrObjectAttributes OPTIONAL,
	IN PTR_64(HANDLE) ProcessHandle,
	IN PTR_64(PVOID) StartRoutine,
	IN PTR_64(PVOID) Argument OPTIONAL,
	IN ULONG CreateFlags,
	IN PTR_64(ULONG_PTR) ZeroBits OPTIONAL,
	IN PTR_64(SIZE_T) StackSize OPTIONAL,
	IN PTR_64(SIZE_T) MaximumStackSize OPTIONAL,
	IN PTR_64(PVOID) AttributeList OPTIONAL);

DWORD64
SYSCALLAPI
HcWow64Syscall(int idx, int argC, ...);

#if defined (__cplusplus)
}
#endif

#endif