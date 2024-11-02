module taskbar_11;
import visual;
import taskbar;
import taskbar_ps;
import keyboard;
import main;
import <Windows.h>;

int wordpad_windows;
HWND wordpad_hwnd;
int notepad_windows;
HWND notepad_hwnd;
int powershell_windows;
HWND powershell_hwnd;
int gitbash_windows;
HWND gitbash_hwnd;
int ps_in_visual_key_windows;
HWND ps_in_visual_key_hwnd;
int webstorm_windows;
HWND webstorm_hwnd;
int zoom_windows;
HWND zoom_window_hwnd;

BOOL CALLBACK enum_zoom_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR window_title[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        wstring title(window_title);
        if (title.length() >= 12 && title.starts_with(L"Zoom Meeting")) {
            zoom_window_hwnd = hwnd;
            zoom_windows++;
        }
    }
    return TRUE;
}

BOOL CALLBACK enum_webstorm_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR  window_title[buffer_size];
    WCHAR  className[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        GetClassNameW(hwnd, className, buffer_size);
        wstring wClass(className);
        if (wClass == L"SunAwtFrame") {
            webstorm_hwnd = hwnd;
            webstorm_windows++;
        }
    }
    return TRUE;
}

BOOL CALLBACK enum_wordpad_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR  window_title[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        wstring title(window_title);
        if (title.length() >= 7 && title.substr(title.length() - 7) == L"WordPad") {
            wordpad_hwnd = hwnd;
            wordpad_windows++;
        }
    }
    return TRUE;
}

BOOL CALLBACK enum_notepad_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR window_title[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        wstring title(window_title);
        if (title.length() >= 7 && title.substr(title.length() - 7) == L"Notepad") {
            notepad_hwnd = hwnd;
            notepad_windows++;
        }
    }
    return TRUE;
}

BOOL CALLBACK enum_gitbash_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR window_title[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        wstring title(window_title);
        if (title.starts_with(L"/")) {
            gitbash_hwnd = hwnd;
            gitbash_windows++;
        }
    }
    return TRUE;
}

BOOL CALLBACK enum_powershell_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR window_title[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        wstring title(window_title);
        if (title.starts_with(L"Administrator:")) {
            powershell_hwnd = hwnd;
            powershell_windows++;
        }
    }
    return TRUE;
}

BOOL CALLBACK enum_ps_in_visual_key_windows(HWND hwnd, LPARAM lParam) {
    const int buffer_size = 1024;
    WCHAR window_title[buffer_size];
    if (GetWindowTextW(hwnd, window_title, buffer_size) > 0 && IsWindowVisible(hwnd)) {
        wstring title(window_title);
        if (title.starts_with(L"DJ@DESKTOP:")) {
            ps_in_visual_key_hwnd = hwnd;
            ps_in_visual_key_windows++;
        }
    }
    return TRUE;
}

/** \runtime */
void activate_wordpad() {
    wordpad_windows = 0;
    EnumWindows(enum_wordpad_windows, 0);
    if (!wordpad_windows) {
        logg("activate_wordpad() - launch new");
        send_ctrl_alt_x();
    }
    else if (wordpad_windows == 1) {
        HWND current_window = GetForegroundWindow();
        if (IsIconic(wordpad_hwnd)) {
            ShowWindow(wordpad_hwnd, SW_RESTORE);
            SetForegroundWindow(wordpad_hwnd);
            logg("activate_wordpad() - restore");
        }
        else if (current_window == wordpad_hwnd) {
            ShowWindow(wordpad_hwnd, SW_MINIMIZE);
            logg("activate_wordpad() - minimize");
        }
        else if (current_window != program_window) {
            activate_auto_core();
            Sleep(50);
            logg("activate_wordpad() - switch into focus");
            SetForegroundWindow(wordpad_hwnd);
        }
        else if (current_window == program_window) {
            logg("activate_wordpad() - set focus");
            SetForegroundWindow(wordpad_hwnd);
        }
    }
    else {
        activate_auto_core();
        Sleep(50);
        logg("activate_wordpad() - multiple windows detected");
        SetForegroundWindow(wordpad_hwnd);
    }
}

