#pragma once
#include <intermarx.h>
#include <far/far.h>
#include <ex/ex.h>
#include <ob/ob.h>
#include <rtl/rtl.h>

struct EXCEPTION_STATE;
struct FRAME;
struct MANAGED_WRAPPER;

//
//  Enums
//

INUSTATIC const CHAR *const ExByteTypeName = "System.Byte";
INUSTATIC const CHAR *const ExBooleanTypeName = "System.Boolean";
INUSTATIC const CHAR *const ExCharTypeName = "System.Char";
INUSTATIC const CHAR *const ExSByteTypeName = "System.SByte";
INUSTATIC const CHAR *const ExUInt16TypeName = "System.UInt16";
INUSTATIC const CHAR *const ExInt16TypeName = "System.Int16";
INUSTATIC const CHAR *const ExUInt32TypeName = "System.UInt32";
INUSTATIC const CHAR *const ExInt32TypeName = "System.Int32";
INUSTATIC const CHAR *const ExUInt64TypeName = "System.UInt64";
INUSTATIC const CHAR *const ExInt64TypeName = "System.Int64";
INUSTATIC const CHAR *const ExSingleTypeName = "System.Single";
INUSTATIC const CHAR *const ExDoubleTypeName = "System.Double";
INUSTATIC const CHAR *const ExUIntPtrTypeName = "System.UIntPtr";
INUSTATIC const CHAR *const ExIntPtrTypeName = "System.IntPtr";
INUSTATIC const CHAR *const ExVoidTypeName = "System.Void";
INUSTATIC const CHAR *const ExStringTypeName = "System.String";
INUSTATIC const CHAR *const ExThreadTypeName = "System.Threading.Thread";
INUSTATIC const CHAR *const ExDelegateTypeName = "System.MulticastDelegate";
INUSTATIC const CHAR *const ExFarCallTypeName = "Internationale.FarCall.FarCallAttribute";

enum EXECUTIVE_OWNER_DESCRIPTOR
{
    DESCRIPTOR_TYPE,
    DESCRIPTOR_FIELD,
    DESCRIPTOR_METHOD
};

enum HANDLER_TYPE
{
    HANDLER_UNKNOWN,
    HANDLER_CATCH,
    HANDLER_FINALLY
};

enum FRAME_BLOCK_TYPE
{
    MACHINE_UNKNOWN,
    MACHINE_INT32,
    MACHINE_INT64,
    MACHINE_INTPTR,
    MACHINE_MFLOAT,
    MACHINE_OBJECT,
    MACHINE_MANAGED_POINTER,
    MACHINE_STRUCT
};

enum METADATA_CHARACTERISTICS
{
    MxExMetadataNone,
    MxExMetadataPrivate      = 1,
    MxExMetadataPublic       = 2,
    MxExMetadataStatic       = 4,
    MxExMetadataInternal     = 8,
    MxExMetadataAbstract     = 16,
    MxExMetadataInterface    = 32,
    MxExMetadataVirtual      = 64,
    MxExMetadataGetter       = 128,
    MxExMetadataSetter       = 256,
    MxExMetadataExtern       = 512,
    MxExMetadataPrimitive    = 1024,
    MxExMetadataClass        = 2048,
    MxExMetadataEnum         = 4096,
    MxExMetadataStruct       = 8192,
    MxExMetadataConstructor  = 16384,
    MxExMetadataMonitor      = 32768,
    MxExMetadataArray        = 65536,
    MxExMetadataSynthetic    = 131072,
    MxExMetadataPointer      = 262144,
    MxExMetadataAggregate    = 524288,
    MxExMetadataSealed       = 1048576,
    MxExMetadataImport       = 2097152,
    MxExMetadataDelegate     = 4194304,
    MxExMetadataFinal        = 8388608,
    MxExMetadataInitialize   = 16777216,
    MxExMetadataReference    = 33554432,
    MxExMetadataShapeshifter = 67108864,
    MxExMetadataTemplate     = 134217728
};


