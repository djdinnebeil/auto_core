module itunes;
import visual;
import pipes;
import journey;
import <Windows.h>;

/**
 * \brief Starts the iTunes component.
 *
 * This function starts the iTunes component by creating a new process for the iTunes executable.
 */
void start_iTunes_component() {
    wstring itunes_path = LR"(.\bin\ac_itunes.exe)";
    create_process(itunes_path);
}

/**
 * \brief Sends a command to print iTunes songs.
 *
 * This function sends a command to the iTunes pipe to print the list of iTunes songs.
 *
 * \runtime
 */
void print_iTunes_songs() {
    send_pipe_command(ac_itunes_pipe, 3);
}

/**
 * \brief Sends a command to skip to the next iTunes song.
 *
 * This function sends a command to the iTunes pipe to play the next song in iTunes.
 *
 * \runtime
 */
void iTunes_next_song() {
    send_pipe_command(ac_itunes_pipe, 2);
}

/**
 * \brief Sends a command to print the next up song list.
 *
 * This function sends a command to the iTunes pipe to print the list of upcoming songs.
 *
 * \runtime
 */
void print_next_up_song_list() {
    send_pipe_command(ac_itunes_pipe, 4);
}

/**
 * \brief Sends a command to play/pause iTunes.
 *
 * This function sends a command to the iTunes pipe to play or pause the current iTunes song.
 *
 * \runtime
 */
void iTunes_play_pause() {
    send_pipe_command(ac_itunes_pipe, 1);
}

/**
 * \brief Sends a command to play the previous iTunes song.
 *
 * This function sends a command to the iTunes pipe to play the previous song in iTunes.
 */
void iTunes_prev_song() {
    send_pipe_command(ac_itunes_pipe, 6);
}

/**
 * \brief Sends a command to update the iTunes logger.
 *
 * This function sends a command to the iTunes pipe to update the iTunes logger.
 */
void update_iTunes_logger() {
    send_pipe_command(ac_itunes_pipe, 5);
}

/**
 * \brief Sends a command to stop the iTunes component.
 *
 * This function sends a command to the iTunes pipe to end the iTunes component.
 */
void send_iTunes_end_signal() {
    send_pipe_command(ac_itunes_pipe, 0);
}

/**
 * \brief Sends a command to remove a song from iTunes.
 *
 * This function sends a command to the iTunes pipe to remove a song from iTunes.
 *
 * \runtime
 */
void remove_iTunes_song() {
    send_pipe_command(ac_itunes_pipe, 9);
}