/** \runtime */
void activate_notepad() {
    notepad_windows = 0;
    EnumWindows(enum_notepad_windows, 0);
    if (!notepad_windows) {
        logg("activate_notepad() - launch new");
        send_ctrl_alt_n();
    }
    else if (notepad_windows == 1) {
        HWND current_window = GetForegroundWindow();
        if (IsIconic(notepad_hwnd)) {
            ShowWindow(notepad_hwnd, SW_RESTORE);
            SetForegroundWindow(notepad_hwnd);
            logg("activate_notepad() - restore");
        }
        else if (current_window == notepad_hwnd) {
            ShowWindow(notepad_hwnd, SW_MINIMIZE);
            logg("activate_notepad() - minimize");
        }
        else if (current_window != program_window) {
            activate_auto_core();
            Sleep(50);
            logg("activate_notepad() - switch into focus");
            SetForegroundWindow(notepad_hwnd);
        }
        else if (current_window == program_window) {
            logg("activate_notepad() - set focus");
            SetForegroundWindow(notepad_hwnd);
        }
    }
    else {
        activate_auto_core();
        Sleep(50);
        logg("activate_notepad() - multiple windows detected");
        SetForegroundWindow(notepad_hwnd);
    }
}

/** \runtime */
void activate_gitbash() {
    gitbash_windows = 0;
    EnumWindows(enum_gitbash_windows, 0);
    if (!gitbash_windows) {
        logg("activate_gitbash() - launch new");
        send_ctrl_alt_r();
    }
    else if (gitbash_windows == 1) {
        HWND current_window = GetForegroundWindow();
        if (IsIconic(gitbash_hwnd)) {
            ShowWindow(gitbash_hwnd, SW_RESTORE);
            SetForegroundWindow(gitbash_hwnd);
            logg("activate_gitbash() - restore");
        }
        else if (current_window == gitbash_hwnd) {
            ShowWindow(gitbash_hwnd, SW_MINIMIZE);
            logg("activate_gitbash() - minimize");
        }
        else if (current_window != program_window) {
            activate_auto_core();
            Sleep(50);
            logg("activate_gitbash() - switch into focus");
            SetForegroundWindow(gitbash_hwnd);
        }
        else if (current_window == program_window) {
            logg("activate_gitbash() - set focus");
            SetForegroundWindow(gitbash_hwnd);
        }
    }
    else {
        activate_auto_core();
        Sleep(50);
        logg("activate_gitbash() - multiple windows detected");
        SetForegroundWindow(gitbash_hwnd);
    }
}

/** \runtime */
void activate_powershell() {
    powershell_windows = 0;
    EnumWindows(enum_powershell_windows, 0);
    if (!powershell_windows) {
        logg("activate_powershell() - launch new");
        send_ctrl_alt_p();
    }
    else if (powershell_windows == 1) {
        HWND current_window = GetForegroundWindow();
        if (IsIconic(powershell_hwnd)) {
            ShowWindow(powershell_hwnd, SW_RESTORE);
            SetForegroundWindow(powershell_hwnd);
            logg("activate_powershell() - restore");
        }
        else if (current_window == powershell_hwnd) {
            ShowWindow(powershell_hwnd, SW_MINIMIZE);
            logg("activate_powershell() - minimize");
        }
        else if (current_window != program_window) {
            activate_auto_core();
            Sleep(50);
            logg("activate_powershell() - switch into focus");
            SetForegroundWindow(powershell_hwnd);
        }
        else if (current_window == program_window) {
            logg("activate_powershell() - set focus");
            SetForegroundWindow(powershell_hwnd);
        }
    }
    else {
        activate_auto_core();
        Sleep(50);
        logg("activate_powershell() - multiple windows detected");
        SetForegroundWindow(powershell_hwnd);
    }
}

