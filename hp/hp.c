#include <stdio.h>
#include <hp/hp.h>
#include <pal/pal.h>
#include <mt/mt.h>
#include <ex/ex.h>
#include <pal/corelib.h>

#include "hpp.h"

#define HP_MANAGED_HEAP_BUMP_HEAP_TIME_TICKS 1000

INUGLOBAL struct VECTOR HpGlobalRootFieldList;
INUIMPORT struct VECTOR MtGlobalThreadList;

INUGLOBAL struct MANAGED_HEAP HpGlobalManagedHeap;
INUGLOBAL struct MANAGED_HEAP HpGlobalNextManagedHeap;

VOID HpInitialize()
{
    RtlVectorInitialize(&HpGlobalRootFieldList);
    HpManagedHeapInitialize(&HpGlobalManagedHeap);
    HpManagedHeapInitialize(&HpGlobalNextManagedHeap);
}

VOID * HpAllocateNative(UINTPTR size)
{
    return PalMemoryAllocate(size);
}

VOID * HpAllocateManaged(UINTPTR size)
{
    struct OBJECT_HEADER* header = HpManagedHeapAllocate(&HpGlobalManagedHeap,size);
    PalMemoryZero(header,size);
    header->color = GC_GREY;
    header->size = size;
    return header;
}

VOID HpFreeNative(VOID *pointer)
{
    PalMemoryFree(pointer);
}

VOID HpRegisterRootField(struct FIELD *field)
{
    RtlVectorAdd(&HpGlobalRootFieldList,field);
}

VOID HppWaitSafepoint()
{
    struct THREAD* ownThread = MtThreadGetCurrent();

    for (int i = 0; i < MtGlobalThreadList.count; ++i)
    {
        struct THREAD* thread = RtlVectorGet(&MtGlobalThreadList,i);

        while (!thread->inSafePoint && thread != ownThread)
        {

        }
    }

}

VOID* HppTransferObject(struct OBJECT_HEADER* header)
{
    if (header == NULL)
    {
        return NULL;
    }
    else if (header->forward != NULL)
    {
        return header->forward;
    }

    if (RtlNStringCompare(header->type->shortName,"StringBuilder"))
    {
        fflush(stdout);
    }

    UINTPTR size = header->size;
    VOID* newPtr = HpManagedHeapAllocate(&HpGlobalNextManagedHeap,size);
    header->forward = newPtr;
    header->color = GC_GREY;

    PalMemoryCopy(newPtr,header,size);

    struct OBJECT_HEADER* headerNew = newPtr;
    headerNew->forward = newPtr;
    headerNew->color = GC_GREY;

    struct MANAGED_WRAPPER* wrapper = newPtr;
    if (headerNew->managedWrapper == TRUE)
    {
        PalMemoryCopy(wrapper->fixUpAddress,&newPtr,sizeof(UINTPTR));
    }

    return newPtr;
}

VOID* HppTraceObject(struct OBJECT_HEADER* header)
{
    if (header == NULL)
    {
        return NULL;
    }

    if (header->forward != NULL)
    {
        header = header->forward;
    }

    if (header->color == GC_BLACK)
    {
        return header->forward;
    }
    else
    {
        struct TYPE* objectType = header->type;

        for (int i = 0; i < objectType->fields.count; ++i)
        {
            struct FIELD* field = RtlVectorGet(&objectType->fields,i);

            VOID* objectPtr = (char*)header + sizeof(struct OBJECT_HEADER) + field->offset;

            if (!ExMetadataIs(field->metadata,MxExMetadataStatic))
            {
                if (ExMetadataIs(field->declared->metadata,MxExMetadataPrimitive) || ExMetadataIs(field->declared->metadata,MxExMetadataEnum))
                {
                    continue;
                }
                else if (ExMetadataIs(field->declared->metadata,MxExMetadataStruct))
                {
                    HppTraceStruct(objectPtr,field->declared);
                }
                else
                {
                    VOID** fieldPtr = objectPtr;
                    VOID* relocatedAddress = HppTransferObject(*fieldPtr);
                    *fieldPtr = relocatedAddress;
                }
            }
        }

        if (header->forward == NULL)
        {
            struct OBJECT_HEADER* relocPtr = HppTransferObject(header);
            relocPtr->color = GC_BLACK;
            header->color = GC_BLACK;
            return relocPtr;
        }
        else
        {
            header->color = GC_BLACK;
            return header->forward;
        }
    }
}

VOID HppTraceField(struct FIELD* field)
{
    if (ExMetadataIs(field->declared->metadata,MxExMetadataEnum) || ExMetadataIs(field->declared->metadata,MxExMetadataPrimitive))
    {
        return;
    }
    else if (ExMetadataIs(field->declared->metadata,MxExMetadataStruct))
    {
        HppTraceStruct(&field->staticValue,field->declared);
    }
    else
    {
        VOID** fieldOffset = &field->staticValue;
        *fieldOffset = HppTransferObject(*fieldOffset);
    }
}

VOID HppTraceStruct(VOID* structure, struct TYPE* type)
{
    for (int i = 0; i < type->fields.count; ++i)
    {
        struct FIELD* field = RtlVectorGet(&type->fields,i);

        if (ExMetadataIs(field->declared->metadata,MxExMetadataEnum) || ExMetadataIs(field->declared->metadata,MxExMetadataPrimitive))
        {
            continue;
        }
        else if (ExMetadataIs(field->declared->metadata,MxExMetadataStruct))
        {
            HppTraceStruct(&structure[field->offset],field->declared);
        }
        else
        {
            VOID* fieldPtr = (char*)structure + field->offset;
            VOID** fieldOffset = (VOID**)fieldPtr;
        }
    }
}

