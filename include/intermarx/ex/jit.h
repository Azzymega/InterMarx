#pragma once
#include <intermarx/intermarx.h>

enum CPU_CALLS
{
    CPU_MEMSET_CALL,
    CPU_ZEROMEM_CALL,
    CPU_THROW_CALL,
    CPU_IS_INSTANCE_CALL,
    CPU_NEWOBJECT_CALL,
};

struct CPU_REGISTER
{
    INTPTR index;
    BOOLEAN used;
    struct RUNTIME_TYPE* managed;
};

struct MACHINE_STATE
{
    struct CPU_REGISTER r0;

    struct CPU_REGISTER r1;
    struct CPU_REGISTER r2;
    struct CPU_REGISTER r3;
    struct CPU_REGISTER r4;

    struct CPU_REGISTER f1;
    struct CPU_REGISTER f2;
    struct CPU_REGISTER f3;

    struct CPU_REGISTER gp;
    struct CPU_REGISTER fp;
    struct CPU_REGISTER sp;
};

enum CPU_OPCODES
{
    CPU_NOP,
    CPU_MOVE,
    CPU_LOAD,
    CPU_STORE,

    CPU_ADD,
    CPU_DIV,
    CPU_AND,
    CPU_SUB,
    CPU_XOR,
    CPU_NEG,
    CPU_MUL,
    CPU_NOT,
    CPU_OR,
    CPU_REM,

    CPU_SHL,
    CPU_SHR,

    CPU_BRANCH_IF_EQUAL,
    CPU_BRANCH_IF_NOT_EQUAL,
    CPU_BRANCH_IF_LESS_THAN,
    CPU_BRANCH_OF_GREATER_THAN,
    CPU_BRANCH_OF_GREATER_OR_EQUAL,
    CPU_BRANCH_OF_LES_OR_EQUAL,

    CPU_CALL,
    CPU_RET,
    CPU_BREAKPOINT,

    CPU_SET_IF_EQUAL,
    CPU_SET_IF_NON_EQUAL,

    CPU_SET_IF_GREATER_THAN,
    CPU_SET_IF_LOWER_THAN,

    CPU_CONVERT_TO_FLOAT,
    CPU_LOAD_IMMEDIATE_INTPTR,
    CPU_LOAD_IMMEDIATE_FLOAT,
};

struct OPCODE
{
    enum CPU_OPCODES opcode;
    struct CPU_REGISTER firstOpreand;
    struct CPU_REGISTER secondOpreand;
    struct CPU_REGISTER result;
    INTPTR immediateInt;
    MFLOAT immediateFloat;
};

VOID ExJitEmitLoad(struct OPCODE* code, struct CPU_REGISTER* source, struct CPU_REGISTER* target);
VOID ExJitEmitStore(struct OPCODE* code, struct CPU_REGISTER* source, struct CPU_REGISTER* target);
VOID ExJitEmitMove(struct OPCODE* code, struct CPU_REGISTER* source, struct CPU_REGISTER* target);
VOID ExJitEmitSub(struct OPCODE* code, struct CPU_REGISTER* first, struct CPU_REGISTER* second, struct CPU_REGISTER* target);
VOID ExJitEmitAdd(struct OPCODE* code, struct CPU_REGISTER* first, struct CPU_REGISTER* second, struct CPU_REGISTER* target);
VOID ExJitEmitImmediateInt(struct OPCODE* code, struct CPU_REGISTER* target, INTPTR value);
VOID ExJitEmitImmediateFloat(struct OPCODE* code, struct CPU_REGISTER* target, MFLOAT value);


