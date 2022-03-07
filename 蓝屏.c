#include <windows.h>
#include <STDBOOL.h>
BOOL SetPrivilege(LPCSTR lpPrivilegeName, BOOL fEnable)
{
    HANDLE hToken; 
    TOKEN_PRIVILEGES NewState; 
    LUID luidPrivilegeLUID; 

    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    { 
        /*如果打开令牌失败…*/
        return FALSE; 
    } 

    if(fEnable == FALSE) /*我们禁用所有特权…*/
    {
        if(!AdjustTokenPrivileges(hToken, TRUE, NULL, 0, NULL, NULL))
    {
        return FALSE;
    }
        else return TRUE;
    }
    /*查找权限的LUID值…*/
    LookupPrivilegeValue(NULL, lpPrivilegeName, &luidPrivilegeLUID);

    NewState.PrivilegeCount = 1; 
    NewState.Privileges[0].Luid = luidPrivilegeLUID; 
    NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
    /*改进这个进程的特权，这样我们接下来就可以关闭系统了。*/
    if(!AdjustTokenPrivileges(hToken, FALSE, &NewState, 0, NULL, NULL))
    {
        return FALSE;
    }

    /*我们不仅要检查改进是否成功……  */
    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    {
        return FALSE;
    }
        return TRUE;
}

typedef enum _HARDERROR_RESPONSE_OPTION {
    OptionAbortRetryIgnore,
    OptionOk,
    OptionOkCancel,
    OptionRetryCancel,
    OptionYesNo,
    OptionYesNoCancel,
    OptionShutdownSystem,
    OptionOkNoWait,
    OptionCancelTryContinue
} HARDERROR_RESPONSE_OPTION;

typedef LONG (WINAPI *type_ZwRaiseHardError)(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, HARDERROR_RESPONSE_OPTION ValidResponseOptions, PULONG Response);

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWCH Buffer;
} UNICODE_STRING;

int main(void)
{
    FreeConsole();
    UNICODE_STRING str = {8, 10, L"test"};
    unsigned long long args[] = {0x12345678, 0x87654321, (unsigned long long)&str};
    unsigned long x;
    HMODULE hDll = GetModuleHandle(TEXT("ntdll.dll"));
    type_ZwRaiseHardError ZwRaiseHardError = (type_ZwRaiseHardError)GetProcAddress(hDll, "ZwRaiseHardError");

    bool bSuccess = SetPrivilege(SE_SHUTDOWN_NAME, TRUE);
    if(bSuccess)
    {
        ZwRaiseHardError(0xC000021A, 3, 4, args, OptionShutdownSystem, &x);
    }
    SetPrivilege(NULL, FALSE);
    
}

