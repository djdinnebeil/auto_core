/**
\file sp.ixx
\brief Integrates Spotify functionality within Auto Core, providing music player automation and control.

This module facilitates the integration of Spotify features into the Auto Core application. It provides functions to control Spotify playback, retrieve and display the user's Spotify queue, and manage various playback settings. The module also handles starting the Spotify component, sending commands to it via named pipes, and updating the logger.

The main functions include:
- Starting the Spotify component executable.
- Sending commands to control Spotify playback (play/pause, next song, previous song).
- Retrieving and displaying the user's Spotify queue and currently playing songs.
- Switching playback between devices and downloading album covers.
- Sending an end signal to gracefully terminate the Spotify component.
*/
export module sp;
import base;
import pipes;
import journey;
import <Windows.h>;

export HANDLE ac_sp_pipe;

export {
    void start_sp_component();
    void get_user_sp_queue();
    void print_spotify_songs();
    void spotify_play_pause();
    void spotify_prev_song();
    void sp_switch_player();
    void download_album_cover();
    void spotify_next_song();
    void send_sp_end_signal();
    void update_sp_logger();
}
