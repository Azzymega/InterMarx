#include <ex/ex.h>
#include <ex/runtime.h>
#include <pal/pal.h>
#include <math.h>

struct FRAME_BLOCK ExPeek(struct FRAME *frame)
{
    return frame->stack[frame->sp - 1];
}

struct FRAME_BLOCK ExPop(struct FRAME *frame)
{
    return frame->stack[--frame->sp];
}

VOID ExPush(struct FRAME *frame, struct FRAME_BLOCK block)
{
    frame->stack[frame->sp++] = block;
}

VOID* ExGetPoolElement(struct READER *reader, struct FRAME *frame)
{
    UINT32 index = RtlReaderReadInt32(reader);
    return RtlVectorGet(&frame->method->pool,index);
}

MARX_STATUS ExIsInstance(struct TYPE *castTarget, struct TYPE *objectType)
{
    BOOLEAN casted = FALSE;
    if (ExMetadataIs(castTarget->metadata, MxExMetadataInterface))
    {
        for (UINTPTR i = 0; i < castTarget->interfaces.count; ++i)
        {
            if (RtlVectorGet(&castTarget->interfaces, i) == objectType)
            {
                casted = TRUE;
            }
        }
    }
    if (ExMetadataIs(castTarget->metadata, MxExMetadataEnum))
    {
        struct FIELD *firstField = RtlVectorGet(&castTarget->fields, 0);
        if (firstField->declared == objectType)
        {
            casted = TRUE;
        }
    }
    else
    {
        while (objectType != NULL)
        {
            if (objectType == castTarget)
            {
                casted = TRUE;
                return STATUS_SUCCESS;
            }
            objectType = objectType->super;
        }
    }

    return STATUS_FAIL;
}

BOOLEAN ExIsZero(struct FRAME_BLOCK first)
{
    switch (first.type)
    {
        case MACHINE_OBJECT:
            return first.descriptor == 0;
            break;
        case MACHINE_MANAGED_POINTER:
            return first.link.pointer == 0;
            break;
        case MACHINE_INT32:
            return first.int32 == 0;
            break;
        case MACHINE_INT64:
            return first.int64 == 0;
            break;
        case MACHINE_MFLOAT:
            return first.floating == 0;
            break;
        case MACHINE_INTPTR:
            return first.pointer == 0;
            break;
        default:
        {
            break;
        }
    }
    return 0;
}

BOOLEAN ExIsNonZero(struct FRAME_BLOCK first)
{
    return !ExIsZero(first);
}

