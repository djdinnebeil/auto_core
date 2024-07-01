/**
\file logger.ixx
\brief Defines a custom logger featuring logg() and loggnl().

This module provides logging functionality for the Auto Core application. It includes
functions to log messages with and without newline characters, update the log file,
and handle formatting errors.
\hardlink
*/
module;

#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif

export module logger;
import base;
import config;
import clock;
import <Windows.h>;

export string session_start;
export ofstream main_log_stream;
export string main_log_name;
export string logger_datestamp;
export string log_directory = R"(.\log\)";

export {
    DLL_API void logg(const string& msg);
    DLL_API void loggnl(const string& msg);
    DLL_API void logg(const wstring& msg);
    DLL_API void loggnl(const wstring& msg);
    DLL_API void logg(char msg);
    DLL_API void loggnl(char msg);
    DLL_API void logg(wchar_t msg);
    DLL_API void loggnl(wchar_t msg);
    DLL_API void update_main_log_file();
    DLL_API void close_main_log_file();
    DLL_API void log_end();
    template<typename... Args> DLL_API void logg(const char* format_string, Args&&... args);
    template<typename... Args> DLL_API void loggnl(const char* format_string, Args&&... args);
}

using logger_func = void(*)(const string&);

DLL_API string to_string(const wstring& wstr);
DLL_API string to_string(wchar_t wch);

template<typename... Args>
DLL_API void logger_template(const char* format_string, logger_func logger, Args&&... args) {
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

        logger(formatted_string);
    }
    catch (const format_error& e) {
        string error = format("Format error: {}", e.what());
        logg(error);
    }
    catch (const invalid_argument& e) {
        string error = format("Invalid argument: {}", e.what());
        logg(error);
    }
}

template<typename... Args>
DLL_API void logg(const char* format_string, Args&&... args) {
    logger_template(format_string, logg, forward<Args>(args)...);
}

template<typename... Args>
DLL_API void loggnl(const char* format_string, Args&&... args) {
    logger_template(format_string, loggnl, forward<Args>(args)...);
}
