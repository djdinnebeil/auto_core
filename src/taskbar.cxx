module taskbar;
import base;
import config;
import visual;
import notes;
import <Windows.h>;

BOOL CALLBACK enum_folder_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR  window_title[buffer_size];
    WCHAR  className[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        GetClassNameW(hwnd, className, buffer_size);
        wstring wClass(className);
        if (wClass == L"CabinetWClass") {
            taskbar.folder_windows++;
        }
    }
    return TRUE;
}

BOOL CALLBACK enum_word_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR window_title[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        wstring title(window_title);
        if (title.length() >= 4 && title.substr(title.length() - 4) == L"Word") {
            taskbar.word_windows += 1;
        }
    }
    return TRUE;
}

BOOL CALLBACK enum_vs_code_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR window_title[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        wstring title(window_title);
        if (title.length() >= 18 && title.substr(title.length() - 18) == L"Visual Studio Code") {
            taskbar.vs_code_windows += 1;
        }
    }
    return TRUE;
}

BOOL CALLBACK enum_chrome_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR  window_title[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        wstring title(window_title);
        if (title.length() >= 13 && title.substr(title.length() - 13) == L"Google Chrome") {
            taskbar.chrome_windows++;
        }
    }
    return TRUE;
}

BOOL CALLBACK enum_visual_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR window_title[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        wstring title(window_title);
        if (title.length() >= 13 && title.substr(title.length() - 13) == L"Visual Studio") {
            taskbar.visual_windows++;
        }
    }
    return TRUE;
}

BOOL CALLBACK enum_firefox_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR window_title[buffer_size];
    WCHAR className[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        GetClassNameW(hwnd, className, buffer_size);
        wstring wTitle(window_title);
        wstring wClass(className);
        if (wClass == L"MozillaWindowClass" && wTitle != L"Mozilla Firefox Private Browsing") {
            taskbar.firefox_windows++;
        }
    }
    return TRUE;
}

void Taskbar::switch_windows(int keycode) {
    if (switch_keycode == keycode) {
        send_winkey_and_number(switch_position);
        logg("cycle window");
    }
    else {
        release_winkey();
        logg("window selected");
        switch_set = false;
        if (winkey_locked) {
            print("winkey unlocked");
            winkey_locked = false;
        }
    }
}

void Taskbar::activate_auto_core() {
    send_winkey(config.taskbar_position["auto_core"]);
}

void Taskbar::activate_folder() {
    folder_windows = 0;
    EnumWindows(enum_folder_windows, 0);
    if (folder_windows < 2) {
        send_winkey(config.taskbar_position["folder"]);
    }
    else {
        logg("multiple windows detected");
        press_and_hold_winkey();
        switch_set = true;
        send_winkey_and_number(config.taskbar_position["folder"]);
        switch_position = config.taskbar_position["folder"];
    }
}

void Taskbar::activate_word() {
    word_windows = 0;
    EnumWindows(enum_word_windows, 0);
    if (word_windows < 2) {
        send_winkey(config.taskbar_position["word"]);
    }
    else {
        logg("multiple windows detected");
        press_and_hold_winkey();
        switch_set = true;
        send_winkey_and_number(config.taskbar_position["word"]);
        switch_position = config.taskbar_position["word"];
    }
}

void Taskbar::activate_vs_code() {
    vs_code_windows = 0;
    EnumWindows(enum_vs_code_windows, 0);
    if (vs_code_windows < 2) {
        send_winkey(config.taskbar_position["vs_code"]);
    }
    else {
        logg("multiple windows detected");
        press_and_hold_winkey();
        switch_set = true;
        send_winkey_and_number(config.taskbar_position["vs_code"]);
        switch_position = config.taskbar_position["vs_code"];
    }
}

void Taskbar::activate_iTunes() {
    send_winkey(config.taskbar_position["itunes"]);
}

void Taskbar::activate_chrome() {
    chrome_windows = 0;
    EnumWindows(enum_chrome_windows, 0);
    if (chrome_windows < 2) {
        send_winkey(config.taskbar_position["chrome"]);
    }
    else {
        logg("multiple windows detected");
        press_and_hold_winkey();
        switch_set = true;
        send_winkey_and_number(config.taskbar_position["chrome"]);
        switch_position = config.taskbar_position["chrome"];
    }
}

void Taskbar::activate_visual() {
    visual_windows = 0;
    EnumWindows(enum_visual_windows, 0);
    if (visual_windows < 2) {
        send_winkey(config.taskbar_position["visual"]);
    }
    else {
        logg("multiple windows detected");
        press_and_hold_winkey();
        switch_set = true;
        send_winkey_and_number(config.taskbar_position["visual"]);
        switch_position = config.taskbar_position["visual"];
    }
}

void Taskbar::activate_discord() {
    send_winkey(config.taskbar_position["discord"]);
}

void Taskbar::activate_firefox() {
    firefox_windows = 0;
    EnumWindows(enum_firefox_windows, 0);
    if (firefox_windows < 2) {
        send_winkey(config.taskbar_position["firefox"]);
    }
    else {
        logg("multiple windows detected");
        press_and_hold_winkey();
        switch_set = true;
        send_winkey_and_number(config.taskbar_position["firefox"]);
        switch_position = config.taskbar_position["firefox"];
    }
}

void Taskbar::activate_spotify() {
    send_winkey(config.taskbar_position["spotify"]);
}

/** \runtime */
void activate_auto_core() {
    logg("activate_auto_core()");
    taskbar.activate_auto_core();
}

/** \runtime */
void activate_folder() {
    logg("activate_folder()");
    taskbar.activate_folder();
}

/** \runtime */
void activate_word() {
    logg("activate_word()");
    taskbar.activate_word();
}

/** \runtime */
void activate_vs_code() {
    logg("activate_vs_code()");
    taskbar.activate_vs_code();
}

/** \runtime */
void activate_iTunes() {
    logg("activate_iTunes()");
    taskbar.activate_iTunes();
}

/** \runtime */
void activate_chrome() {
    logg("activate_chrome()");
    taskbar.activate_chrome();
}

/** \runtime */
void activate_visual() {
    logg("activate_visual()");
    taskbar.activate_visual();
}

/** \runtime */
void activate_discord() {
    logg("activate_discord()");
    taskbar.activate_discord();
}

/** \runtime */
void activate_firefox() {
    logg("activate_firefox()");
    taskbar.activate_firefox();
}

/** \runtime */
void activate_spotify() {
    logg("activate_spotify()");
    taskbar.activate_spotify();
}
