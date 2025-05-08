#include <stdatomic.h>
#include <intermarx/pal/pal.h>
#include <intermarx/ex/runtime.h>
#include <intermarx/mt/mt.h>

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
    struct RUNTIME_FRAME_BLOCK ret;
    struct RUNTIME_FRAME_BLOCK delegate;

    delegate.type = MACHINE_OBJECT;
    delegate.descriptor = arg;

    if (MARX_SUCCESS(ExMethodPrologueDelegate(&delegate,NULL,NULL,&ret)))
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
    return calloc(1,count);
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
    return (NATIVE_HANDLE)CreateThread(0,0,(VOID*)delegate,argument,0,0);
}

VOID PalThreadResume(NATIVE_HANDLE thread)
{
    ResumeThread((HANDLE)thread);
}

VOID PalThreadSuspend(NATIVE_HANDLE thread)
{
    SuspendThread((HANDLE)thread);
}

UINTPTR PalThreadGetId(NATIVE_HANDLE thread)
{
    GetThreadId((HANDLE)thread);
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
        struct RUNTIME_THREAD* current = MtThreadGetCurrent();
        current->inSafePoint = TRUE;

        PalMonitorEnter(&PalGlobalMaintainenceBump);
        PalMonitorExit(&PalGlobalMaintainenceBump);

        current->inSafePoint = FALSE;
    }
}

void PalThreadExitCurrent(UINTPTR code)
{
    while (TRUE);
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