enum BYTECODE : BYTE
{
    OpBreakpoint,
    OpNoOperation,

    OpDup,
    OpPop,

    OpAdd,
    OpSub,
    OpMu,
    OpDiv,
    OpRem,
    OpNeg,

    OpAnd,
    OpOr,
    OpXor,
    OpNot,

    OpShiftLeft,
    OpShiftRight,

    OpConvertI8,
    OpConvertI16,
    OpConvertI32,
    OpConvertI64,
    OpConvertFloat,
    OpConvertDouble,
    OpConvertIntPtr,

    OpLoadValueFieldAddress,
    OpLoadLocalVariableAddress,
    OpLoadArgumentAddress,
    OpLoadValueFromPointer,

    OpLoadValueField,
    OpLoadStaticField,
    OpStoreValueField,
    OpStoreStaticField,

    OpLoadArgument,
    OpStoreArgument,
    OpLoadLocal,
    OpStoreLocal,

    OpLoadNull,
    OpLoadString,
    OpLoadMethodDescriptor,
    OpLoadVirtualMethodDescriptor,

    OpNewArray,
    OpInitializeObject,
    OpNewObject,

    OpLoadImmediateInt32,
    OpLoadImmediateInt64,
    OpLoadImmediateFloat,
    OpLoadImmediateDouble,

    OpLoadNativeIntFromArray,
    OpLoadInt8FromArray,
    OpLoadInt16FromArray,
    OpLoadInt32FromArray,
    OpLoadInt64FromArray,
    OpLoadFloatFromArray,
    OpLoadDoubleFromArray,
    OpLoadObjectFromArray,

    OpLoadArrayLength,

    OpStoreNativeIntToArray,
    OpStoreInt8ToArray,
    OpStoreInt16ToArray,
    OpStoreInt32ToArray,
    OpStoreInt64ToArray,
    OpStoreFloatToArray,
    OpStoreDoubleToArray,
    OpStoreObjectToArray,

    OpBranchIfEquals,
    OpBranchIfGreaterOrEqual,
    OpBranchIfGreater,
    OpBranchIfLessOrEqual,
    OpBranchIfLess,
    OpBranchIfZero,
    OpBranchIfNonZero,
    OpBranch,

    OpBranchIfUnequalUnordered,

    OpPushOneIfEqual,
    OpPushOneIfGreater,
    OpPushOneIfLower,
    OpPushOneIfGreaterUn,
    OpPushOneIfLowerUn,

    OpCall,
    OpVirtualCall,
    OpReturn,

    OpIsInstance,
    OpCastClass,

    OpBox,
    OpUnboxToPointer,
    OpUnboxToValue,

    OpThrowException,
    OpEndFinallyException,
    OpRethrowException,
    OpLeaveException,
};

enum BASE_TYPE_SYSTEM_TYPE
{
    BASE_OTHER,
    BASE_INTPTR,
    BASE_VOID,
    BASE_INT64,
    BASE_INT32,
    BASE_INT16,
    BASE_SINGLE,
    BASE_DOUBLE,
    BASE_BYTE,
    BASE_CHAR,
};

//
//  Native representations
//

struct EXCEPTION_STATE
{
    VOID* exception;
    BOOLEAN isUnwinding;
    struct VECTOR stackTrace;
};

struct TYPE
{
    enum EXECUTIVE_OWNER_DESCRIPTOR descriptor;
    struct VECTOR attributes;

    UINTPTR size;
    BOOLEAN hasFinalizer;

    enum BASE_TYPE_SYSTEM_TYPE inlined;
    enum METADATA_CHARACTERISTICS metadata;

    struct TYPE* super;
    struct DOMAIN* domain;

    struct VECTOR interfaces;
    struct VECTOR fields;
    struct VECTOR methods;

