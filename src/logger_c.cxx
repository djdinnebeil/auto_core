module logger_c;
import base;
import config;
import clock;
import logger;
import print;
import <Windows.h>;

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

Logger::Logger(const string& log_name) {
    name = log_name;
    directory = log_directory + name + "\\";
    open_log_file();
}

Logger::~Logger() {
    close_log_file();
}

void Logger::logg(const string& msg) {
    log_stream << msg << endl;
    if (config.send_logg_to_cout) {
        cout << msg << endl;
    }
}

void Logger::loggnl(const string& msg) {
    log_stream << msg;
    if (config.send_logg_to_cout) {
        cout << msg;
    }
}

void Logger::logg_and_logg(const string& msg) {
    log_stream << msg << endl;
    ::logg(msg);
}

void Logger::loggnl_and_loggnl(const string& msg) {
    log_stream << msg;
    ::loggnl(msg);
}

void Logger::logg_and_print(const string& msg) {
    log_stream << msg << endl;
    print(msg);
}

void Logger::loggnl_and_printnl(const string& msg) {
    log_stream << msg;
    printnl(msg);
}

void Logger::logg(const wstring& msg) {
    log_stream << to_string(msg) << endl;
    if (config.send_logg_to_cout) {
        cout << to_string(msg) << endl;
    }
}

void Logger::loggnl(const wstring& msg) {
    log_stream << to_string(msg);
    if (config.send_logg_to_cout) {
        cout << to_string(msg);
    }
}

void Logger::logg_and_logg(const wstring& msg) {
    log_stream << to_string(msg) << endl;
    ::logg(to_string(msg));
}

void Logger::loggnl_and_loggnl(const wstring& msg) {
    log_stream << to_string(msg);
    ::loggnl(to_string(msg));
}

void Logger::logg_and_print(const wstring& msg) {
    log_stream << to_string(msg) << endl;
    print(to_string(msg));
}

void Logger::loggnl_and_printnl(const wstring& msg) {
    log_stream << to_string(msg);
    printnl(to_string(msg));
}

void Logger::logg(char msg) {
    log_stream << to_string(msg) << endl;
    if (config.send_logg_to_cout) {
        cout << to_string(msg) << endl;
    }
}

void Logger::loggnl(char msg) {
    log_stream << to_string(msg);
    if (config.send_logg_to_cout) {
        cout << to_string(msg);
    }
}

void Logger::logg_and_logg(char msg) {
    log_stream << to_string(msg) << endl;
    ::logg(to_string(msg));
}

void Logger::loggnl_and_loggnl(char msg) {
    log_stream << to_string(msg);
    ::loggnl(to_string(msg));
}

void Logger::logg_and_print(char msg) {
    log_stream << to_string(msg) << endl;
    print(to_string(msg));
}

void Logger::loggnl_and_printnl(char msg) {
    log_stream << to_string(msg);
    printnl(to_string(msg));
}

void Logger::logg(wchar_t msg) {
    log_stream << to_string(msg) << endl;
    if (config.send_logg_to_cout) {
        cout << to_string(msg) << endl;
    }
}

void Logger::loggnl(wchar_t msg) {
    log_stream << to_string(msg);
    if (config.send_logg_to_cout) {
        cout << to_string(msg);
    }
}

void Logger::logg_and_logg(wchar_t msg) {
    log_stream << to_string(msg) << endl;
    ::logg(to_string(msg));
}

void Logger::loggnl_and_loggnl(wchar_t msg) {
    log_stream << to_string(msg);
    ::loggnl(to_string(msg));
}

void Logger::logg_and_print(wchar_t msg) {
    log_stream << to_string(msg) << endl;
    print(to_string(msg));
}

void Logger::loggnl_and_printnl(wchar_t msg) {
    log_stream << to_string(msg);
    printnl(to_string(msg));
}

/**
 * \brief Updates the log file.
 *
 * Closes the current log file (if open), updates the log file name based on the current date,
 * and opens a new log file.
 */
void Logger::update_log_file() {
    string session_status = "";
    if (log_stream.is_open()) {
        session_status = "---\n";
        log_stream << session_status;
        log_stream.flush();
        log_stream.close();
    }
    session_status += session_started;
    string datestamp = get_datestamp();
    string filename = name + "_" + datestamp + ".log";
    string logger_path = directory + filename;
    log_stream.open(logger_path, ios::app);
    log_stream << session_status << endl;
}

/**
 * \brief Opens the log file.
 *
 * Creates the necessary directories and opens the log file for writing.
 */
void Logger::open_log_file() {
    fs::create_directories(directory);
    session_started = "Session started at " + get_datetime_stamp_for_logger();
    update_log_file();
}

/**
 * \brief Closes the log file.
 *
 * Flushes and closes the log file if it is open, logging the session end.
 */
void Logger::close_log_file() {
    if (log_stream.is_open()) {
        string session_ended = "Session ended at " + get_datetime_stamp_for_logger();
        log_stream << session_ended << "\n***\n";
        log_stream.flush();
        log_stream.close();
    }
}
