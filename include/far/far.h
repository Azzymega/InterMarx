#pragma once
#include <intermarx.h>

struct FRAME;
struct DOMAIN;
struct FRAME_BLOCK;

enum FAR_CALL_SYMBOL_SOURCE
{
    SYMBOL_SOURCE_NONE,
    SYMBOL_SOURCE_INTERNAL,
    SYMBOL_SOURCE_EXTERNAL,
};

enum FAR_CALL_CALL_CONVENTION
{
    CALL_CONVENTION_NONE,
    CALL_CONVENTION_CDECL,
};

enum FAR_CALL_STRING_ENCODING
{
    STRING_ENCODING_NONE,
    STRING_ENCODING_ISO646,
    STRING_ENCODING_UCS2,
};

struct FAR_CALL
{
    BOOLEAN isInitialized;
    VOID* function;

    enum FAR_CALL_SYMBOL_SOURCE source;
    enum FAR_CALL_CALL_CONVENTION convention;
    enum FAR_CALL_STRING_ENCODING encoding;
};

struct FAR_IMPORT
{
    const WCHAR* name;
    VOID* function;
};

MARX_STATUS FarInitialize(struct DOMAIN* domain);
MARX_STATUS FarLoadArgument(void *argument, UINTPTR argumentSize, void *stack, UINTPTR *stackPtr);
MARX_STATUS FarNativeMethodExecute(struct FRAME* frame, struct FRAME_BLOCK* returnValue);