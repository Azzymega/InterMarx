#include <stdio.h>
#include <stdlib.h>
#include <ex/runtime.h>
#include <pal/pal.h>
#include <ldr/ldr.h>
#include <mt/mt.h>

INUGLOBAL struct DOMAIN* ExGlobalZeroDomain;
INUGLOBAL MONITOR ExGlobalStackTraceMonitor;

VOID MxDumpStackTrace(struct EXCEPTION_STATE* state)
{
    PalMonitorEnter(&ExGlobalStackTraceMonitor);

    struct MANAGED_EXCEPTION* exception = state->exception;

    printf("%s","\r\n");
    printf("Exception in thread %i! ",PalThreadGetCurrentId());
    wprintf(L"%ls",exception->header.type->fullName.characters);
    printf("%s"," - ");

    if (exception->nativeMessage != NULL)
    {
        wprintf(L"%ls",exception->nativeMessage);
    }
    else
    {
        for (int i = 0; i < exception->message->characters->count; ++i)
        {
            putchar(exception->message->characters->characters[i]);
        }
    }

    printf("%s","\r\n");
    printf("%s","Stack trace: \r\n");

    for (int i = 0; i < state->stackTrace.count; ++i)
    {
        struct NSTRING* str = RtlVectorGet(&state->stackTrace,i);
        wprintf(L"%ls\r\n",str->characters);
    }

    printf("%s","\r\n");
    fflush(stdout);

    PalMonitorExit(&ExGlobalStackTraceMonitor);
    PalThreadExitCurrent(-1);
}

VOID MxStart(VOID *buffer)
{
    struct IMAGE_LOADER loader = {
        .image = buffer,
        .offset = 0
    };

    PalMonitorInitialize(&ExGlobalStackTraceMonitor);

    struct DOMAIN *domain = LdrLoadDomain(&loader);
    ExGlobalZeroDomain = domain;

    ExInitialize();
    MtInitialize();

    LdrExecuteStaticConstructors(domain);
    LdrExecuteAttributesConstructors(domain);

    FarInitialize(domain);

    struct TYPE *getType = ExDomainLocateType(domain, "Internationale.Runtime.Initializer");
    struct METHOD *method = ExTypeLocateMethod(getType, "Initialize");

    if (MARX_SUCCESS(ExMethodPrologue(method)))
    {
        return;
    }
    else
    {
        MxDumpStackTrace(&MtThreadGetCurrent()->state);
    }
}

int main(void)
{
    FILE *f = fopen("H:\\Projects\\CC++\\MarxVm\\System.vkp", "rb");
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    BYTE *buffer = malloc(fsize);
    fread(buffer, fsize, 1, f);

    MxStart(buffer);

    free(buffer);

    return 0;
}