BOOLEAN ExIsEquals(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    return first.int32 == second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    return first.int32 == second.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT64:
                {
                    return first.int64 == second.int64;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_MFLOAT:
        {
            switch (second.type)
            {
                case MACHINE_MFLOAT:
                {
                    return first.floating == second.floating;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    return first.pointer == second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    return first.pointer == second.pointer;
                    break;
                }
                case MACHINE_MANAGED_POINTER:
                {
                    return first.pointer == (INTPTR) second.link.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_MANAGED_POINTER:
        {
            switch (second.type)
            {
                case MACHINE_INTPTR:
                {
                    return first.link.pointer == (void *) second.pointer;
                    break;
                }
                case MACHINE_MANAGED_POINTER:
                {
                    return first.link.pointer == second.link.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_OBJECT:
        {
            switch (second.type)
            {
                case MACHINE_OBJECT:
                {
                    return first.descriptor == second.descriptor;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        default:
        {
        }
    }

    return FALSE;
}

BOOLEAN ExIsUnEquals(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    return !ExIsEquals(first, second);
}

BOOLEAN ExIsGreaterEquals(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    return first.int32 >= second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    return first.int32 >= second.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT64:
                {
                    return first.int64 >= second.int64;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_MFLOAT:
        {
            switch (second.type)
            {
                case MACHINE_MFLOAT:
                {
                    return first.floating >= second.floating;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    return first.pointer >= second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    return first.pointer >= second.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_MANAGED_POINTER:
        {
            switch (second.type)
            {
                case MACHINE_MANAGED_POINTER:
                {
                    return first.link.pointer >= second.link.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        default:
        {
        }
    }


    return FALSE;
}

BOOLEAN ExIsGreater(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    return first.int32 > second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    return first.int32 > second.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT64:
                {
                    return first.int64 > second.int64;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_MFLOAT:
        {
            switch (second.type)
            {
                case MACHINE_MFLOAT:
                {
                    return first.floating > second.floating;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    return first.pointer > second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    return first.pointer > second.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_MANAGED_POINTER:
        {
            switch (second.type)
            {
                case MACHINE_MANAGED_POINTER:
                {
                    return first.link.pointer > second.link.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_OBJECT:
        {
            switch (second.type)
            {
                case MACHINE_OBJECT:
                {
                    return first.descriptor > second.descriptor;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        default:
        {
        }
    }


    return FALSE;
}

BOOLEAN ExIsLowerEquals(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    return first.int32 <= second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    return first.int32 <= second.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT64:
                {
                    return first.int64 <= second.int64;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_MFLOAT:
        {
            switch (second.type)
            {
                case MACHINE_MFLOAT:
                {
                    return first.floating <= second.floating;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    return first.pointer <= second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    return first.pointer <= second.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_MANAGED_POINTER:
        {
            switch (second.type)
            {
                case MACHINE_MANAGED_POINTER:
                {
                    return first.link.pointer <= second.link.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        default:
        {
        }
    }


    return FALSE;
}

BOOLEAN ExIsLower(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    return first.int32 < second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    return first.int32 < second.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT64:
                {
                    return first.int64 < second.int64;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_MFLOAT:
        {
            switch (second.type)
            {
                case MACHINE_MFLOAT:
                {
                    return first.floating < second.floating;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    return first.pointer < second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    return first.pointer < second.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case MACHINE_MANAGED_POINTER:
        {
            switch (second.type)
            {
                case MACHINE_MANAGED_POINTER:
                {
                    return first.link.pointer < second.link.pointer;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        default:
        {
        }
    }
    return FALSE;
}

struct FRAME_BLOCK ExConvMFloat(struct FRAME_BLOCK first)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            first.type = MACHINE_MFLOAT;
            first.floating = first.int32;
            break;
        }
        case MACHINE_INT64:
        {
            first.type = MACHINE_MFLOAT;
            first.floating = first.int64;
            break;
        }
        case MACHINE_INTPTR:
        {
            first.type = MACHINE_MFLOAT;
            first.floating = first.pointer;
            break;
        }
        default:
            PalDebugbreak("Bad argument!");
    }

    return first;
}

struct FRAME_BLOCK ExConvI64(struct FRAME_BLOCK first)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            first.type = MACHINE_INT64;
            first.int64 = first.int32;
            break;
        }
        case MACHINE_MFLOAT:
        {
            first.type = MACHINE_INT64;
            first.int64 = (INT64) first.floating;
            break;
        }
        case MACHINE_INTPTR:
        {
            first.type = MACHINE_INT64;
            first.int64 = (INT64) first.pointer;
            break;
        }
        default:
            PalDebugbreak("Bad argument!");
    }

    return first;
}

struct FRAME_BLOCK ExConvI32(struct FRAME_BLOCK first)
{
    switch (first.type)
    {
        case MACHINE_INT64:
        {
            first.type = MACHINE_INT32;
            first.int32 = (INT32) first.int64;
            break;
        }
        case MACHINE_INT32:
        {
            break;
        }
        case MACHINE_MFLOAT:
        {
            first.type = MACHINE_INT32;
            first.int32 = (INT32) first.floating;
            break;
        }
        case MACHINE_INTPTR:
        {
            first.type = MACHINE_INT32;
            first.int32 = first.pointer;
            break;
        }
        default:
            PalDebugbreak("Bad argument!");
    }

    return first;
}

struct FRAME_BLOCK ExConvI16(struct FRAME_BLOCK first)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            first.type = MACHINE_INT32;
            first.int32 = (INT16) first.int32;
            break;
        }
        case MACHINE_INT64:
        {
            first.type = MACHINE_INT32;
            first.int32 = (INT16) first.int64;
            break;
        }
        case MACHINE_MFLOAT:
        {
            first.type = MACHINE_INT32;
            first.int32 = (INT16) first.floating;
            break;
        }
        case MACHINE_INTPTR:
        {
            first.type = MACHINE_INT32;
            first.int32 = (INT16) first.pointer;
            break;
        }
        default:
            PalDebugbreak("Bad argument!");
    }

    return first;
}

struct FRAME_BLOCK ExConvIntPtr(struct FRAME_BLOCK first)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            first.type = MACHINE_INTPTR;
            first.pointer = first.int32;
            break;
        }
        case MACHINE_INT64:
        {
            first.type = MACHINE_INTPTR;
            first.pointer = first.int64;
            break;
        }
        case MACHINE_MFLOAT:
        {
            first.type = MACHINE_INTPTR;
            first.pointer = first.floating;
            break;
        }
        case MACHINE_INTPTR:
        {
            break;
        }
        default:
            PalDebugbreak("Bad argument!");
    }

    return first;
}

struct FRAME_BLOCK ExConvI8(struct FRAME_BLOCK first)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            first.type = MACHINE_INT32;
            first.int32 = (BYTE) first.int32;
            break;
        }
        case MACHINE_INT64:
        {
            first.type = MACHINE_INT32;
            first.int32 = (BYTE) first.int64;
            break;
        }
        case MACHINE_MFLOAT:
        {
            first.type = MACHINE_INT32;
            first.int32 = (BYTE) first.floating;
            break;
        }
        case MACHINE_INTPTR:
        {
            first.type = MACHINE_INT32;
            first.int32 = (BYTE) first.pointer;
            break;
        }
        default:
            PalDebugbreak("Bad argument!");
    }

    return first;
}

struct FRAME_BLOCK ExNot(struct FRAME_BLOCK first)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            first.int32 = ~first.int32;
            break;
        }
        case MACHINE_INT64:
        {
            first.int64 = ~first.int64;
            break;
        }
        case MACHINE_INTPTR:
        {
            first.pointer = -first.pointer;
            break;
        }
        default:
            PalDebugbreak("Bad argument!");
    }

    return first;
}

struct FRAME_BLOCK ExXor(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            first.int32 ^= second.int32;
            break;
        }
        case MACHINE_INT64:
        {
            first.int64 ^= second.int64;
            break;
        }
        case MACHINE_INTPTR:
        {
            first.pointer ^= second.pointer;
            break;
        }
        default:
            PalDebugbreak("Bad argument!");
    }

    return first;
}

struct FRAME_BLOCK ExOr(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            first.int32 |= second.int32;
            break;
        }
        case MACHINE_INT64:
        {
            first.int64 |= second.int64;
            break;
        }
        case MACHINE_INTPTR:
        {
            first.pointer |= second.pointer;
            break;
        }
        default:
            PalDebugbreak("Bad argument!");
    }

    return first;
}

struct FRAME_BLOCK ExAnd(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    struct FRAME_BLOCK result = {0};

    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INT32;
                    result.int32 = first.int32 && second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INT32;
                    result.int32 = first.int32 && second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INT64;
                    result.int64 = first.int64 && second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INT64;
                    result.int64 = first.int64 && second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer && second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer && second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad type!");
                    break;
                }
            }
            break;
        }
        default:
        {
            PalDebugbreak("Bad type!");
            break;
        }
    }

    return result;
}

struct FRAME_BLOCK ExShr(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    struct FRAME_BLOCK result = {0};

    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INT32;
                    result.int32 = first.int32 >> second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INT32;
                    result.int32 = first.int32 >> second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INT64;
                    result.int64 = first.int64 >> second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INT64;
                    result.int64 = first.int64 >> second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer >> second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer >> second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad type!");
                    break;
                }
            }
            break;
        }
        default:
        {
            PalDebugbreak("Bad type!");
            break;
        }
    }

    return result;
}

struct FRAME_BLOCK ExShl(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    struct FRAME_BLOCK result = {0};

    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INT32;
                    result.int32 = first.int32 << second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INT32;
                    result.int32 = first.int32 << second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INT64;
                    result.int64 = first.int64 << second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INT64;
                    result.int64 = first.int64 << second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer << second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer << second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad type!");
                    break;
                }
            }
            break;
        }
        default:
        {
            PalDebugbreak("Bad type!");
            break;
        }
    }

    return result;
}

struct FRAME_BLOCK ExNeg(struct FRAME_BLOCK first)
{
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            first.int32 = -first.int32;
            break;
        }
        case MACHINE_INT64:
        {
            first.int64 = -first.int64;
            break;
        }
        case MACHINE_MFLOAT:
        {
            first.floating = -first.floating;
            break;
        }
        case MACHINE_INTPTR:
        {
            first.pointer = -first.pointer;
            break;
        }
        default:
            PalDebugbreak("Bad argument!");
    }

    return first;
}

