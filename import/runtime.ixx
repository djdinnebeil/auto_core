/**
 * \file runtime.ixx
 * \brief This module contains runtime maps and configurations if enabled.
 */
export module runtime;
import visual;
import numkey;
import main;
import taskbar;
import auto_core;
import <Windows.h>;

export {
    LRESULT CALLBACK send_numpad_event(int nCode, WPARAM wParam, LPARAM lParam);
    void process_numpad_event(const MSG& msg);
}

/**
 * \brief Constants for custom numpad key messages.
 *
 * These constants represent custom Windows messages for each numpad key,
 * starting from WM_APP + 96.
 */
export {
    constexpr int ac_numkey_0 = WM_APP + 96;
    constexpr int ac_numkey_1 = WM_APP + 97;
    constexpr int ac_numkey_2 = WM_APP + 98;
    constexpr int ac_numkey_3 = WM_APP + 99;
    constexpr int ac_numkey_4 = WM_APP + 100;
    constexpr int ac_numkey_5 = WM_APP + 101;
    constexpr int ac_numkey_6 = WM_APP + 102;
    constexpr int ac_numkey_7 = WM_APP + 103;
    constexpr int ac_numkey_8 = WM_APP + 104;
    constexpr int ac_numkey_9 = WM_APP + 105;
    constexpr int ac_numkey_star = WM_APP + 106;
    constexpr int ac_numkey_plus = WM_APP + 107;
    constexpr int ac_numkey_enter = WM_APP + 108;
    constexpr int ac_numkey_dash = WM_APP + 109;
    constexpr int ac_numkey_dot = WM_APP + 110;
    constexpr int ac_numkey_slash = WM_APP + 111;
    constexpr int ac_numkey_play_pause = WM_APP + 112;
    constexpr int ac_numkey_calc_key = WM_APP + 113;
    constexpr int ac_numkey_mail_key = WM_APP + 114;
    constexpr int ac_numkey_home_page = WM_APP + 115;
}
