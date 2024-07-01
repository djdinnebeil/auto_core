module star;
import visual;
import cloud;
import <sqlite3.h>;
import <Windows.h>;

/**
 * \brief Simulates a save file operation.
 *
 * This function sends keyboard inputs to simulate the Ctrl+S key combination to save a file.
 */
void save_file() {
    INPUT inputs[4] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'S';
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'S';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

void create_new_file() {
    INPUT inputs[4] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'N';
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'N';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

wstring get_window_title() {
    HWND current_window_hwnd = GetForegroundWindow();
    int length = GetWindowTextLength(current_window_hwnd);
    if (length == 0) {
        return wstring();
    }

    // Allocate buffer of appropriate size
    wstring title(length, '\0');
    GetWindowTextW(current_window_hwnd, &title[0], length + 1); // +1 to include null terminator
    return title;
}

/**
 * \runtime
 */
void save_file_and_create_new_file() {
    wstring current_window_title = get_window_title();
    save_file();
    Sleep(50);
    create_new_file();
    Sleep(300);
    wstring new_file_window_title = get_window_title();
    if (new_file_window_title != L"" && (new_file_window_title != current_window_title)) {
        print_episode_title();
    }
}

/**
 * \brief Retrieves the star name from the configuration file.
 *
 * This function reads the star name from the star.ini configuration file.
 *
 * \return The star name as a string.
 */
string get_star_name() {
    logg("get_star_name()");
    ifstream star_rc(R"(.\config\star.ini)");
    string line;
    getline(star_rc, line);
    logg(line);
    return line;
}

/**
 * \brief Retrieves the database path from the configuration file.
 *
 * This function reads the database path from the star.ini configuration file.
 *
 * \return The database path as a string.
 */
string get_database_path() {
    logg("get_database_path()");
    ifstream star_rc(R"(.\config\star.ini)");
    string line;
    getline(star_rc, line);  // Skip the star name line
    getline(star_rc, line);  // Read the database path line
    logg(line);
    return line;
}

/**
 * \brief Retrieves and updates the episode number from the database.
 *
 * This function retrieves the current episode number from the SQLite database, increments it,
 * and updates the database with the new episode number.
 *
 * \return The current episode number, or -1 if an error occurs.
 */
int get_episode_number() {
    static string db_path = get_database_path();
    sqlite3* db;
    sqlite3_stmt* selectStmt;
    sqlite3_stmt* updateStmt;
    int rc = sqlite3_open(db_path.c_str(), &db);
    if (rc != SQLITE_OK) {
        return -1;
    }
    const char* selectSQL = "SELECT value FROM counter LIMIT 1;";
    rc = sqlite3_prepare_v2(db, selectSQL, -1, &selectStmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(selectStmt);
        sqlite3_close(db);
        return -1;
    }
    int episode_number;
    if (sqlite3_step(selectStmt) == SQLITE_ROW) {
        episode_number = sqlite3_column_int(selectStmt, 0);
    }
    else {
        episode_number = -1;
    }
    sqlite3_finalize(selectStmt);
    if (episode_number != -1) {
        const char* updateSQL = "UPDATE counter SET value = ?;";
        rc = sqlite3_prepare_v2(db, updateSQL, -1, &updateStmt, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(updateStmt);
            sqlite3_close(db);
            return episode_number;
        }
        sqlite3_bind_int(updateStmt, 1, episode_number + 1);
        sqlite3_step(updateStmt);
        sqlite3_finalize(updateStmt);
    }
    sqlite3_close(db);
    return episode_number;
}

/**
 * \brief Generates the episode title.
 *
 * This function generates the episode title using the star name and episode number,
 * updates the title in the cloud, and returns the formatted title string.
 *
 * \return The formatted episode title as a string.
 */
string get_episode_title() {
    static string star_name = get_star_name();
    string star_and_number = format("{} {}", star_name, get_episode_number());
    update_string_in_firebase(star_and_number);
    oss s;
    s << star_and_number << '\n' << get_datestamp() << "\n\n" << get_timestamp();
    return s.str();
}

/**
 * \brief Prints the episode title.
 *
 * This function prints the generated episode title to the screen and saves the file.
 *
 * \runtime
 */
void print_episode_title() {
    wstring most_recent_clipboard_text = get_clipboard_text();
    wstring episode_title = str_to_wstr(get_episode_title());
    print(episode_title);
    set_clipboard_text(episode_title + L"\n\n");
    Sleep(50);
    paste_from_clipboard();
    Sleep(100);
    save_file();
    Sleep(100);
    set_clipboard_text(most_recent_clipboard_text);
}
