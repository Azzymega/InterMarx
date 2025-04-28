#pragma once
#include <intermarx.h>
#include <rtl/rtl.h>
#include <far/far.h>
#include <hp/hp.h>

struct FIELD;

struct OBJECT_HEADER
{
    UINTPTR size;
    struct TYPE* type;

    UINTPTR interop : 1;
    UINTPTR managedWrapper : 1;
    UINTPTR reserved : 30;

    enum GC_COLOR color;
    VOID* forward;
};

struct INUMANAGED INUPACKED MANAGED_ARRAY
{
    struct OBJECT_HEADER header;

    struct TYPE* elementType;
    UINTPTR count;

    union
    {
        VOID **pointer;
        INTPTR *nint;
        INT32 *int32;
        INT64 *int64;
        INT16 *int16;
        SINGLE *single;
        DOUBLE *duoble;
        BYTE *byte;
        WCHAR *characters;
    };
};

struct INUMANAGED INUPACKED MANAGED_WRAPPER
{
    struct OBJECT_HEADER header;

    NATIVE_HANDLE handle;
    VOID* fixUpAddress;
};

struct INUMANAGED INUPACKED MANAGED_STRING
{
    struct OBJECT_HEADER header;

    struct MANAGED_ARRAY* characters;
};

struct INUMANAGED INUPACKED MANAGED_EXCEPTION
{
    struct OBJECT_HEADER header;

    struct MANAGED_STRING* message;

    WCHAR* nativeMessage;
};

struct INUMANAGED INUPACKED MANAGED_DELEGATE
{
    struct OBJECT_HEADER header;

    struct MANAGED_ARRAY* thisObjects;
    struct MANAGED_ARRAY* callSites;
    INT32 reserved;
};


struct INUMANAGED INUPACKED MANAGED_ATTRIBUTE
{
    struct OBJECT_HEADER header;

    struct METHOD *ctor;

    union
    {
        enum EXECUTIVE_OWNER_DESCRIPTOR *owner;
        struct TYPE* typeOwner;
        struct METHOD* methodOwner;
        struct FIELD* fieldOwner;
    };

    struct VECTOR parametersList;
    struct VECTOR params;
};

struct INUMANAGED INUPACKED FAR_CALL_MANAGED_ATTRIBUTE
{
    struct MANAGED_ATTRIBUTE base;

    struct MANAGED_STRING* functionName;
    enum FAR_CALL_CALL_CONVENTION convention;
    enum FAR_CALL_STRING_ENCODING encoding;
    enum FAR_CALL_SYMBOL_SOURCE source;
};

VOID* ObManagedArrayInitialize(UINTPTR elementCount, UINTPTR elementSize);
VOID ObManagedAttributeInitialize(struct MANAGED_ATTRIBUTE* thisPtr);
VOID ObManagedExceptionInitialize(struct MANAGED_EXCEPTION* thisPtr, struct TYPE* type, const WCHAR* message);
VOID ObManagedWrapperInitialize(struct MANAGED_WRAPPER* thisPtr, VOID* nativeHandle);
VOID ObManagedDelegateInitialize(struct MANAGED_DELEGATE* thisPtr, VOID** object, VOID** callSite, INTPTR entryCount, struct MANAGED_DELEGATE* base);