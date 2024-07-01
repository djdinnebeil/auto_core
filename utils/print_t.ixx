export module print_t;

export import <tchar.h>;
export import <sstream>;
export import <iomanip>;
export import <string>;

// Using directives to avoid namespace pollution
export {
    using std::basic_string;
    using std::basic_ostringstream;
    using std::setw;
    using std::setfill;
    using std::uppercase;
    using std::hex;
}

// Convert any type to a basic_string<TCHAR>
export template<typename T>
basic_string<TCHAR> to_basic_string(const T& value) {
    basic_ostringstream<TCHAR> oss;
    oss << value;
    return oss.str();
}

// Print a single value
export template<typename T>
void print_t(const T& t) {
    _tprintf(_T("%s"), to_basic_string(t).c_str());
}

// Variadic template to print multiple values
export template<typename T, typename... Args>
void print_t(const T& t, const Args&... args) {
    _tprintf(_T("%s"), to_basic_string(t).c_str());
    _tprintf(_T(" "));
    print_t(args...);
}

// Specialization for printing TCHAR pointers
export void print_t(const TCHAR* t) {
    _tprintf(_T("%s"), t);
}

// Specialization for printing basic_string<TCHAR>
export void print_t(const basic_string<TCHAR>& t) {
    _tprintf(_T("%s"), t.c_str());
}
