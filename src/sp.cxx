module sp;
import base;
import pipes;
import journey;
import <Windows.h>;

/**
 * \brief Starts the Spotify component executable.
 *
 * This function launches the Spotify component executable, `sp_ac.exe`, which is responsible for handling Spotify-related tasks.
 */
void start_sp_component() {
    wstring sp_path = LR"(.\bin\sp_ac.exe)";
    create_process(sp_path);
}

/**
 * \brief Retrieves and displays the user's Spotify queue.
 *
 * This function sends a command to the Spotify component to fetch and display the user's current Spotify queue.
 * \runtime
 */
void get_user_sp_queue() {
    send_pipe_command(ac_sp_pipe, 4);
}

/**
 * \brief Retrieves and displays the currently playing Spotify songs.
 *
 * This function sends a command to the Spotify component to fetch and display the currently playing Spotify songs.
 * \runtime
 */
void print_spotify_songs() {
    send_pipe_command(ac_sp_pipe, 3);
}

/**
 * \brief Toggles play/pause on the Spotify player.
 *
 * This function sends a command to the Spotify component to toggle the play/pause state of the Spotify player.
 * \runtime
 */
void spotify_play_pause() {
    send_pipe_command(ac_sp_pipe, 1);
}

/**
 * \brief Skips to the next song in the Spotify playlist.
 *
 * This function sends a command to the Spotify component to skip to the next song in the Spotify playlist.
 * \runtime
 */
void spotify_next_song() {
    send_pipe_command(ac_sp_pipe, 2);
}

/**
 * \brief Switches the Spotify playback device.
 *
 * This function sends a command to the Spotify component to switch the playback device (e.g., between desktop and mobile).
 * \runtime
 */
void sp_switch_player() {
    send_pipe_command(ac_sp_pipe, 6);
}

/**
 * \brief Downloads the album cover of the currently playing song.
 *
 * This function sends a command to the Spotify component to download the album cover of the currently playing song.
 */
void download_album_cover() {
    send_pipe_command(ac_sp_pipe, 8);
}

/**
 * \brief Sends a signal to terminate the Spotify component.
 *
 * This function sends a command to the Spotify component to gracefully shut down and terminate the process.
 */
void send_sp_end_signal() {
    send_pipe_command(ac_sp_pipe, 0);
}

/**
 * \brief Updates the Spotify logger.
 *
 * This function sends a command to the Spotify component to update the log file with the latest information.
 */
void update_sp_logger() {
    send_pipe_command(ac_sp_pipe, 5);
}
