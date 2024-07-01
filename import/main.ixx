/**
 * \file main.ixx
 * \brief Core support for cross-module communication and integration within the Auto Core framework.
 *
 * This module provides core functionality for managing program state and facilitating
 * communication between different modules within the Auto Core framework. It includes
 * functions for closing the program, activating and deactivating function keys, printing
 * timestamps, and setting focus to the Auto Core window.
 */
export module main;
import visual;
import <Windows.h>;
import <conio.h>;
import sp;
import itunes;
import logger_x;
import server;
import taskbar;
import taskbar_ps;

export bool primary = true;
export HWND close_window;
export HWND program_window;
export HHOOK keyboard_hook;
export DWORD main_thread_id;
export bool program_closing = false;

export {
    void close_program();
    void activate_function_key();
    void deactivate_function_key();
    void set_focus_auto_core();
    void print_timestamp();
    void print_datestamp();
    void print_today_is_day();
    void add_brackets_around_clipboard();
    void send_alt_f12();
}
