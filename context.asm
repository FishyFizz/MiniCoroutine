
_text SEGMENT

PUSHXMM MACRO XMMREG
    SUB RSP, 16
    MOVDQU [RSP], XMMREG
ENDM

POPXMM MACRO XMMREG
    MOVDQU XMMREG, [RSP]
    ADD RSP, 16
ENDM

ContextSize EQU 248

EXTERNDEF __global_current_ctx_stack_base:QWORD

;int64_t SaveContextAndJump(
;   void* pSaveContext,     [RCX]
;   void* pJumpContext,     [RDX]
;   int64_t retForJumpCtx   [R8]
;)

SaveContextAndJump PROC
    
    POP RAX                                     ; POP return address to RAX
    MOV R9, RSP                                 ; Save stack pointer to R9
    MOV R10, __global_current_ctx_stack_base    ; Save current context identifier to R10

    ;Set RSP to pSaveContext + ContextSize, so we can store current context to pSaveContext by PUSH-ing
    MOV RSP, RCX 
    ADD RSP, ContextSize

    ;Save context info
    PUSH R9     ;stack pointer
    PUSH RAX    ;return address
    PUSH R10    ;save __global_current_ctx_stack_base

    ;Then save the callee-saved GPRs
    PUSH RBX
    PUSH RBP
    PUSH RDI
    PUSH RSI
    PUSH R12
    PUSH R13
    PUSH R14
    PUSH R15

    ;Save the callee-saved XMM registers
    PUSHXMM XMM6
    PUSHXMM XMM7
    PUSHXMM XMM8
    PUSHXMM XMM9
    PUSHXMM XMM10
    PUSHXMM XMM11
    PUSHXMM XMM12
    PUSHXMM XMM13
    PUSHXMM XMM14
    PUSHXMM XMM15

    ;Set RSP to pJumpContext so we can restore data from pJumpContext by POP-ing
    MOV RSP, RDX

    ;Restore the XMM registers
    POPXMM XMM15
    POPXMM XMM14
    POPXMM XMM13
    POPXMM XMM12
    POPXMM XMM11
    POPXMM XMM10
    POPXMM XMM9
    POPXMM XMM8
    POPXMM XMM7
    POPXMM XMM6

    ;Restore the GPRs
    POP R15
    POP R14
    POP R13
    POP R12
    POP RSI
    POP RDI
    POP RBP
    POP RBX

    ;Read context info
    POP R9      ;context identifier
    POP R10     ;return address
    POP R11     ;stack pointer

    MOV __global_current_ctx_stack_base, R9 ;restore __global_current_ctx_stack_base
    MOV RSP, R11 ;restore stack
    PUSH R10 ;set up RET address

    ;Set up return value / parameter value
    MOV RAX, R8
    MOV RCX, R8

    RET

SaveContextAndJump ENDP 

_text ENDS

END