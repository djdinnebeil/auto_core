/**
 * \file keyboard.ixx
 * \brief Supports the simulation of keyboard events.
 * \hardlink
 */
module;

#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif

export module keyboard;
import base;
import <Windows.h>;

export {
    DLL_API void send_winkey(int position);
    DLL_API void send_winkey_and_number(int number);
    DLL_API void press_and_hold_winkey();
    DLL_API void release_winkey();
    DLL_API void send_key_combination(const BYTE* keys, int key_count);
}
