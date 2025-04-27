#include <hp/hp.h>
#include <pal/pal.h>
#include <ex/ex.h>

VOID * HpAllocateNative(UINTPTR size)
{
    return PalMemoryAllocate(size);
}

VOID * HpAllocateManaged(UINTPTR size)
{
    return PalMemoryAllocate(size);
}

VOID HpFreeNative(VOID *pointer)
{
    PalMemoryFree(pointer);
}

VOID HpRegisterRootField(struct FIELD *field)
{

}
