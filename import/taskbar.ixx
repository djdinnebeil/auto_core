/**
\file taskbar.ixx
\brief Enables quick taskbar access for the first 10 programs in the taskbar using the system shortcut of 'winkey + #'.
*/
export module taskbar;
import base;
import config;
import visual;
import <Windows.h>;

export {
    void activate_auto_core();
    void activate_folder();
    void activate_word();
    void activate_vs_code();
    void activate_iTunes();
    void activate_discord();
    void activate_chrome();
    void activate_spotify();
    void activate_visual();
    void activate_firefox();
    void refresh_firefox();
    void refresh_page();
    void start_reddit_new_tab();
}

export class Taskbar {
public:
    int switch_position;
    int switch_keycode;
    bool switch_set;
    void switch_windows(int keycode);
    void activate_auto_core();
    void activate_folder();
    void activate_word();
    void activate_vs_code();
    void activate_iTunes();
    void activate_chrome();
    void activate_visual();
    void activate_discord();
    void activate_firefox();
    void activate_spotify();
    int folder_windows = 0;
    int word_windows = 0;
    int vs_code_windows = 0;
    int chrome_windows = 0;
    int visual_windows = 0;
    int firefox_windows = 0;
    bool winkey_locked;
};

export Taskbar taskbar;
