module utils;
import base;
import <Windows.h>;

/**
 * \brief Converts a wide string (wstring) to a standard string.
 *
 * \param wstr The wide string to convert.
 * \return The converted standard string.
 */
string wstr_to_str(const wstring& wstr) {
    if (wstr.empty()) return string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, NULL, NULL);
    return str;
}
/**
 * \brief Converts a standard string to a wide string (wstring).
 *
 * \param str The standard string to convert.
 * \return The converted wide string.
 */
wstring str_to_wstr(const string& str) {
    if (str.empty()) return wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
    return wstr;
}
/**
 * \brief Encodes a string into base64 format.
 *
 * \param in The input string to encode.
 * \return The base64 encoded string.
 */
string base64_encode(const string& in) {
    string out;
    int val = 0, valb = -6;
    for (uint8_t c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}
/**
 * \brief Removes the first and last characters from a string.
 *
 * \param input The input string.
 * \return The string with the first and last characters removed. Returns an empty string if the input length is less than or equal to 2.
 */
string remove_first_and_last_char(const string& input) {
    if (input.length() <= 2) {
        return "";
    }
    return input.substr(1, input.length() - 2);
}
