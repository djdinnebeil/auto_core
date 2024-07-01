/**
\file taskbar_11.ixx
\brief Provides support for accessing applications not pinned to the taskbar or in positions 11 and above.
*/

export module taskbar_11;
import visual;
import taskbar;
import taskbar_ps;
import keyboard;
import main;
import <Windows.h>;

export {
    void activate_wordpad();
    void activate_notepad();
    void activate_powershell();
    void activate_gitbash();
    void activate_ps_in_visual_key();
    void activate_webstorm();
    void activate_zoom();
}