VOID HppScanFrame(struct FRAME* frame)
{
    for (int i = 0; i < frame->sp; ++i)
    {
        struct FRAME_BLOCK* block = &frame->stack[i];

        if (block->type == MACHINE_OBJECT)
        {
            block->descriptor = HppTransferObject(block->descriptor);
        }
        else if (block->type == MACHINE_STRUCT)
        {
            HppTraceStruct(block->valueType.pointer,block->valueType.type);
        }
        else if (block->type == MACHINE_MANAGED_POINTER)
        {
            HppTraceStruct(block->link.pointer,block->link.type);
        }
    }

    for (int i = 0; i < frame->method->variables.count; ++i)
    {
        struct FRAME_BLOCK* block = &frame->variables[i];

        if (block->type == MACHINE_OBJECT)
        {
            block->descriptor = HppTransferObject(block->descriptor);
        }
        else if (block->type == MACHINE_STRUCT)
        {
            HppTraceStruct(block->valueType.pointer, block->valueType.type);
        }
        else if (block->type == MACHINE_MANAGED_POINTER)
        {
            HppTraceStruct(block->link.pointer, block->link.type);
        }
    }

    for (int i = 0; i < frame->method->parameters.count; ++i)
    {
        struct FRAME_BLOCK* block = &frame->args[i];

        if (block->type == MACHINE_OBJECT)
        {
            block->descriptor = HppTransferObject(block->descriptor);
        }
        else if (block->type == MACHINE_STRUCT)
        {
            HppTraceStruct(block->valueType.pointer, block->valueType.type);
        }
        else if (block->type == MACHINE_MANAGED_POINTER)
        {
            HppTraceStruct(block->link.pointer, block->link.type);
        }
    }
}

struct MANAGED_HEAP* HpGc()
{
    PalEnterMaintainenceMode();
    HppWaitSafepoint();


    for (int i = 0; i < HpGlobalRootFieldList.count; ++i)
    {
        struct FIELD* field = RtlVectorGet(&HpGlobalRootFieldList,i);
        HppTraceField(field);
    }


    for (int i = 0; i < MtGlobalThreadList.count; ++i)
    {
        struct THREAD* thread = RtlVectorGet(&MtGlobalThreadList,i);
        struct FRAME* frame = thread->firstFrame;

        while (frame != NULL)
        {
            HppScanFrame(frame);
            frame = frame->next;
        }
    }


    VOID* ptr = HpGlobalManagedHeap.start;
    for (int i = 0; i < HpGlobalManagedHeap.objectCount; ++i)
    {
        struct OBJECT_HEADER* header = ptr;

        if (header->interop == TRUE)
        {
            HppTransferObject(header);
        }

        ptr += header->size;
    }

    GcInspect:

    BOOLEAN tryAgain = FALSE;
    VOID* newPtr = HpGlobalNextManagedHeap.start;

    for (int i = 0; i < HpGlobalNextManagedHeap.objectCount; ++i)
    {
        struct OBJECT_HEADER* header = newPtr;

        if (header->color == GC_GREY)
        {
            HppTraceObject(header);
            tryAgain = TRUE;
        }

        newPtr += header->size;
    }

    if (tryAgain)
    {
        goto GcInspect;
    }

    newPtr = HpGlobalNextManagedHeap.start;
    for (int i = 0; i < HpGlobalNextManagedHeap.objectCount; ++i)
    {
        struct OBJECT_HEADER* header = newPtr;

        header->color = GC_WHITE;
        header->forward = NULL;

        newPtr += header->size;
    }

    struct MANAGED_HEAP oldHeap = HpGlobalManagedHeap;
    oldHeap.current = oldHeap.start;
    oldHeap.objectCount = 0;

    HpGlobalManagedHeap = HpGlobalNextManagedHeap;
    HpGlobalNextManagedHeap = oldHeap;

    PalMemoryZero(oldHeap.start,oldHeap.size);

    PalExitMaintainenceMode();

    return &HpGlobalManagedHeap;
}

VOID HpManagedHeapInitialize(struct MANAGED_HEAP *thisPtr)
{
    thisPtr->current = PalMemoryAllocate(1024*1024*2);
    thisPtr->start = thisPtr->current;
    thisPtr->size = 1024*1024*2;
    PalMonitorInitialize(&thisPtr->lock);
}

VOID* HpManagedHeapAllocate(struct MANAGED_HEAP *thisPtr, UINTPTR size)
{
    if (thisPtr->current + size >= thisPtr->start + thisPtr->size)
    {
        if (PalTryEnterLock(&thisPtr->isGcInProgress))
        {
            struct MANAGED_HEAP* oldHeap = thisPtr;
            struct MANAGED_HEAP* newHeap = HpGc();

            printf("%s","\r\nGC IN PROGRESS!!!\r\n");

            PalExitLock(&oldHeap->isGcInProgress);
            PalExitLock(&newHeap->isGcInProgress);
        }
        else
        {
            PalSafepoint();
        }
    }

    PalMonitorEnter(&thisPtr->lock);

    VOID* space = thisPtr->current;
    thisPtr->current += size;
    thisPtr->objectCount++;

    PalMonitorExit(&thisPtr->lock);
    return space;
}
