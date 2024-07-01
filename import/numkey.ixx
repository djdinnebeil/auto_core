/**
\file numkey.ixx
\brief Defines key codes and messages for numeric keypad and special key handling within Auto Core.

This module provides constant definitions for virtual key codes related to the numeric keypad and
special keys. It includes a custom value for the numpad enter key to differentiate its behavior
within Auto Core.

\note Auto Core uses a custom value of 108 for the numpad enter, while the actual vk code is 13--the
same as the main keyboard enter vk code. However, the numpad enter key is intended to have a distinct
behavior within Auto Core. To differentiate it, we check for the LLKHF_EXTENDED flag, which is set for
the numpad enter key press but not for the main keyboard enter key press.
*/
export module numkey;
import visual;
import <Windows.h>;

export {
    const int enter_key_vk = 13;
    const int numkey_enter = 108;
    const int numkey_0 = 96;
    const int numkey_1 = 97;
    const int numkey_2 = 98;
    const int numkey_3 = 99;
    const int numkey_4 = 100;
    const int numkey_5 = 101;
    const int numkey_6 = 102;
    const int numkey_7 = 103;
    const int numkey_8 = 104;
    const int numkey_9 = 105;
    const int numkey_star = 106;
    const int numkey_plus = 107;
    const int numkey_dash = 109;
    const int numkey_dot = 110;
    const int numkey_slash = 111;
    const int home_page_key = 172;
    const int play_pause_key = 179;
    const int mail_key = 180;
    const int calculator_key = 183;
}
