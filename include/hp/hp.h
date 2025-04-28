#pragma once
#include <intermarx.h>
#include <pal/pal.h>

struct FIELD;

enum GC_COLOR
{
    GC_WHITE,
    GC_GREY,
    GC_BLACK,
};

struct MANAGED_HEAP
{
    VOID *start;
    VOID *current;

    UINTPTR size;
    UINTPTR objectCount;
    MONITOR lock;

    INUVOLATILE BOOLEAN isGcInProgress;
};


VOID HpInitialize();
VOID HpFreeNative(VOID *pointer);
VOID *HpAllocateNative(UINTPTR size);
VOID *HpAllocateManaged(UINTPTR size);


VOID HpRegisterRootField(struct FIELD *field);
struct MANAGED_HEAP* HpGc();


VOID HpManagedHeapInitialize(struct MANAGED_HEAP *thisPtr);
VOID* HpManagedHeapAllocate(struct MANAGED_HEAP *thisPtr, UINTPTR size);
