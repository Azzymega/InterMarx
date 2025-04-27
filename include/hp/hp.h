#pragma once
#include <intermarx.h>

struct FIELD;

VOID* HpAllocateNative(UINTPTR size);
VOID* HpAllocateManaged(UINTPTR size);
VOID HpFreeNative(VOID* pointer);
VOID HpRegisterRootField(struct FIELD* field);