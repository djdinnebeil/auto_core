import base;
import print_b;
import print_t;

import <iostream>;
import <vector>;
import <Windows.h>;
import <tchar.h>;

const int max_class_name = 256; // Declare max_class_name
int window_counter = 0; // Initialize window_counter

basic_string<TCHAR> parse_and_retrieve_title(const basic_string<TCHAR>& title) {
    basic_string<TCHAR> filtered_title;
    filtered_title.reserve(title.size());
    for (TCHAR c : title) {
        if (c == 0x2014) {
            print("***character em dash [U+2014] detected***");
            filtered_title += c;  // Including the em dash in the filtered title for demonstration
        }
        else if (c == 0x200B) {
            print("***zero width space [0x200B] detected***");
        }
        else if (c == _T('\0')) {
            break;
        }
        else if (_istprint(c) || c == _T(' ')) {
            filtered_title += c;
        }
        else {
            basic_ostringstream<TCHAR> oss;
            oss << _T("***[U+") << setw(4) << setfill(_T('0')) << uppercase << hex << static_cast<int>(c) << _T("] detected!***\n");
            print_t(oss.str());
        }
    }
    return filtered_title;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    int length = GetWindowTextLength(hwnd) + 1;
    if (!IsWindowVisible(hwnd)) {
        return TRUE;  // Skip non-visible windows
    }
    basic_string<TCHAR> title(length, _T('\0'));
    basic_string<TCHAR> class_name(max_class_name, _T('\0'));
    GetWindowText(hwnd, &title[0], length);
    GetClassName(hwnd, &class_name[0], max_class_name);
    window_counter++;
    print("{}", window_counter);
    title = parse_and_retrieve_title(title);
    print_t(title, _T("\n"));
    print_t(class_name, _T("\n"));
    return TRUE;
}

int main() {
    EnumWindows(EnumWindowsProc, 0);
    print("Press enter to exit...");
    cin.get();
    return 0;
}