struct FRAME_BLOCK ExRem(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    struct FRAME_BLOCK result = {0};

    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INT32;
                    result.int32 = first.int32 % second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.int32 % second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT64:
                {
                    result.type = MACHINE_INT64;
                    result.int32 = first.int64 % second.int64;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_MFLOAT:
        {
            switch (second.type)
            {
                case MACHINE_MFLOAT:
                {
                    result.type = MACHINE_MFLOAT;
                    result.floating = fmod(first.floating, second.floating);
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer % second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer % second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        default:
        {
            PalDebugbreak("Bad add type!");
        }
    }

    return result;
}

struct FRAME_BLOCK ExMul(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    struct FRAME_BLOCK result = {0};

    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INT32;
                    result.int32 = first.int32 * second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.int32 * second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT64:
                {
                    result.type = MACHINE_INT64;
                    result.int32 = first.int64 * second.int64;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_MFLOAT:
        {
            switch (second.type)
            {
                case MACHINE_MFLOAT:
                {
                    result.type = MACHINE_MFLOAT;
                    result.floating = first.floating * second.floating;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer * second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer * second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        default:
        {
            PalDebugbreak("Bad add type!");
        }
    }

    return result;
}

struct FRAME_BLOCK ExDiv(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    struct FRAME_BLOCK result = {0};

    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INT32;
                    result.int32 = first.int32 / second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.int32 / second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT64:
                {
                    result.type = MACHINE_INT64;
                    result.int32 = first.int64 / second.int64;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_MFLOAT:
        {
            switch (second.type)
            {
                case MACHINE_MFLOAT:
                {
                    result.type = MACHINE_MFLOAT;
                    result.floating = first.floating / second.floating;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer / second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer / second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        default:
        {
            PalDebugbreak("Bad add type!");
        }
    }

    return result;
}

struct FRAME_BLOCK ExSub(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    struct FRAME_BLOCK result = {0};

    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INT32;
                    result.int32 = first.int32 - second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.int32 - second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT64:
                {
                    result.type = MACHINE_INT64;
                    result.int32 = first.int64 - second.int64;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_MFLOAT:
        {
            switch (second.type)
            {
                case MACHINE_MFLOAT:
                {
                    result.type = MACHINE_MFLOAT;
                    result.floating = first.floating - second.floating;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer - second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer - second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_MANAGED_POINTER:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_MANAGED_POINTER;
                    result.link.pointer = first.link.pointer - second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_MANAGED_POINTER;
                    result.link.pointer = first.link.pointer - second.pointer;
                    break;
                }
                case MACHINE_MANAGED_POINTER:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.link.pointer - second.link.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        default:
        {
            PalDebugbreak("Bad add type!");
        }
    }

    return result;
}

struct FRAME_BLOCK ExAdd(struct FRAME_BLOCK first, struct FRAME_BLOCK second)
{
    struct FRAME_BLOCK result = {0};
    switch (first.type)
    {
        case MACHINE_INT32:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INT32;
                    result.int32 = first.int32 + second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.int32 + second.pointer;
                    break;
                }
                case MACHINE_MANAGED_POINTER:
                {
                    result.type = MACHINE_MANAGED_POINTER;
                    result.link.pointer = first.int32 + second.link.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INT64:
        {
            switch (second.type)
            {
                case MACHINE_INT64:
                {
                    result.type = MACHINE_INT64;
                    result.int32 = first.int64 + second.int64;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_MFLOAT:
        {
            switch (second.type)
            {
                case MACHINE_MFLOAT:
                {
                    result.type = MACHINE_MFLOAT;
                    result.floating = first.floating + second.floating;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_INTPTR:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer + second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_INTPTR;
                    result.pointer = first.pointer + second.pointer;
                    break;
                }
                case MACHINE_MANAGED_POINTER:
                {
                    result.type = MACHINE_MANAGED_POINTER;
                    result.link.pointer = first.pointer + second.link.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        case MACHINE_MANAGED_POINTER:
        {
            switch (second.type)
            {
                case MACHINE_INT32:
                {
                    result.type = MACHINE_MANAGED_POINTER;
                    result.link.pointer = first.link.pointer + second.int32;
                    break;
                }
                case MACHINE_INTPTR:
                {
                    result.type = MACHINE_MANAGED_POINTER;
                    result.link.pointer = first.link.pointer + second.pointer;
                    break;
                }
                default:
                {
                    PalDebugbreak("Bad add type!");
                    break;
                }
            }
            break;
        }
        default:
        {
            PalDebugbreak("Bad add type!");
        }
    }

    return result;
}
