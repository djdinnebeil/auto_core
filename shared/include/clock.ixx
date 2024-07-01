/**
 * \file clock.ixx
 * \brief Offers clock and timing functionalities, including timestamping and timers.
 * \hardlink
 */
module;

#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif

export module clock;
import base;
import config;
import <Windows.h>;

export {
    DLL_API int get_minutes_stamp();
    DLL_API int get_current_seconds();
    DLL_API string get_timestamp();
    DLL_API string get_datestamp();
    DLL_API string get_timestamp_with_seconds();
    DLL_API string get_datetime_stamp_with_seconds();
    DLL_API string get_datetime_stamp_for_logger();
    DLL_API string get_day_of_week();
}
