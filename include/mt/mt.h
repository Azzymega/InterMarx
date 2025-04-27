#pragma once
#include <intermarx.h>

VOID MtInitialize();
struct THREAD* MtThreadGetCurrent();
VOID MtThreadRegister(struct THREAD* thread);