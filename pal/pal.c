#include <stdatomic.h>
#include <stdio.h>
#include <pal/pal.h>
#include <windows.h>
#include <ex/runtime.h>
#include <mt/mt.h>

INUGLOBAL BOOLEAN PalGlobalMaintainence;
INUGLOBAL MONITOR PalGlobalMaintainenceBump;

extern VOID MxDumpStackTrace(struct EXCEPTION_STATE* state);

VOID PalEnterLock(INUVOLATILE BOOLEAN* monitor)
{
    while (atomic_flag_test_and_set_explicit((struct atomic_flag*)monitor, memory_order_acquire))
    {
        asm("pause");
    }
}

VOID PalExitLock(INUVOLATILE BOOLEAN* monitor)
{
    atomic_flag_clear_explicit((struct atomic_flag*)monitor, memory_order_release);
}

BOOLEAN PalTryEnterLock(INUVOLATILE BOOLEAN* monitor)
{
    return !atomic_flag_test_and_set_explicit((struct atomic_flag*)monitor, memory_order_acquire);
}

void PalInitialize()
{
    PalMonitorInitialize(&PalGlobalMaintainenceBump);
    PalGlobalMaintainence = FALSE;
}

void PalDebugbreak(const CHAR *message)
{
    DebugBreak();
}

UINTPTR PalThreadBootstrap(void *arg)
{
    struct FRAME_BLOCK ret;
    if (MARX_SUCCESS(ExMethodPrologueDelegate(arg,NULL,NULL,&ret)))
    {
        return ret.int32;
    }
    else
    {
        MxDumpStackTrace(&MtThreadGetCurrent()->state);
    }

    return ret.int32;
}

VOID * PalMemoryAllocate(UINTPTR count)
{
    return malloc(count);
}

VOID PalMemoryFree(VOID *memory)
{
    free(memory);
}

void PalMemoryCopy(void *dest, void *src, UINTPTR length)
{
    memcpy(dest,src,length);
}

void PalMemoryZero(void *dest, UINTPTR length)
{
    memset(dest,0,length);
}

void PalMemoryZeroBlock(void *dest, UINTPTR elementCount, UINTPTR elementLength)
{
    PalMemoryZero(dest,elementCount*elementLength);
}

UINTPTR PalStringLength(const CHAR *message)
{
    return strlen(message);
}

UINTPTR PalWStringLength(const WCHAR *message)
{
    const WCHAR *start = message;

    while (*message != L'\0') {
        message++;
    }

    return message - start;
}

NATIVE_HANDLE PalThreadGetCurrentHandle()
{
    return (NATIVE_HANDLE)GetCurrentThread();
}

UINTPTR PalThreadGetCurrentId()
{
    return GetCurrentThreadId();
}

NATIVE_HANDLE PalThreadCreate(THREAD_LAUNCH delegate, VOID *argument)
{
    return (NATIVE_HANDLE)CreateThread(NULL,0,(VOID*)delegate,argument,CREATE_SUSPENDED,NULL);
}

VOID PalThreadResume(NATIVE_HANDLE thread)
{
    ResumeThread((HANDLE)thread);
}

VOID PalThreadSuspend(NATIVE_HANDLE thread)
{
    SuspendThread((HANDLE)thread);
}

void PalEnterMaintainenceMode()
{
    PalMonitorEnter(&PalGlobalMaintainenceBump);

    PalGlobalMaintainence = TRUE;
}

void PalExitMaintainenceMode()
{
    PalMonitorExit(&PalGlobalMaintainenceBump);

    PalGlobalMaintainence = FALSE;
}

void PalSafepoint()
{
    if (PalGlobalMaintainence)
    {
        struct THREAD* current = MtThreadGetCurrent();
        current->inSafePoint = TRUE;

        PalMonitorEnter(&PalGlobalMaintainenceBump);
        PalMonitorExit(&PalGlobalMaintainenceBump);

        current->inSafePoint = FALSE;
    }
}

void PalThreadExitCurrent(UINTPTR code)
{
    ExitThread(code);
}

void PalMonitorInitialize(MONITOR *monitor)
{
    InitializeCriticalSection(monitor);
}

void PalMonitorEnter(MONITOR *monitor)
{
    EnterCriticalSection(monitor);
}

void PalMonitorExit(MONITOR *monitor)
{
    LeaveCriticalSection(monitor);
}