    struct NSTRING fullName;
    struct NSTRING shortName;
};

struct DOMAIN
{
    UINTPTR id;

    struct VECTOR types;
    struct VECTOR threads;
};

struct THREAD
{
    UINTPTR id;
    NATIVE_HANDLE handle;
    BOOLEAN inSafePoint;

    struct DOMAIN* domain;
    struct EXCEPTION_STATE state;

    struct MANAGED_WRAPPER* wrapper;
    struct MANAGED_DELEGATE* delegate;

    struct FRAME* currentFrame;
    struct FRAME* firstFrame;
};

struct FIELD
{
    enum EXECUTIVE_OWNER_DESCRIPTOR descriptor;
    struct VECTOR attributes;

    UINTPTR dataSize;
    UINTPTR offset;

    enum METADATA_CHARACTERISTICS metadata;

    struct NSTRING fullName;
    struct NSTRING shortName;

    struct TYPE* owner;
    struct TYPE* declared;

    VOID* staticValue;
};

struct METHOD
{
    enum EXECUTIVE_OWNER_DESCRIPTOR descriptor;
    struct VECTOR attributes;

    enum METADATA_CHARACTERISTICS metadata;
    BOOLEAN isReturns;

    struct NSTRING fullName;
    struct NSTRING shortName;

    struct TYPE* owner;
    struct TYPE* returnType;

    struct VECTOR pool;
    struct VECTOR parameters;
    struct VECTOR variables;
    struct VECTOR stringTable;
    struct VECTOR handlers;

    BYTE* bytecode;

    struct FAR_CALL farCall;
};

struct HANDLER
{
    enum HANDLER_TYPE handler;

    struct METHOD* owner;
    struct TYPE* catch;

    INTPTR handlerStart;
    INTPTR handlerEnd;
    INTPTR tryStart;
    INTPTR tryEnd;
};

//
//  Executive elements
//

struct FRAME_BLOCK_MANAGED_POINTER
{
    struct TYPE *type;
    VOID *pointer;
};

struct FRAME_BLOCK_STRUCT
{
    struct TYPE *type;
    VOID *pointer;
};

struct FRAME_BLOCK
{
    enum FRAME_BLOCK_TYPE type;

    union
    {
        VOID* descriptor;
        INT32 int32;
        INT64 int64;
        INTPTR pointer;
        MFLOAT floating;
        struct FRAME_BLOCK_MANAGED_POINTER link;
        struct FRAME_BLOCK_STRUCT valueType;
    };
};

struct FRAME
{
    struct READER* reader;
    INTPTR sp;
    INTPTR maxStack;

    struct FRAME_BLOCK* stack;
    struct FRAME_BLOCK* args;
    struct FRAME_BLOCK* variables;

    struct METHOD* method;
    struct DOMAIN* domain;
    struct THREAD* thread;

    struct FRAME* next;
};

BOOLEAN ExMetadataIs(enum METADATA_CHARACTERISTICS thisValue, enum METADATA_CHARACTERISTICS metadata);
VOID ExDomainInitialize(struct DOMAIN* thisPtr);
VOID ExTypeInitialize(struct TYPE* thisPtr);
VOID ExFieldInitialize(struct FIELD* thisPtr);
VOID ExMethodInitialize(struct METHOD* thisPtr);
VOID ExThreadInitialize(struct THREAD* thisPtr, struct DOMAIN* domain);

VOID ExExceptionStateInitialize(struct EXCEPTION_STATE* thisPtr);
VOID ExExceptionStateAppend(struct EXCEPTION_STATE* thisPtr, struct METHOD* method);
VOID ExExceptionStateDrop(struct EXCEPTION_STATE* thisPtr);

struct TYPE* ExDomainLocateType(struct DOMAIN* thisPtr, const CHAR* string);
struct METHOD* ExTypeLocateMethod(struct TYPE* thisPtr, const CHAR* shortName);