import base;
import print_b;
import <Windows.h>;

SYSTEMTIME get_local_time();
int get_minutes_stamp();
int get_current_seconds();
string get_timestamp();
string get_datestamp();
string get_timestamp_with_seconds();
string get_datetime_stamp_with_seconds();
string get_datetime_stamp_for_logger();
string set_timestamp_with_seconds(int hour, int min, int sec);
string set_datestamp(int mon, int day, int year);

SYSTEMTIME get_local_time() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    return st;
}
static string format_hour(int hour) {
    oss hour_str;
    if (hour < 7) {
        hour += 24;
    }
    else if (hour < 10) {
        hour_str << '0';
    }
    else if (hour >= 13 && hour < 22) {
        hour -= 12;
    }
    hour_str << hour;
    return hour_str.str();
}
static string format_minutes(int minutes) {
    oss minutes_str;
    if (minutes < 10) {
        minutes_str << '0';
    }
    minutes_str << minutes;
    return minutes_str.str();
}
static string format_seconds(int seconds) {
    oss seconds_str;
    if (seconds < 10) {
        seconds_str << '0';
    }
    seconds_str << seconds;
    return seconds_str.str();
}
string get_timestamp() {
    SYSTEMTIME st = get_local_time();
    oss timestamp;
    timestamp << format_hour(st.wHour) << ':'
        << format_minutes(st.wMinute);
    return timestamp.str();
}
string get_timestamp_with_seconds() {
    SYSTEMTIME st = get_local_time();
    oss timestamp;
    timestamp << format_hour(st.wHour) << ':'
        << format_minutes(st.wMinute) << ':'
        << format_seconds(st.wSecond);
    return timestamp.str();
}
string get_datestamp() {
    SYSTEMTIME st = get_local_time();
    oss datestamp;
    datestamp << st.wMonth << '-'
        << st.wDay << '-'
        << st.wYear % 100;
    return datestamp.str();
}
string get_datetime_stamp_with_seconds() {
    oss datetime_stamp;
    datetime_stamp << get_timestamp_with_seconds() << " on " << get_datestamp();
    return datetime_stamp.str();
}
string get_datetime_stamp_for_logger() {
    oss datetime_stamp;
    datetime_stamp << get_datestamp() << " at " << get_timestamp_with_seconds();
    return datetime_stamp.str();
}
int get_minutes_stamp() {
    SYSTEMTIME st = get_local_time();
    return st.wHour * 60 + st.wMinute;
}
int get_current_seconds() {
    SYSTEMTIME st = get_local_time();
    return st.wSecond;
}
string set_timestamp_with_seconds(int hour, int min, int sec) {
    oss timestamp;
    timestamp << format_hour(hour) << ':'
        << format_minutes(min) << ':'
        << format_seconds(sec);
    return timestamp.str();
}
string set_datestamp(int mon, int day, int year) {
    oss datestamp;
    datestamp << mon + 1 << '-'
        << day << '-'
        << year % 100;
    return datestamp.str();
}
void maintainability() {
    int seconds_to_sleep = 60 - get_current_seconds();
    string timestamp = get_timestamp_with_seconds();
    string current_datestamp = get_datestamp();
}
void performance() {
    SYSTEMTIME st = get_local_time();
    int seconds_to_sleep = 60 - st.wSecond;
    string timestamp = set_timestamp_with_seconds(st.wHour, st.wMinute, st.wSecond);
    string datestamp = set_datestamp(st.wMonth, st.wDay, st.wYear);
}
int clock_speed_micro_maintainability() {
    auto start = chrono::high_resolution_clock::now();
    maintainability();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken by maintainability: " << duration.count() << " microseconds" << endl;
    return static_cast<int>(duration.count());

}
int clock_speed_micro_performance() {
    auto start = chrono::high_resolution_clock::now();
    performance();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken by performance: " << duration.count() << " microseconds" << endl;
    return static_cast<int>(duration.count());
}
int clock_speed_nano_maintainability() {
    auto start = chrono::high_resolution_clock::now();
    maintainability();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
    cout << "Time taken by maintainability: " << duration.count() << " nanoseconds" << endl;
    return static_cast<int>(duration.count());
}
int clock_speed_nano_performance() {
    auto start = chrono::high_resolution_clock::now();
    performance();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
    cout << "Time taken by performance: " << duration.count() << " nanoseconds" << endl;
    return static_cast<int>(duration.count());
}

int main() {
    int maintainable_nano = clock_speed_nano_maintainability();
    int performance_nano = clock_speed_nano_performance();
    int total_gain_nano = 0;
    for (int i = 0; i < 25; i++) {
        maintainable_nano = clock_speed_nano_maintainability();
        performance_nano = clock_speed_nano_performance();
        int round_results = maintainable_nano - performance_nano;
        total_gain_nano += round_results;
        print("A gain of {} nanoseconds", round_results);
    }
    print("Total gain of {} nanoseconds", total_gain_nano);

    int maintainable_micro = clock_speed_micro_maintainability();
    int performance_micro = clock_speed_micro_performance();
    int total_gain_micro = 0;
    for (int i = 0; i < 25; i++) {
        maintainable_micro = clock_speed_micro_maintainability();
        performance_micro = clock_speed_micro_performance();
        int round_results = maintainable_micro - performance_micro;
        total_gain_micro += round_results;
        print("A gain of {} microseconds", round_results);
    }
    print("Total gain of {} microseconds", total_gain_micro);
    return 0;
}