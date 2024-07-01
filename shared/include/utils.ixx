/**
\file utils.ixx
\brief Provides utility functions.

This module contains various utility functions used throughout the application,
including string conversions and base64 encoding.

\hardlink
*/
module;

#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif

export module utils;
import base;
import <Windows.h>;

export {
    DLL_API string wstr_to_str(const wstring& wstr);
    DLL_API wstring str_to_wstr(const string& str);
    DLL_API string base64_encode(const string& in);
    DLL_API string remove_first_and_last_char(const string& input);
}
