#include <ex/ex.h>
#include <mt/mt.h>
#include <ob/ob.h>
#include <pal/pal.h>

INUIMPORT struct DOMAIN* ExGlobalZeroDomain;

INUGLOBAL struct THREAD MtGlobalBootstrapThread;
INUGLOBAL struct VECTOR MtGlobalThreadList;

VOID MtInitialize()
{
    RtlVectorInitialize(&MtGlobalThreadList);
    RtlVectorAdd(&MtGlobalThreadList,&MtGlobalBootstrapThread);

    ExThreadInitialize(&MtGlobalBootstrapThread,ExGlobalZeroDomain);
}

VOID MtThreadRegister(struct THREAD *thread)
{
    RtlVectorAdd(&MtGlobalThreadList,thread);
}

struct THREAD * MtThreadGetCurrent()
{
    UINTPTR id = PalThreadGetCurrentId();

    for (int i = 0; i < MtGlobalThreadList.count; ++i)
    {
        struct THREAD* thread = RtlVectorGet(&MtGlobalThreadList,i);
        if (thread->id == id)
        {
            return thread;
        }
    }

    return NULL;
}
