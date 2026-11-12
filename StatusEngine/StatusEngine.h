#pragma comment(lib, "ntdll.lib")

extern "C" {
	struct RTL_CRITICAL_SECTION {
		void* DebugInfo;
		int32_t LockCount;
		int32_t RecursionCount;
		void* OwningThread;
		void* LockSemaphore;
		uint32_t SpinCount;
	};

	struct CLIENT_ID {
		void* UniqueProcess;
		void* UniqueThread;
	};

	struct OBJECT_ATTRIBUTES {
		uint32_t Length;
		void* RootDirectory;
		void* ObjectName;
		uint32_t Attributes;
		void* SecurityDescriptor;
		void* SecurityQualityOfService;
	};

	struct PROCESS_BASIC_INFORMATION {
		void* ExitStatus;
		void* PebBaseAddress;
		unsigned long long AffinityMask;
		int BasePriority;
		unsigned long long UniqueProcessId;
		unsigned long long InheritedFromUniqueProcessId;
	};

	typedef int32_t NTSTATUS;
	#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

	__declspec(dllimport) void __stdcall RtlInitializeCriticalSection(RTL_CRITICAL_SECTION*);
	__declspec(dllimport) void __stdcall RtlEnterCriticalSection(RTL_CRITICAL_SECTION*);
	__declspec(dllimport) void __stdcall RtlLeaveCriticalSection(RTL_CRITICAL_SECTION*);
	__declspec(dllimport) void __stdcall RtlDeleteCriticalSection(RTL_CRITICAL_SECTION*);

	__declspec(dllimport) NTSTATUS __stdcall NtOpenProcess(
		void** ProcessHandle,
		uint32_t DesiredAccess,
		OBJECT_ATTRIBUTES* ObjectAttributes,
		CLIENT_ID* ClientId
	);

	__declspec(dllimport) NTSTATUS __stdcall NtWaitForSingleObject(
		void* Handle,
		unsigned char Alertable,
		int64_t* Timeout
	);

	__declspec(dllimport) NTSTATUS __stdcall NtQueryInformationProcess(
		void* ProcessHandle,
		uint32_t ProcessInformationClass,
		void* ProcessInformation,
		uint32_t ProcessInformationLength,
		uint32_t* ReturnLength
	);
}
