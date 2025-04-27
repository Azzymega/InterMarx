#include <hp/hp.h>
#include <ob/ob.h>

VOID ObManagedAttributeInitialize(struct MANAGED_ATTRIBUTE *thisPtr)
{
    RtlVectorInitialize(&thisPtr->params);
    RtlVectorInitialize(&thisPtr->parametersList);
}

VOID ObManagedExceptionInitialize(struct MANAGED_EXCEPTION *thisPtr, struct TYPE *type, const WCHAR *message)
{
    thisPtr->nativeMessage = message;
    thisPtr->header.forward = NULL;
    thisPtr->header.type = type;
}

VOID ObManagedWrapperInitialize(struct MANAGED_WRAPPER *thisPtr, VOID *nativeHandle)
{
    thisPtr->handle = (NATIVE_HANDLE)nativeHandle;
}

VOID ObManagedDelegateInitialize(struct MANAGED_DELEGATE *thisPtr, VOID **object, VOID **callSite, INTPTR entryCount,
    struct MANAGED_DELEGATE *base)
{
    thisPtr->reserved = FALSE;
    thisPtr->callSites = HpAllocateManaged(sizeof(struct MANAGED_ARRAY));
    thisPtr->thisObjects = HpAllocateManaged(sizeof(struct MANAGED_ARRAY));

    thisPtr->callSites->elementType = base->callSites->elementType;
    thisPtr->callSites->header.type = base->callSites->header.type;

    thisPtr->thisObjects->elementType = base->thisObjects->elementType;
    thisPtr->thisObjects->header.type = base->thisObjects->header.type;

    thisPtr->callSites->pointer = HpAllocateNative(sizeof(VOID*)*entryCount);
    thisPtr->thisObjects->pointer = HpAllocateNative(sizeof(VOID*)*entryCount);

    for (int i = 0; i < entryCount; ++i)
    {
        thisPtr->callSites->pointer[i] = callSite[i];
        thisPtr->thisObjects->pointer[i] = object[i];
    }
}
