module main;
import visual;
import <Windows.h>;
import <conio.h>;
import sp;
import itunes;
import logger_x;
import server;
import taskbar;
import taskbar_ps;

/**
 * \brief Closes the program.
 *
 * This function performs the necessary cleanup and shutdown operations for the program.
 * It stops the server, sends end signals to iTunes and Spotify, destroys the taskbar window,
 * unhooks the keyboard hook, and posts a quit message to the main thread.
 *
 * \runtime
 */
void close_program() {
    logg("close_program()");
    program_closing = true;
    stop_server();
    send_iTunes_end_signal();
    send_sp_end_signal();
    send_wake_end_signal();
    if (taskbar_ps_hwnd != NULL) {
        DestroyWindow(taskbar_ps_hwnd);
    }
    if (keyboard_hook != NULL) {
        UnhookWindowsHookEx(keyboard_hook);
    }
    PostThreadMessage(main_thread_id, WM_QUIT, 0, 0);
}

/**
 * \brief Activates the function key.
 *
 * This function sets the primary flag to false and logs the activation of the function key.
 *
 * \runtime
 */
void activate_function_key() {
    primary = false;
    print("Function key activated");
}

/**
 * \brief Deactivates the function key.
 *
 * This function sets the primary flag to true and logs the deactivation of the function key.
 *
 * \runtime
 */
void deactivate_function_key() {
    primary = true;
    print("Function key deactivated");
}

/**
 * \brief Prints the current timestamp.
 * 
 * This function prints the current timestamp to the screen.
 * \runtime
 */
void print_timestamp() {
    wstring most_recent_clipboard_text = get_clipboard_text();
    Sleep(50);
    print_to_screen(get_timestamp());
    Sleep(100);
    set_clipboard_text(most_recent_clipboard_text);
}

/**
 * \brief Prints the current datestamp.
 *
 * This function prints the current timestamp to the screen.
 * \runtime
 */
void print_datestamp() {
    print_to_screen(get_datestamp());
}
/**
 * \brief Prints the day is today.
 *
 */
void print_today_is_day() {
    print("Today is {}", get_day_of_week());
}

/**
 * \brief Clears the input buffer.
 *
 * This function clears any remaining input in the input buffer.
 */
void clear_input_buffer() {
    while (_kbhit()) {
        int getch = _getch();
    }
    cin.clear();
}

/**
 * \brief Sets focus to the Auto Core window.
 *
 * This function sets the focus to the Auto Core window, clearing the input buffer
 * and activating the Auto Core window if it is not already in focus.
 */
void set_focus_auto_core() {
    logg("set_focus_auto_core()");
    clear_input_buffer();
    HWND current_window_hwnd = GetForegroundWindow();
    if (current_window_hwnd != program_window) {
        taskbar.activate_auto_core();
    }
}

/**
 * \brief Adds brackets around the clipboard text.
 * \runtime
 * This function adds brackets around the current text in the clipboard and pastes it.
 */
void add_brackets_around_clipboard() {
    wstring clipboard_item = L"[" + get_clipboard_text() + L"]";
    print(clipboard_item);
    set_clipboard_text(clipboard_item);
    paste_from_clipboard();
}

/**
 * \brief Sends keyboard event of 'alt + f12'
 *
 * This event launches a terminal window in WebStorm.
 * \runtime
 */
void send_alt_f12() {
    INPUT inputs[4] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_MENU; // Virtual key code for Alt
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_F12; // Virtual key code for F12
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = VK_F12;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_MENU;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}