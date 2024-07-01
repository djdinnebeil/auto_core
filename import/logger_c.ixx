 /**
 \file logger_c.ixx
 \brief Offers specialized logging capabilities focused on individual components like music player activity.

 This module provides a Logger class that supports specialized logging for individual components.
 It includes functions for logging messages, updating log files, and handling formatted log messages.
 \hardlink
  */
module;

#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif

export module logger_c;
import base;
import config;
import clock;
import logger;
import print;
import <Windows.h>;

DLL_API string to_string(const wstring& wstr);
DLL_API string to_string(wchar_t wch);

export class Logger {
public:
    DLL_API Logger(const string& log_name);
    DLL_API ~Logger();
    string name;
    string directory;
    string session_started;
    DLL_API void logg(const string& msg);
    DLL_API void loggnl(const string& msg);
    DLL_API void logg_and_print(const string& msg);
    DLL_API void logg_and_logg(const string& msg);
    DLL_API void loggnl_and_printnl(const string& msg);
    DLL_API void loggnl_and_loggnl(const string& msg);
    DLL_API void logg(const wstring& msg);
    DLL_API void loggnl(const wstring& msg);
    DLL_API void logg_and_print(const wstring& msg);
    DLL_API void logg_and_logg(const wstring& msg);
    DLL_API void loggnl_and_printnl(const wstring& msg);
    DLL_API void loggnl_and_loggnl(const wstring& msg);
    DLL_API void logg(char msg);
    DLL_API void loggnl(char msg);
    DLL_API void logg_and_print(char msg);
    DLL_API void logg_and_logg(char msg);
    DLL_API void loggnl_and_printnl(char msg);
    DLL_API void loggnl_and_loggnl(char msg);
    DLL_API void logg(wchar_t msg);
    DLL_API void loggnl(wchar_t msg);
    DLL_API void logg_and_print(wchar_t msg);
    DLL_API void logg_and_logg(wchar_t msg);
    DLL_API void loggnl_and_printnl(wchar_t msg);
    DLL_API void loggnl_and_loggnl(wchar_t msg);
    template<typename... Args>
    DLL_API void logg(const char* format_string, Args&&... args);
    template<typename... Args>
    DLL_API void loggnl(const char* format_string, Args&&... args);
    template<typename... Args>
    DLL_API void logg_and_print(const char* format_string, Args&&... args);
    template<typename... Args>
    DLL_API void loggnl_and_printnl(const char* format_string, Args&&... args);
    template<typename... Args>
    DLL_API void logg_and_logg(const char* format_string, Args&&... args);
    template<typename... Args>
    DLL_API void loggnl_and_loggnl(const char* format_string, Args&&... args);
    DLL_API void update_log_file();
    DLL_API void open_log_file();
    DLL_API void close_log_file();
    ofstream log_stream;
};

template<typename... Args>
DLL_API void Logger::logg(const char* format_string, Args&&... args) {
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
        log_stream << formatted_string << endl;
        if (config.send_logg_to_cout) {
            cout << formatted_string << endl;
        }
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
DLL_API void Logger::loggnl(const char* format_string, Args&&... args) {
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
        log_stream << formatted_string;
        if (config.send_logg_to_cout) {
            cout << formatted_string;
        }
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
DLL_API void Logger::logg_and_print(const char* format_string, Args&&... args) {
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
        log_stream << formatted_string << endl;
        print(formatted_string);
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
DLL_API void Logger::loggnl_and_printnl(const char* format_string, Args&&... args) {
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
        log_stream << formatted_string;
        printnl(formatted_string);
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
DLL_API void Logger::logg_and_logg(const char* format_string, Args&&... args) {
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
        log_stream << formatted_string << endl;
        ::logg(formatted_string);
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
DLL_API void Logger::loggnl_and_loggnl(const char* format_string, Args&&... args) {
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
        log_stream << formatted_string;
        ::loggnl(formatted_string);
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
