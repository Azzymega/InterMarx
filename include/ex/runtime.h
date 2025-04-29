#pragma once
#include <intermarx.h>
#include <ex/ex.h>
#include <ob/ob.h>

INUSTATIC const char* const ExExecutionEngineErrorName = "System.ExecutionEngineException";
INUSTATIC const char* const ExNotImplementedName = "System.NotImplementedException";
INUSTATIC const char* const ExNullReferenceName = "System.NullReferenceException";
INUSTATIC const char* const ExIndexOutOfRangeName = "System.IndexOutOfRangeException";

//
//  Base function
//

MARX_STATUS ExInitialize();

MARX_STATUS ExMethodPrologueDelegate(struct FRAME_BLOCK* delegate, struct FRAME_BLOCK* args, struct FRAME* previous, struct FRAME_BLOCK* returnValue);

MARX_STATUS ExMethodPrologueArgs(struct METHOD* method, struct FRAME_BLOCK* args, struct FRAME* previous, struct FRAME_BLOCK* returnValue);
MARX_STATUS ExMethodPrologueCtor(struct METHOD* method, struct FRAME* previous, VOID* ptr, struct FRAME_BLOCK* returnValue);

MARX_STATUS ExMethodPrologueArgsNative(struct METHOD* method, struct FRAME_BLOCK* args, struct FRAME* previous, struct FRAME_BLOCK* returnValue);
MARX_STATUS ExMethodPrologueCtorNative(struct METHOD* method, struct FRAME* previous, VOID* ptr, struct FRAME_BLOCK* returnValue);

MARX_STATUS ExMethodPrologue(struct METHOD* method);
MARX_STATUS ExMethodEpilogue(struct FRAME* frame);
MARX_STATUS ExMethodExecute(struct FRAME* frame, struct FRAME_BLOCK* returnValue);

MARX_STATUS ExThrowException(struct MANAGED_EXCEPTION* exception);
MARX_STATUS ExHandleException(struct MANAGED_EXCEPTION** exception);
MARX_STATUS ExLocateHandler(struct HANDLER* handler, struct FRAME *frame);
MARX_STATUS ExLocateFinally(struct HANDLER* handler, struct FRAME *frame);
MARX_STATUS ExLocateVirtualMethod(struct FRAME_BLOCK *header, struct METHOD* method, struct METHOD** returnTarget);

//
//  Helpers
//

MARX_STATUS ExNullCheck(struct FRAME_BLOCK block);
MARX_STATUS ExBoundCheck(struct FRAME_BLOCK block, UINTPTR accessIndex);

//
//  Opcodes
//

struct FRAME_BLOCK ExPeek(struct FRAME* frame);
struct FRAME_BLOCK ExPop(struct FRAME* frame);
VOID ExPush(struct FRAME* frame, struct FRAME_BLOCK block);
VOID* ExGetPoolElement(struct READER* reader, struct FRAME* frame);


struct FRAME_BLOCK ExAdd(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
struct FRAME_BLOCK ExSub(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
struct FRAME_BLOCK ExDiv(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
struct FRAME_BLOCK ExMul(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
struct FRAME_BLOCK ExRem(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
struct FRAME_BLOCK ExNeg(struct FRAME_BLOCK first);
struct FRAME_BLOCK ExShl(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
struct FRAME_BLOCK ExShr(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
struct FRAME_BLOCK ExAnd(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
struct FRAME_BLOCK ExOr(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
struct FRAME_BLOCK ExXor(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
struct FRAME_BLOCK ExNot(struct FRAME_BLOCK first);

MARX_STATUS ExIsInstance(struct TYPE* castTarget, struct TYPE* objectType);

BOOLEAN ExIsZero(struct FRAME_BLOCK first);
BOOLEAN ExIsNonZero(struct FRAME_BLOCK first);
BOOLEAN ExIsEquals(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
BOOLEAN ExIsUnEquals(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
BOOLEAN ExIsGreaterEquals(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
BOOLEAN ExIsGreater(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
BOOLEAN ExIsLowerEquals(struct FRAME_BLOCK first, struct FRAME_BLOCK second);
BOOLEAN ExIsLower(struct FRAME_BLOCK first, struct FRAME_BLOCK second);


struct FRAME_BLOCK ExConvI8(struct FRAME_BLOCK first);
struct FRAME_BLOCK ExConvIntPtr(struct FRAME_BLOCK first);
struct FRAME_BLOCK ExConvI16(struct FRAME_BLOCK first);
struct FRAME_BLOCK ExConvI32(struct FRAME_BLOCK first);
struct FRAME_BLOCK ExConvI64(struct FRAME_BLOCK first);
struct FRAME_BLOCK ExConvMFloat(struct FRAME_BLOCK first);