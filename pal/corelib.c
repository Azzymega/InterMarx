#pragma once
#include <stdio.h>
#include <time.h>
#include <ex/ex.h>
#include <hp/hp.h>
#include <mt/mt.h>
#include <pal/corelib.h>
#include <pal/pal.h>


VOID PalManagedThreadCreate(struct MANAGED_WRAPPER *thread, struct MANAGED_DELEGATE *delegate)
{
    struct THREAD* target = HpAllocateNative(sizeof(struct THREAD));
    ExThreadInitialize(target,thread->header.type->domain);
    MtThreadRegister(target);

    target->wrapper = thread;
    thread->header.interop = TRUE;
    thread->header.managedWrapper = TRUE;
    thread->fixUpAddress = &target->wrapper;
    thread->handle = (NATIVE_HANDLE)target;

    target->delegate = *delegate;
    target->handle = PalThreadCreate(PalThreadBootstrap,&target->delegate);
    target->id = GetThreadId((HANDLE)target->handle);

    for (int i = 0; i < target->delegate.thisObjects->count; ++i)
    {
        struct OBJECT_HEADER* header = target->delegate.thisObjects->pointer[i];

        if (header != NULL)
        {
            header->interop = TRUE;
        }
    }
}

INT32 PalManagedThreadStart(struct MANAGED_WRAPPER *thread)
{
    const struct THREAD* target = (struct THREAD*)thread->handle;

    if (target->delegate.thisObjects == NULL || target->delegate.callSites == NULL)
    {
        return FALSE;
    }

    if (target->delegate.callSites->count == 0)
    {
        return FALSE;
    }

    PalThreadResume(target->handle);

    return TRUE;
}

INT32 PalManagedThreadId()
{
    return MtThreadGetCurrent()->id;
}

VOID* PalManagedThreadGetCurrent()
{
    return MtThreadGetCurrent()->wrapper;
}

INT64 PalTimerClock()
{
    return clock();
}

VOID PalLoggerPrint(const CHAR *message)
{
    printf("%s",message);
    fflush(stdout);
}

struct MANAGED_DELEGATE * PalManagedDelegateRemoveImplNative(struct MANAGED_DELEGATE *thisPtr,
    struct MANAGED_DELEGATE *delegate)
{
    if (delegate == NULL)
    {
        return thisPtr;
    }
    else
    {
        UINTPTR thisCount = thisPtr->callSites->count;
        UINTPTR presentCount = 0;
        UINTPTR counter = 0;

        struct MANAGED_DELEGATE* newDelegate = HpAllocateManaged(sizeof(struct MANAGED_DELEGATE));
        newDelegate->header.type = thisPtr->header.type;

        VOID* objects[thisCount];
        VOID* callSites[thisCount];

        for (int i = 0; i < thisPtr->callSites->count; ++i)
        {
            objects[i] = thisPtr->thisObjects->pointer[i];
            callSites[i] = thisPtr->callSites->pointer[i];
        }

        for (int i = 0; i < delegate->callSites->count; ++i)
        {
            for (int z = 0; z < thisCount; ++z)
            {
                if (callSites[z] == delegate->callSites->pointer[i])
                {
                    callSites[z] = NULL;
                }
            }
        }

        for (int i = 0; i < thisCount; ++i)
        {
            if (callSites[i] != NULL)
            {
                presentCount++;
            }
        }

        VOID* finalObjects[presentCount];
        VOID* finalCallSites[presentCount];

        for (int i = 0; i < thisCount; ++i)
        {
            if (callSites[i] != NULL)
            {
                finalObjects[counter] = objects[i];
                finalCallSites[counter] = callSites[i];
                counter++;
            }
        }

        ObManagedDelegateInitialize(newDelegate, finalObjects, finalCallSites, presentCount, thisPtr);

        return newDelegate;
    }
}

struct MANAGED_DELEGATE * PalManagedDelegateCombineImplNative(struct MANAGED_DELEGATE *thisPtr,
                                                              struct MANAGED_DELEGATE *delegate)
{
    if (delegate == NULL)
    {
        return thisPtr;
    }
    else
    {
        UINTPTR maxCount = thisPtr->callSites->count + delegate->callSites->count;
        UINTPTR counter = 0;

        struct MANAGED_DELEGATE* newDelegate = HpAllocateManaged(sizeof(struct MANAGED_DELEGATE));
        newDelegate->header.type = thisPtr->header.type;

        VOID* objects[maxCount];
        VOID* callSites[maxCount];

        for (int i = 0; i < thisPtr->callSites->count; ++i)
        {
            objects[counter] = thisPtr->thisObjects->pointer[i];
            callSites[counter] = thisPtr->callSites->pointer[i];

            counter++;
        }

        for (int i = 0; i < delegate->callSites->count; ++i)
        {
            objects[counter] = delegate->thisObjects->pointer[i];
            callSites[counter] = delegate->callSites->pointer[i];

            counter++;
        }

        ObManagedDelegateInitialize(newDelegate, objects, callSites, counter, thisPtr);

        return newDelegate;
    }
}

