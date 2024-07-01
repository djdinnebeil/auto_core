export module print_b;
import base;
import <Windows.h>;

export {
    void print(const string& msg);
    void printnl(const string& msg);
    void print(const wstring& msg);
    void printnl(const wstring& msg);
    void print(char msg);
    void printnl(char msg);
    void print(wchar_t msg);
    void printnl(wchar_t msg);
    template<typename... Args> void print(const char* format_string, Args&&... args);
    template<typename... Args> void printnl(const char* format_string, Args&&... args);
}

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

template<typename... Args>
void print_template(const char* format_string, print_func println, Args&&... args) {
    try {
        if (format_string == nullptr) {
            throw invalid_argument("Format string is null");
        }
        auto convert_arg = [](auto&& arg) {
            if constexpr (is_same_v<decay_t<decltype(arg)>, const wchar_t*>) {
                return to_string(arg);
            }
            else if constexpr (is_same_v<decay_t<decltype(arg)>, wstring>) {
                return to_string(arg);
            }
            else if constexpr (is_same_v<decay_t<decltype(arg)>, wchar_t>) {
                return to_string(arg);
            }
            else {
                return forward<decltype(arg)>(arg);
            }
            };

        auto tuple_args = make_tuple(convert_arg(args)...);
        auto formatted_string = apply([&](auto&&... unpacked_args) {
            return vformat(format_string, make_format_args(unpacked_args...));
            }, tuple_args);

        println(formatted_string);
    }
    catch (const format_error& e) {
        string error = format("Format error: {}", e.what());
        print(error);
    }
    catch (const invalid_argument& e) {
        string error = format("Invalid argument: {}", e.what());
        print(error);
    }
}

template<typename... Args>
void print(const char* format_string, Args&&... args) {
    print_template(format_string, print, forward<Args>(args)...);
}

template<typename... Args>
void printnl(const char* format_string, Args&&... args) {
    print_template(format_string, printnl, forward<Args>(args)...);
}

void print(const string& s) {
    cout << s << endl;
}

void printnl(const string& s) {
    cout << s;
}

void print(const wstring& msg) {
    cout << to_string(msg) << endl;
}

void printnl(const wstring& msg) {
    cout << to_string(msg);
}

void print(char msg) {
    cout << to_string(msg) << endl;
}

void printnl(char msg) {
    cout << to_string(msg);
}

void print(wchar_t msg) {
    cout << to_string(msg) << endl;
}

void printnl(wchar_t msg) {
    cout << to_string(msg);
}