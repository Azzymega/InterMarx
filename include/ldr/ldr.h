#pragma once
#include <intermarx.h>

enum SECTION_TYPE
{
    NlSectionHeader,
    NlSectionString,
    NlSectionType,
    NldSectionField,
    NlSectionMethod,
    NlSectionCode,
    NlSectionHandler,
    NlSectionAttribute,
    NlSectionArgument
};

struct INUPACKED LOADER_SECTION {
    UINT32 size;
    enum SECTION_TYPE type;
};

struct IMAGE_LOADER
{
    VOID* image;
    struct NSTRING* stringTable;
    VOID** sections;
    struct DOMAIN* domain;
    UINTPTR offset;
};

VOID* LdrLoadStringTable(struct IMAGE_LOADER* thisPtr);
VOID* LdrLoadType(struct IMAGE_LOADER* thisPtr);
VOID* LdrLoadField(struct IMAGE_LOADER* thisPtr);
VOID* LdrLoadMethod(struct IMAGE_LOADER* thisPtr);
VOID* LdrLoadExecutable(struct IMAGE_LOADER* thisPtr);
VOID* LdrLoadHandler(struct IMAGE_LOADER* thisPtr);
VOID* LdrLoadAttribute(struct IMAGE_LOADER* thisPtr);
VOID* LdrLoadArgument(struct IMAGE_LOADER* thisPtr);

VOID LdrExecuteStaticConstructors(struct DOMAIN* thisPtr);
VOID LdrExecuteAttributesConstructors(struct DOMAIN* thisPtr);

VOID LdrImageLoaderInitialize(struct IMAGE_LOADER* thisPtr, VOID* imageAddress);
VOID LdrImageLoaderWind(struct IMAGE_LOADER* thisPtr, UINTPTR length);
VOID LdrImageRead(struct IMAGE_LOADER* thisPtr, VOID* dest, UINTPTR length);
UINT32 LdrImageReadIndex(struct IMAGE_LOADER* thisPtr);

VOID LdrFillTypeInfo(struct DOMAIN* thisPtr);
VOID LdrCalculateTypeSizes(struct DOMAIN* thisPtr);
UINTPTR LdrCalculateTypeSize(struct TYPE* thisPtr);

struct DOMAIN* LdrLoadDomain(struct IMAGE_LOADER* thisPtr);