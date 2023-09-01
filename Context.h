#pragma once

#include <cstdint>

/// \file Execution context related functions.

/// Stores value of XMM register
struct XMM
{
    uint64_t l, h;
};

/// Defined in context.asm, stores the stack base pointer of current running context, nullptr if it's the main context.
extern "C" void* __global_current_ctx_stack_base;

/// Stores a context: All callee-saved registers, and context_stack_base that is used to identify a context.
struct Context
{
    XMM xmm15, xmm14, xmm13, xmm12, xmm11, xmm10, xmm9, xmm8, xmm7, xmm6;
    uint64_t r15, r14, r13, r12, rsi, rdi, rbp, rbx;
    void* context_stack_base;
    uint64_t rip, rsp;
};
constexpr size_t ContextSize = sizeof(Context);

/**
 Defined in context.asm, stores current running context into pSavedContext, and switches to pJumpContext.
 \param retOrParamForJumpCtx Determines the value of RAX and RCX after context switch.
                             If the context being switched to now has previously called SaveContextAndJump to
                             suspend itself, then SaveContextAndJump in that scope will return retOrParamForJumpCtx
                             supplied here. (RAX) \n
                             If it's a new context, retOrParamForJumpCtx will be the parameter for the function. (RCX)
*/
extern "C" void* SaveContextAndJump(Context * pSaveContext, Context * pJumpContext, void* retOrParamForJumpCtx);

/**
 Allocates stack for a new context and set it's entry/exit point.
 \param pFnExecutionEntrance Is the function to run in the new context. It can take a single 64 bit parameter.
 \param pFnContextFnReturnHandler Is the function to run after pFnExecutionEntrance returns. This function is responsible
                                  for doing a context switch to a valid context, so that the application continues to run
                                  properly if pFnExecutionEntrance returns by itself rather than switches to a valid context.
 \param contextUserdata A pointer to put into the context that can be retrieved later with \ref GetCurrentContextUserdata() or
                        GetContextUserdata()
*/
uint8_t* CreateContext(
    void* pFnExecutionEntrance,
    void* pFnContextFnReturnHandler,
    void* contextUserdata,
    Context* outContext,
    size_t stackSize = 1024 * 1024);

void* GetCurrentContextUserdata();

void* GetContextUserdata(Context* ctx);
