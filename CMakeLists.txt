project (context)
cmake_minimum_required(VERSION 3.00)
enable_language(C CXX ASM_MASM)

add_executable(context context.cpp context.asm "Context.h" "main.cpp" "Coroutine.h" "Coroutine.cpp")