#include <ex/ex.h>
#include <pal/pal.h>
#include <rtl/rtl.h>

INUGLOBAL UINTPTR ExGlobalDomainId;

INUIMPORT struct TYPE *ExStringType;
INUIMPORT struct TYPE *ExCharArrayType;
INUIMPORT struct TYPE *ExCharType;
INUIMPORT struct TYPE *ExThreadType;


BOOLEAN ExMetadataIs(enum METADATA_CHARACTERISTICS thisValue, enum METADATA_CHARACTERISTICS metadata)
{
    if ((thisValue & metadata) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

VOID ExDomainInitialize(struct DOMAIN *thisPtr)
{
    thisPtr->id = ExGlobalDomainId++;
    RtlVectorInitialize(&thisPtr->threads);
    RtlVectorInitialize(&thisPtr->types);
}

VOID ExTypeInitialize(struct TYPE *thisPtr)
{
    thisPtr->descriptor = DESCRIPTOR_TYPE;
    thisPtr->super = NULL;
    thisPtr->inlined = BASE_OTHER;

    RtlVectorInitialize(&thisPtr->attributes);
    RtlVectorInitialize(&thisPtr->fields);
    RtlVectorInitialize(&thisPtr->interfaces);
    RtlVectorInitialize(&thisPtr->methods);
}

VOID ExFieldInitialize(struct FIELD *thisPtr)
{
    thisPtr->descriptor = DESCRIPTOR_FIELD;

    RtlVectorInitialize(&thisPtr->attributes);
}

VOID ExMethodInitialize(struct METHOD *thisPtr)
{
    thisPtr->descriptor = DESCRIPTOR_METHOD;

    PalMemoryZero(&thisPtr->farCall,sizeof(struct FAR_CALL));

    RtlVectorInitialize(&thisPtr->attributes);
    RtlVectorInitialize(&thisPtr->handlers);
    RtlVectorInitialize(&thisPtr->parameters);
    RtlVectorInitialize(&thisPtr->pool);
    RtlVectorInitialize(&thisPtr->variables);
    RtlVectorInitialize(&thisPtr->stringTable);
}

VOID ExThreadInitialize(struct THREAD *thisPtr, struct DOMAIN* domain)
{
    PalMemoryZero(thisPtr,sizeof(struct THREAD));

    thisPtr->handle = PalThreadGetCurrentHandle();
    thisPtr->id = PalThreadGetCurrentId();
    thisPtr->domain = domain;
    thisPtr->wrapper = NULL;
    ExExceptionStateInitialize(&thisPtr->state);
}

VOID ExExceptionStateInitialize(struct EXCEPTION_STATE *thisPtr)
{
    thisPtr->exception = NULL;
    thisPtr->isUnwinding = FALSE;
    RtlVectorInitialize(&thisPtr->stackTrace);
}

void ExExceptionStateAppend(struct EXCEPTION_STATE *thisPtr, struct METHOD *method)
{
    RtlVectorAdd(&thisPtr->stackTrace,&method->fullName);
}

void ExExceptionStateDrop(struct EXCEPTION_STATE *thisPtr)
{
    thisPtr->stackTrace.count = 0;
}

struct METHOD * ExTypeLocateMethod(struct TYPE *thisPtr, const CHAR *shortName)
{
    for (int i = 0; i < thisPtr->methods.count; ++i)
    {
        struct METHOD* method = RtlVectorGet(&thisPtr->methods,i);

        if (RtlNStringCompare(method->shortName,shortName))
        {
            return method;
        }
    }

    return NULL;
}

struct TYPE * ExDomainLocateType(struct DOMAIN *thisPtr, const CHAR *string)
{
    for (int i = 0; i < thisPtr->types.count; ++i)
    {
        struct TYPE* type = RtlVectorGet(&thisPtr->types,i);
        if (RtlNStringCompare(type->fullName,string))
        {
            return type;
        }
    }

    PalDebugbreak("Fail to find type!");
    return NULL;
}