/** \runtime */
void activate_ps_in_visual_key() {
    ps_in_visual_key_windows = 0;
    EnumWindows(enum_ps_in_visual_key_windows, 0);
    if (!ps_in_visual_key_windows) {
        logg("activate_ps_in_visual_key() - launch new");
        send_ctrl_alt_t();
    }
    else if (ps_in_visual_key_windows == 1) {
        HWND current_window = GetForegroundWindow();
        if (IsIconic(ps_in_visual_key_hwnd)) {
            ShowWindow(ps_in_visual_key_hwnd, SW_RESTORE);
            SetForegroundWindow(ps_in_visual_key_hwnd);
            logg("activate_ps_in_visual_key() - restore");
        }
        else if (current_window == ps_in_visual_key_hwnd) {
            ShowWindow(ps_in_visual_key_hwnd, SW_MINIMIZE);
            logg("activate_ps_in_visual_key() - minimize");
        }
        else if (current_window != program_window) {
            activate_auto_core();
            Sleep(50);
            logg("activate_ps_in_visual_key() - switch into focus");
            SetForegroundWindow(ps_in_visual_key_hwnd);
        }
        else if (current_window == program_window) {
            logg("activate_ps_in_visual_key() - set focus");
            SetForegroundWindow(ps_in_visual_key_hwnd);
        }
    }
    else {
        activate_auto_core();
        Sleep(50);
        logg("activate_ps_in_visual_key() - multiple windows detected");
        SetForegroundWindow(ps_in_visual_key_hwnd);
    }
}

/** \runtime */
void activate_webstorm() {
    webstorm_windows = 0;
    EnumWindows(enum_webstorm_windows, 0);
    if (!webstorm_windows) {
        logg("activate_webstorm() - launch new");
        send_ctrl_alt_l();
    }
    else if (webstorm_windows == 1) {
        HWND current_window = GetForegroundWindow();
        if (IsIconic(webstorm_hwnd)) {
            ShowWindow(webstorm_hwnd, SW_RESTORE);
            SetForegroundWindow(webstorm_hwnd);
            logg("activate_webstorm() - restore");
        }
        else if (current_window == webstorm_hwnd) {
            ShowWindow(webstorm_hwnd, SW_MINIMIZE);
            logg("activate_webstorm() - minimize");
        }
        else if (current_window != program_window) {
            activate_auto_core();
            Sleep(50);
            logg("activate_webstorm() - switch into focus");
            SetForegroundWindow(webstorm_hwnd);
        }
        else if (current_window == program_window) {
            logg("activate_webstorm() - set focus");
            SetForegroundWindow(webstorm_hwnd);
        }
    }
    else {
        activate_auto_core();
        Sleep(50);
        logg("activate_webstorm() - multiple windows detected");
        SetForegroundWindow(webstorm_hwnd);
    }
}

/** \runtime */
void activate_zoom() {
    zoom_windows = 0;
    EnumWindows(enum_zoom_windows, 0);
    if (!zoom_windows) {
        print("activate_zoom() - launch new - not implemented"); 
    }
    else if (zoom_windows == 1) {
        HWND current_window = GetForegroundWindow();
        if (IsIconic(zoom_window_hwnd)) {
            ShowWindow(zoom_window_hwnd, SW_RESTORE);
            SetForegroundWindow(zoom_window_hwnd);
            logg("activate_zoom() - restore");
        }
        else if (current_window == zoom_window_hwnd) {
            ShowWindow(zoom_window_hwnd, SW_MINIMIZE);
            logg("activate_zoom() - minimize");
        }
        else if (current_window != program_window) {
            activate_auto_core();
            Sleep(50);
            logg("activate_zoom() - switch into focus");
            SetForegroundWindow(zoom_window_hwnd);
        }
        else if (current_window == program_window) {
            logg("activate_zoom() - set focus");
            SetForegroundWindow(zoom_window_hwnd);
        }
    }
    else {
        activate_auto_core();
        Sleep(50);
        logg("activate_zoom() - multiple windows detected");
        SetForegroundWindow(zoom_window_hwnd);
    }
}
