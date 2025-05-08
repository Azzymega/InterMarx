#include <intermarx/ex/jit.h>

VOID ExJitEmitLoad(struct OPCODE *code, struct CPU_REGISTER *source, struct CPU_REGISTER *target)
{
    struct OPCODE opcode = {
        .opcode = CPU_LOAD,
        .firstOpreand = *source,
        .result = *target
    };

    *code = opcode;
}

VOID ExJitEmitStore(struct OPCODE *code, struct CPU_REGISTER *source, struct CPU_REGISTER *target)
{
    struct OPCODE opcode = {
        .opcode = CPU_STORE,
        .firstOpreand = *source,
        .result = *target
    };

    *code = opcode;
}

VOID ExJitEmitMove(struct OPCODE *code, struct CPU_REGISTER* source, struct CPU_REGISTER* target)
{
    struct OPCODE opcode = {
        .opcode = CPU_MOVE,
        .firstOpreand = *source,
        .result = *target
    };

    *code = opcode;
}

VOID ExJitEmitSub(struct OPCODE *code, struct CPU_REGISTER *first, struct CPU_REGISTER *second,
    struct CPU_REGISTER *target)
{
    struct OPCODE opcode = {
        .opcode = CPU_SUB,
        .firstOpreand = *first,
        .secondOpreand = *second,
        .result = *target
    };

    *code = opcode;
}

VOID ExJitEmitAdd(struct OPCODE *code, struct CPU_REGISTER *first, struct CPU_REGISTER *second,
    struct CPU_REGISTER *target)
{
    struct OPCODE opcode = {
        .opcode = CPU_ADD,
        .firstOpreand = *first,
        .secondOpreand = *second,
        .result = *target
    };

    *code = opcode;
}

VOID ExJitEmitImmediateInt(struct OPCODE *code, struct CPU_REGISTER *target, INTPTR value)
{
    struct OPCODE opcode = {
        .opcode = CPU_LOAD_IMMEDIATE_INTPTR,
        .firstOpreand = *target,
        .immediateInt = value
    };

    *code = opcode;
}

VOID ExJitEmitImmediateFloat(struct OPCODE *code, struct CPU_REGISTER *target, MFLOAT value)
{
    struct OPCODE opcode = {
        .opcode = CPU_LOAD_IMMEDIATE_INTPTR,
        .firstOpreand = *target,
        .immediateFloat = value
    };

    *code = opcode;
}
