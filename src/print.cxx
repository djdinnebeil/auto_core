module print;
import base;
import config;
import logger;
import clipboard;
import <Windows.h>;

using print_func = void(*)(const string&);

string to_string(const wstring& wstr) {
    if (wstr.empty()) return {};
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, nullptr, nullptr);
    return str;
}

string to_string(wchar_t wch) {
    wchar_t wstr[] = {wch, 0};
    return to_string(wstr);
}

void print(const string& msg) {
    cout << msg << endl;
    main_log_stream << msg << endl;
}

void printnl(const string& msg) {
    cout << msg;
    main_log_stream << msg;
}

void print(const wstring& msg) {
    cout << to_string(msg) << endl;
    main_log_stream << to_string(msg) << endl;
}

void printnl(const wstring& msg) {
    cout << to_string(msg);
    main_log_stream << to_string(msg);
}

void print(char msg) {
    cout << to_string(msg) << endl;
    main_log_stream << to_string(msg) << endl;
}

void printnl(char msg) {
    cout << to_string(msg);
    main_log_stream << to_string(msg);
}

void print(wchar_t msg) {
    cout << to_string(msg) << endl;
    main_log_stream << to_string(msg) << endl;
}

void printnl(wchar_t msg) {
    cout << to_string(msg);
    main_log_stream << to_string(msg);
}

void print_to_screen(const string& msg) {
    print(msg);
    wss ws;
    ws << msg.c_str() << L"\n\n";
    set_clipboard_text(ws.str());
    paste_from_clipboard();
}
