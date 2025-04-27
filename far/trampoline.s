.global _NgAsmNativeInvokeTrampolineIntPtr
_NgAsmNativeInvokeTrampolineIntPtr:
    push %ebp
    mov %esp,%ebp

    push %edi
    push %esi

    mov %esp, %edi
    mov 12(%ebp),%esi
    mov 8(%ebp),%esp

    call *%esi

    mov %edi, %esp

    pop %esi
    pop %edi

    mov %ebp,%esp
    pop %ebp
    ret

.global _NgAsmNativeInvokeTrampolineInt32
_NgAsmNativeInvokeTrampolineInt32:
    push %ebp
    mov %esp,%ebp

    push %edi
    push %esi

    mov %esp, %edi
    mov 12(%ebp),%esi
    mov 8(%ebp),%esp

    call *%esi

    mov %edi, %esp

    pop %esi
    pop %edi

    mov %ebp,%esp
    pop %ebp
    ret

.global _NgAsmNativeInvokeTrampolineInt64
_NgAsmNativeInvokeTrampolineInt64:
    push %ebp
    mov %esp,%ebp

    push %edi
    push %esi

    mov %esp, %edi
    mov 12(%ebp),%esi
    mov 8(%ebp),%esp

    call *%esi

    mov %edi, %esp

    pop %esi
    pop %edi

    mov %ebp,%esp
    pop %ebp
    ret


.global _NgAsmNativeInvokeTrampolineSingle
_NgAsmNativeInvokeTrampolineSingle:
    push %ebp
    mov %esp,%ebp

    push %edi
    push %esi

    mov %esp, %edi
    mov 12(%ebp),%esi
    mov 8(%ebp),%esp

    call *%esi

    mov %edi, %esp

    pop %esi
    pop %edi

    mov %ebp,%esp
    pop %ebp
    ret


.global _NgAsmNativeInvokeTrampolineDouble
_NgAsmNativeInvokeTrampolineDouble:
    push %ebp
    mov %esp,%ebp

    push %edi
    push %esi

    mov %esp, %edi
    mov 12(%ebp),%esi
    mov 8(%ebp),%esp

    call *%esi

    mov %edi, %esp

    pop %esi
    pop %edi

    mov %ebp,%esp
    pop %ebp
    ret


.global _NgAsmNativeInvokeTrampolinePointer
_NgAsmNativeInvokeTrampolinePointer:
    push %ebp
    mov %esp,%ebp

    push %edi
    push %esi

    mov %esp, %edi
    mov 12(%ebp),%esi
    mov 8(%ebp),%esp

    call *%esi

    mov %edi, %esp

    pop %esi
    pop %edi

    mov %ebp,%esp
    pop %ebp
    ret
