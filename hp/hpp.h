#pragma once

VOID* HppTransferObject(struct OBJECT_HEADER* header);
VOID HppTraceStruct(VOID* structure, struct TYPE* type);
VOID* HppTraceObject(struct OBJECT_HEADER* header);
VOID HppTraceField(struct FIELD* field);