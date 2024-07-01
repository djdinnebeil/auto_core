module runtime;
import visual;
import numkey;
import main;
import taskbar;
import auto_core;
import <Windows.h>;

/**
 * \brief Callback function to send numpad events.
 *
 * This function processes keyboard events and sends specific messages
 * based on the numpad key pressed.
 *
 * \param nCode The hook code. If less than zero, the hook procedure must
 *              pass the message to CallNextHookEx without further processing.
 * \param wParam The identifier of the keyboard message.
 * \param lParam A pointer to a KBDLLHOOKSTRUCT structure.
 * \return If nCode is less than zero, the hook procedure must return the
 *         value returned by CallNextHookEx. Otherwise, it must return zero.
 */
LRESULT CALLBACK send_numpad_event(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT* p = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        int vk_code = p->vkCode;
        if (vk_code == enter_key_vk && (p->flags & LLKHF_EXTENDED)) {
            vk_code = 108;
        }
        bool special_key_event = false;
        if (vk_code == play_pause_key || vk_code == calculator_key || vk_code == mail_key || vk_code == home_page_key) {
            special_key_event = true;
        }
        if (vk_code >= numkey_0 && vk_code <= numkey_slash || special_key_event) {
            logg("vk_code = {}", vk_code);
            if (vk_code == numkey_0) {
                PostThreadMessage(main_thread_id, ac_numkey_0, 1, vk_code);
            }
            else if (vk_code == numkey_1) {
                PostThreadMessage(main_thread_id, ac_numkey_1, 1, vk_code);
            }
            else if (vk_code == numkey_2) {
                PostThreadMessage(main_thread_id, ac_numkey_2, 1, vk_code);
            }
            else if (vk_code == numkey_3) {
                PostThreadMessage(main_thread_id, ac_numkey_3, 1, vk_code);
            }
            else if (vk_code == numkey_4) {
                PostThreadMessage(main_thread_id, ac_numkey_4, 1, vk_code);
            }
            else if (vk_code == numkey_5) {
                PostThreadMessage(main_thread_id, ac_numkey_5, 1, vk_code);
            }
            else if (vk_code == numkey_6) {
                PostThreadMessage(main_thread_id, ac_numkey_6, 1, vk_code);
            }
            else if (vk_code == numkey_7) {
                PostThreadMessage(main_thread_id, ac_numkey_7, 1, vk_code);
            }
            else if (vk_code == numkey_8) {
                PostThreadMessage(main_thread_id, ac_numkey_8, 1, vk_code);
            }
            else if (vk_code == numkey_9) {
                PostThreadMessage(main_thread_id, ac_numkey_9, 1, vk_code);
            }
            else if (vk_code == numkey_star) {
                PostThreadMessage(main_thread_id, ac_numkey_star, 1, vk_code);
            }
            else if (vk_code == numkey_plus) {
                PostThreadMessage(main_thread_id, ac_numkey_plus, 1, vk_code);
            }
            else if (vk_code == numkey_enter) {
                PostThreadMessage(main_thread_id, ac_numkey_enter, 1, vk_code);
            }
            else if (vk_code == numkey_dash) {
                PostThreadMessage(main_thread_id, ac_numkey_dash, 1, vk_code);
            }
            else if (vk_code == numkey_dot) {
                PostThreadMessage(main_thread_id, ac_numkey_dot, 1, vk_code);
            }
            else if (vk_code == numkey_slash) {
                PostThreadMessage(main_thread_id, ac_numkey_slash, 1, vk_code);
            }
            else if (vk_code == play_pause_key) {
                PostThreadMessage(main_thread_id, ac_numkey_play_pause, 1, vk_code);
            }
            else if (vk_code == calculator_key) {
                PostThreadMessage(main_thread_id, ac_numkey_calc_key, 1, vk_code);
            }
            else if (vk_code == mail_key) {
                PostThreadMessage(main_thread_id, ac_numkey_mail_key, 1, vk_code);
            }
            else if (vk_code == home_page_key) {
                PostThreadMessage(main_thread_id, ac_numkey_home_page, 1, vk_code);
            }
            return 1;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

/**
 * \brief Processes a numpad event message.
 *
 * This function handles the numpad event message, performing actions
 * based on the virtual key code.
 *
 * \param msg The MSG structure containing message information.
 */
void process_numpad_event(const MSG& msg) {
    int vk_code = static_cast<int>(msg.lParam);
    if (taskbar.switch_set) {
        taskbar.switch_windows(vk_code);
        primary = true;
        return;
    }
    auto ac_shortcut = ac_numkey_event.find(vk_code);
    if (ac_shortcut != ac_numkey_event.end()) {
        if (primary) {
            ac_shortcut->second.primary();
        }
        else {
            ac_shortcut->second.secondary();
        }
    }
    taskbar.switch_keycode = vk_code;
    if (!primary && vk_code != numkey_0) {
        primary = true;
    }
}
