/**
\file print.ixx
\brief This module provides functionality for sending output to the console.

This module includes functions to print messages to the console, with support
for formatted strings. It also handles logging of printed messages and interaction
with the clipboard.

\hardlink
*/
module;

#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif

export module print;
import base;
import config;
import logger;
import clipboard;
import <Windows.h>;

export {
    DLL_API void print(const string& msg);
    DLL_API void printnl(const string& msg);
    DLL_API void print(const wstring& msg);
    DLL_API void printnl(const wstring& msg);
    DLL_API void print(char msg);
    DLL_API void printnl(char msg);
    DLL_API void print(wchar_t msg);
    DLL_API void printnl(wchar_t msg);
    DLL_API void print_to_screen(const string& msg);
    template<typename... Args> DLL_API void print(const char* format_string, Args&&... args);
    template<typename... Args> DLL_API void printnl(const char* format_string, Args&&... args);
}

using print_func = void(*)(const string&);

DLL_API string to_string(const wstring& wstr);
DLL_API string to_string(wchar_t wch);

template<typename... Args>
DLL_API void print_template(const char* format_string, print_func println, Args&&... args) {
    try {
        if (format_string == nullptr) {
            throw invalid_argument("Format string is null");
        }
        auto convert_arg = [](auto&& arg) {
            if constexpr (is_same_v<decay_t<decltype(arg)>, const wchar_t*>) {
                return to_string(arg);
            }
            else if constexpr (is_same_v<decay_t<decltype(arg)>, wstring>) {
                return to_string(arg);
            }
            else if constexpr (is_same_v<decay_t<decltype(arg)>, wchar_t>) {
                return to_string(arg);
            }
            else {
                return forward<decltype(arg)>(arg);
            }
            };

        auto tuple_args = make_tuple(convert_arg(args)...);
        auto formatted_string = apply([&](auto&&... unpacked_args) {
            return vformat(format_string, make_format_args(unpacked_args...));
            }, tuple_args);

        println(formatted_string);
    }
    catch (const format_error& e) {
        string error = format("Format error: {}", e.what());
        print(error);
    }
    catch (const invalid_argument& e) {
        string error = format("Invalid argument: {}", e.what());
        print(error);
    }
}

template<typename... Args>
DLL_API void print(const char* format_string, Args&&... args) {
    print_template(format_string, print, forward<Args>(args)...);
}

template<typename... Args>
DLL_API void printnl(const char* format_string, Args&&... args) {
    print_template(format_string, printnl, forward<Args>(args)...);
}
