# MiniCoroutine


A minimal coroutine implementation with well commented code for learning how coroutines work.

---

### Build Instructions

You will need **CMake** and **MSVC** to build this example. And note that this example only runs on x64 platform with SSE2 extension *(most modern PC will do)*. 

- Build with Visual Studio: `Open a Local Folder`, select repository root, wait for CMake to configure. `Select Startup Item -> context.exe` then build and run.
- Build with Visual Studio Code: Make sure to have CMake extension installed, then call out command popup with `ctrl+shift+P`, search for `CMake: Build` and run. If prompted to select a toolchain, use x64 MSVC.