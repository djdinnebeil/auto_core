module clock;
import base;
import config;
import <Windows.h>;

/**
 * \brief Retrieves the local system time.
 * \return The current local system time as SYSTEMTIME.
 */
SYSTEMTIME get_local_time() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    return st;
}

/**
 * \brief Formats the hour value based on the end of day and converts it to a string.
 * \param hour The hour value to format.
 * \return The formatted hour as a string.
 */
static string format_hour(int hour) {
    oss hour_str;
    if (hour < config.end_of_day) {
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

/**
 * \brief Formats the minutes value and converts it to a string.
 * \param minutes The minutes value to format.
 * \return The formatted minutes as a string.
 */
static string format_minutes(int minutes) {
    oss minutes_str;
    if (minutes < 10) {
        minutes_str << '0';
    }
    minutes_str << minutes;
    return minutes_str.str();
}

/**
 * \brief Formats the seconds value and converts it to a string.
 * \param seconds The seconds value to format.
 * \return The formatted seconds as a string.
 */
static string format_seconds(int seconds) {
    oss seconds_str;
    if (seconds < 10) {
        seconds_str << '0';
    }
    seconds_str << seconds;
    return seconds_str.str();
}

/**
 * \brief Retrieves the current timestamp in "HH:MM" format.
 * \return The current timestamp as a string.
 */
string get_timestamp() {
    SYSTEMTIME st = get_local_time();
    oss timestamp;
    timestamp << format_hour(st.wHour) << ':'
        << format_minutes(st.wMinute);
    return timestamp.str();
}

/**
 * \brief Retrieves the current timestamp with seconds in "HH:MM:SS" format.
 * \return The current timestamp with seconds as a string.
 */
string get_timestamp_with_seconds() {
    SYSTEMTIME st = get_local_time();
    oss timestamp;
    timestamp << format_hour(st.wHour) << ':'
        << format_minutes(st.wMinute) << ':'
        << format_seconds(st.wSecond);
    return timestamp.str();
}

/**
 * \brief Retrieves the current datestamp in "MM-DD-YY" format.
 * \return The current datestamp as a string.
 */
string get_datestamp() {
    SYSTEMTIME st = get_local_time();
    oss datestamp;
    datestamp << st.wMonth << '-'
        << st.wDay << '-'
        << st.wYear % 100;
    return datestamp.str();
}

/**
 * \brief Retrieves the current datetime stamp with seconds in "HH:MM:SS on MM-DD-YY" format.
 * \return The current datetime stamp with seconds as a string.
 */
string get_datetime_stamp_with_seconds() {
    oss datetime_stamp;
    datetime_stamp << get_timestamp_with_seconds() << " on " << get_datestamp();
    return datetime_stamp.str();
}

/**
 * \brief Retrieves the current datetime stamp for logging purposes in "MM-DD-YY at HH:MM:SS" format.
 * \return The current datetime stamp for logger as a string.
 */
string get_datetime_stamp_for_logger() {
    oss datetime_stamp;
    datetime_stamp << get_datestamp() << " at " << get_timestamp_with_seconds();
    return datetime_stamp.str();
}

/**
 * \brief Retrieves the current time in minutes since midnight.
 * \return The current time in minutes as an integer.
 */
int get_minutes_stamp() {
    SYSTEMTIME st = get_local_time();
    return st.wHour * 60 + st.wMinute;
}

/**
 * \brief Retrieves the current seconds value.
 * \return The current seconds as an integer.
 */
int get_current_seconds() {
    SYSTEMTIME st = get_local_time();
    return st.wSecond;
}

/**
 * \brief Retrieves the current day of the week.
 * \return The current day of the week as a string.
 */
string get_day_of_week() {
    SYSTEMTIME st = get_local_time();
    GetLocalTime(&st);
    const char* days_of_week[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    return days_of_week[st.wDayOfWeek];
}
