module logger;
import base;
import config;
import clock;
import <Windows.h>;

using logger_func = void(*)(const string&);

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

void logg(const string& msg) {
    main_log_stream << msg << endl;
    if (config.send_logg_to_cout) {
        cout << msg << endl;
    }
}

void loggnl(const string& msg) {
    main_log_stream << msg;
    if (config.send_logg_to_cout) {
        cout << msg;
    }
}

void logg(const wstring& msg) {
    main_log_stream << to_string(msg) << endl;
    if (config.send_logg_to_cout) {
        cout << to_string(msg) << endl;
    }
}

void loggnl(const wstring& msg) {
    main_log_stream << to_string(msg);
    if (config.send_logg_to_cout) {
        cout << to_string(msg);
    }
}

void logg(char msg) {
    main_log_stream << to_string(msg) << endl;
    if (config.send_logg_to_cout) {
        cout << to_string(msg) << endl;
    }
}

void loggnl(char msg) {
    main_log_stream << to_string(msg);
    if (config.send_logg_to_cout) {
        cout << to_string(msg);
    }
}

void logg(wchar_t msg) {
    main_log_stream << to_string(msg) << endl;
    if (config.send_logg_to_cout) {
        cout << to_string(msg) << endl;
    }
}

void loggnl(wchar_t msg) {
    main_log_stream << to_string(msg);
    if (config.send_logg_to_cout) {
        cout << to_string(msg);
    }
}

void update_main_log_file() {
    close_main_log_file();
    logger_datestamp = get_datestamp();
    main_log_name = "log_" + logger_datestamp + ".log";
    string logger_path = log_directory + main_log_name;
    main_log_stream.open(logger_path, ios::app);
}

/**
 * \brief Closes the main log file.
 *
 * Flushes and closes the main log file if it is open.
 */
void close_main_log_file() {
    if (main_log_stream.is_open()) {
        main_log_stream.flush();
        main_log_stream.close();
    }
}

/**
 * \brief Ends the logging session.
 *
 * Logs the session end message and closes the main log file.
 */
void log_end() {
    logg("Session ended {}", get_datetime_stamp_for_logger());
    logg("***");
    close_main_log_file();
}
