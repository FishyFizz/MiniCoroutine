#pragma once

#include "Context.h"

class Coroutine
{
protected:

    /// \brief Stores caller's context on StartCoroutine/Resume/RunToReturn
    /// \details Yield switches to this context.
    Context caller_ctx{};

    /// \brief Stores coroutine's context on Yield.
    /// \details StartCoroutine/Resume/RunToReturn switches to this context.
    Context coroutine_ctx{};

    /// \brief Coroutine's stack space. This is kept to free it later.
    uint8_t* stack = nullptr;

    /// \breif Flag to indicate coroutine's state.
    bool coroutine_running = false;

    /// \brief The actual entrance of a coroutine.
    /// \details In _Entrance, a pointer to corresponding Coroutine object is extracted from the coroutine stack.
    /// Then \ref Main() of the object will be called.
    static void _Entrance(void* param);

    /// \brief The exit of a coroutine. When \ref Main() returns, execution point goes here.
    /// A pointer to corresponding Coroutine object is extracted from the coroutine stack.
    /// Then \ref Returned() of the object will be called.
    static void _Exit();

    /// \brief The actual coroutine code. Use Yield() to return to  \ref caller_ctx.
    /// \retval \ref StartCoroutine(), \ref Resume() or \ref RunToReturn() in caller's context will return this value.
    virtual void* Main(void* param = nullptr) {return nullptr;}

    /// \brief The coroutine suspends itself and returns to caller's context.
    /// \details Must be called in the coroutine \ref Main() function. Stores coroutine's context in \ref coroutine_ctx,
    /// then returns to \ref caller_ctx.
    /// \param yield_ret \ref StartCoroutine(),\ref Resume() or \ref RunToReturn() in caller's context will return this value.
    void Yield(void* yield_ret = nullptr);

    /// \brief Similar to \ref Yield(), but sets the \ref coroutine_running flag to false. This is automatically called
    /// by \ref _Exit() after \ref Main() returns.
    void Returned();

public:

    ~Coroutine() { if (stack) delete[] stack; }

    /// \brief Allocates the stack for coroutine, stores caller's context and switch to coroutine's context, returns when coroutine yields.
    /// \param param The parameter to coroutine's \ref Main() function.
    /// \retval Returns whatever next \ref Yield() is supplied with by the coroutine. Always nullptr if coroutine returns.
    /// \see Yield()
    void* StartCoroutine(void* param = nullptr, size_t stack_size = 1024 * 1024);

    /// \brief Stores caller's context and switch to coroutine's context, returns when coroutine yields.
    /// \param resume_param \ref Yield() in coroutine scope will return this value supplied by caller.
    /// \retval Returns whatever next Yield() is supplied with by the coroutine. Always nullptr if coroutine returns.
    /// \see Yield()
    void* Resume(void* resume_param = nullptr);
};

