#pragma once
#include <intermarx.h>
#include <windows.h>

#define PalStackAllocate(length) __builtin_alloca(length)
#define PalStackBufferAllocate(elementCount,elementLength) __builtin_alloca(elementCount*elementLength)

typedef UINTPTR (*THREAD_LAUNCH)(void *);
typedef CRITICAL_SECTION MONITOR;

void PalDebugbreak(const CHAR* message);
UINTPTR PalThreadBootstrap(VOID* arg);

void* PalMemoryAllocate(UINTPTR count);
void PalMemoryFree(void* memory);

void PalMemoryCopy(void* dest, void* src, UINTPTR length);
void PalMemoryZero(void* dest, UINTPTR length);
void PalMemoryZeroBlock(void* dest, UINTPTR elementCount, UINTPTR elementLength);

UINTPTR PalStringLength(const CHAR* message);
UINTPTR PalWStringLength(const WCHAR* message);

NATIVE_HANDLE PalThreadGetCurrentHandle();
UINTPTR PalThreadGetCurrentId();
NATIVE_HANDLE PalThreadCreate(THREAD_LAUNCH delegate, void* argument);
void PalThreadExitCurrent(UINTPTR code);
void PalThreadResume(NATIVE_HANDLE thread);
void PalThreadSuspend(NATIVE_HANDLE thread);

void PalMonitorInitialize(MONITOR* monitor);
void PalMonitorEnter(MONITOR* monitor);
void PalMonitorExit(MONITOR* monitor);
