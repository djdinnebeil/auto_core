/**
\file thread.ixx
\brief This provides a template for error handling in threads.

This module defines a template function for running code within a thread
with exception handling. It catches and handles any exceptions thrown
during the execution of the provided function.

\hardlink
*/
module;

#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif

export module thread;
import base;
import print;
import <Windows.h>;

/**
 * \brief Runs a function with exception handling.
 *
 * This template function takes a callable object (function, lambda, etc.) and
 * executes it within a try-catch block. It catches and prints any exceptions
 * that are thrown during the execution of the function.
 *
 * \tparam Func The type of the callable object.
 * \param func The callable object to execute.
 */
export template<typename Func>
DLL_API void run_with_exception_handling(Func func) {
    try {
        func();
    }
    catch (const exception& e) {
        print("caught exception: {}", e.what());
    }
    catch (...) {
        print("crash in thread function");
    }
}