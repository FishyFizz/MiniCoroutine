#include "Context.h"

extern "C" void* __global_current_ctx_stack_base = nullptr;

uint8_t* CreateContext(
    void* pFnExecutionEntrance, 
    void* pFnContextFnReturnHandler, 
    void* contextUserdata,
    Context* outContext , 
    size_t stackSize)
{
    // Allocate stack space for new context and set its entrance and stack pointer.
    uint8_t* stack = new uint8_t[stackSize];
    outContext->rsp = (uint64_t)(stack + stackSize); 
    outContext->rip = (uint64_t)pFnExecutionEntrance;

    // Use the stack base to identify the context.
    outContext->context_stack_base = (void*)outContext->rsp;

    // Push contextUserdata, later we can read from this place to get it back.
    outContext->rsp -= 8;
    *((uint64_t*)(outContext->rsp)) = (uint64_t)contextUserdata;

    // Some compiler assumes parameters passed in RCX/RDX/R8/R9 are also pushed onto the stack, and generate
    // function prologues that writes RCX/RDX/R8/R9 back to their "home address".
    // If that happens in our case, userdata could be overwritten.
    // We don't want that so here we offset the stack frame by a "sufficient distance".
    outContext->rsp -= 64; 

    // Push return address to context stack. So after pFnExecutionEntrance is done, RET instruction pops this out and
    // jumps to the address.
    outContext->rsp -= 8;
    *((uint64_t*)(outContext->rsp)) = (uint64_t)pFnContextFnReturnHandler;
    outContext->rbp = outContext->rsp;

    return stack;
}

void* GetCurrentContextUserdata()
{
    if (__global_current_ctx_stack_base == nullptr)
        return nullptr;
    return *((void**)__global_current_ctx_stack_base - 1);
}

void *GetContextUserdata(Context *ctx) {
    if (ctx = nullptr)
        return nullptr;
    if (ctx->context_stack_base == nullptr)
        return nullptr;
    return *((void**)(ctx->context_stack_base) - 1);
}

