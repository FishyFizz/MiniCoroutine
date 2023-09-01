#include "Coroutine.h"

void Coroutine::_Entrance(void* param)
{
    // _Entrance will be called in the coroutine's context.
    // Use context userdata to determine "Who am I".
    auto p_this = (Coroutine*)GetCurrentContextUserdata();
    p_this->Main(param);
}

void Coroutine::_Exit()
{
    // _Exit will be called in the coroutine's context.
    // Use context userdata to determine "Who am I".
    auto p_this = (Coroutine*)GetCurrentContextUserdata();
    p_this->Returned();
}

void Coroutine::Returned()
{
    //Actually we can't delete the stack here because we're currently in it.
    //Stack will be deleted on next StartCoroutine, or in the destructor of object.
    coroutine_running = false;
    SaveContextAndJump(&coroutine_ctx, &caller_ctx, nullptr);
}

void Coroutine::Yield(void* yield_ret) { SaveContextAndJump(&coroutine_ctx, &caller_ctx, yield_ret); }

void* Coroutine::StartCoroutine(void* param, size_t stack_size)
{
    if (coroutine_running)
        return nullptr;

    if (stack)
        delete[] stack;

    coroutine_running = true;
    //this pointer is passed as userdata to later identify the object in static _Entrance and _Exit.
    stack = CreateContext(_Entrance, _Exit, this, &coroutine_ctx, stack_size);
    return SaveContextAndJump(&caller_ctx, &coroutine_ctx, param);
}

void* Coroutine::Resume(void* resume_param)
{
    if (!coroutine_running)
        return nullptr;
    return SaveContextAndJump(&caller_ctx, &coroutine_ctx, resume_param);
}