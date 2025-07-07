/**
\file itunes_t.ixx
\brief This module provides threading support for iTunes integration.

This module manages the iTunes component's threading, including handling song transitions and monitoring playback state changes.

 */
export module itunes_t;
import visual;
import itunes_c;
import itunes_x;
import <Windows.h>;

export void iTunes_next_song();

/**
 * \brief Logs and handles the transition to the next iTunes song.
 *
 * This function logs the transition to the next song, changes the playback state, and notifies the condition variable.
 *
 * \runtime
 */
void iTunes_next_song() {
    iTunes_logger.logg_and_logg("iTunes_next_song()");
    ac_iTunes.next_song();
    iT_playback_state_change = true;
    iT_cv.notify_one();
}
/**
 * \brief Starts the iTunes monitoring thread.
 *
 * This function runs in a separate thread to monitor iTunes playback state, updating song information and handling state changes.
 */
void iTunes::start_iTunes_thread() {
    Sleep(25);
    iTunes_logger.logg_and_logg("iTunes_thread started");
    const int sleep_timerate_secs_playing = 5;
    const int sleep_timerate_secs_pause = 5;
    const int extra_time_ms = 100;
    const int processing_delay_ms = 252;
    Sleep(350);
    try {
        unique_lock<mutex> lock(iT_mtx);
        int sleep_time_secs;
        while (true) {
            iT_playback_state_change = false;
            if (ac_iTunes.initialized) {
                ac_iTunes.get_current_track();
            }
            if (ac_iTunes.is_playing()) {
                if (ac_iTunes.remaining_song_duration > sleep_timerate_secs_playing) {
                    sleep_time_secs = sleep_timerate_secs_playing;
                }
                else if (ac_iTunes.remaining_song_duration < sleep_timerate_secs_playing) {
                    sleep_time_secs = ac_iTunes.remaining_song_duration;
                    Sleep(extra_time_ms);
                }
            }
            else {
                sleep_time_secs = sleep_timerate_secs_pause;
            }
            iTunes_logger.logg("iTunes sleep time {} seconds at {}", sleep_time_secs, get_timestamp_with_seconds());
            if (iT_cv.wait_for(lock, chrono::seconds(sleep_time_secs), [] {return iT_playback_state_change; })) {
                if (ac_iTunes.end_thread) {
                    break;
                }
                iTunes_logger.logg("iTunes_playback_state_change at {}", get_timestamp_with_seconds());
                Sleep(processing_delay_ms);
            }
        }
    }
    catch (const exception& e) {
        iTunes_logger.logg_and_print("iTunes_song_thread() has crashed: {}", e.what());
    }
    catch (...) {
        iTunes_logger.logg_and_print("iTunes_song_thread() has crashed due to an unknown exception");
    }
    iTunes_logger.logg("end of iTunes thread");
}