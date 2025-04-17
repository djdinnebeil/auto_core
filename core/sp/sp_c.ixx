/**
\file sp_c.ixx
\brief Provides Spotify component support for the Auto Core application.

This module defines the Spotify class with methods to control Spotify playback,
retrieve song information, and manage authentication tokens. It also includes
functions for handling Spotify-related tasks such as downloading album covers and
switching playback devices.

 */
export module sp_c;
import visual;
import thread;
import sp_x;
//import <nlohmann/json.hpp>;
import <json.hpp>;
import <cpr/cpr.h>;

using json = nlohmann::json;
using namespace cpr;

export HWND spotify_window_hwnd;

export {
    void get_user_sp_queue();
    void sp_song_thread();
    void start_sp_song_thread();
    void print_spotify_songs();
    void spotify_play_pause();
    void spotify_prev_song();
    void sp_switch_player();
    void download_album_cover();
}

/**
 * \brief Parses a JSON string.
 * \param s JSON string to parse.
 * \return Parsed JSON object.
 */
json parse(const string& s) {
    return json::parse(s);
}

struct SongMetadata {
    string name;
    string artist;
    string album;
    int duration_seconds;
};

/**
 * \brief Spotify class to manage Spotify integration.
 *
 * This class handles Spotify playback control, token management, song retrieval, and
 * other related functionalities.
 */
class Spotify {
public:
    Spotify();
    void get_current_song();
    bool download_album_cover();
    void play_pause();
    void next_song();
    void prev_song();
    string get_user_queue();
    int last_status_code;
    vector<string> song_history;
    string last_song;
    string client_id;
    string client_secret;
    string credentials_64;
    string access_token;
    string refresh_token;
    string album_url;
    int start_stamp;
    int timerate;
    bool check_timerate();
    void extract_tokens();
    bool refresh_tokens();
    int pause_song();
    int play_song();
    int music_song_count;
    string tokens_path;
    string devices_path;
    string codes_path;
    int remaining_song_duration_ms;
    bool next_song_clicked;
    bool is_spotify_playing();
    string desktop_device_id;
    string mobile_device_id;
    void switch_player();
    int minutes_stamp;
    bool song_history_contains(string song);
    int song_history_index = 0;
    int string_array_size = 52;
    array<string, 52> song_history_array;
    void get_devices();
    void get_credentials();
    string authorization_header;
    string content_type;
    string content_length;
    bool is_playing;
    void transfer_playback(string device_id);
    void post_next_or_prev(string url);
    bool is_spotify_open();
    void start_playback_on_desktop();
    void start_playback_on_mobile();
    string format_song_title_user_queue(const json& song_details);
    SongMetadata extract_song_metadata(const json& song_details);
    string format_song_title(const SongMetadata& meta);
    string format_artist_name(const json& artists);
    void calculate_remaining_song_duration_ms(const json& song_details);
    void update_devices();
    int sp_position;
    void get_sp_position();
    void activate();
    bool end_thread;
};

export Spotify ac_spotify;
/**
 * \brief Callback function to enumerate Spotify windows.
 * \param hwnd Handle to the window.
 * \param lParam Application-defined parameter.
 * \return TRUE to continue enumeration, FALSE to stop.
 */
BOOL CALLBACK enum_spotify_premium_window(HWND hwnd, LPARAM lParam) {
    const size_t max_length = 15;
    const int length = GetWindowTextLength(hwnd);
    if (length != max_length) {
        return TRUE;
    }
    wstring window_title(length, L'\0');
    if (!GetWindowTextW(hwnd, &window_title[0], length + 1)) {
        return TRUE;
    }
    if (window_title == L"Spotify Premium") {
        *reinterpret_cast<bool*>(lParam) = true;
        sp_logger.logg_and_print("spotify window found");
        spotify_window_hwnd = hwnd;
        return FALSE;
    }
    return TRUE;
}
/**
 * \brief Constructs the Spotify object and initializes necessary data.
 */
Spotify::Spotify() {
    timerate = 55;
    start_stamp = 0;
    get_sp_position();
    tokens_path = R"(.\star\sp_tokens.rc)";
    devices_path = R"(.\star\sp_devices.rc)";
    codes_path = R"(.\star\sp_codes.rc)";
    content_type = "Content-Type: application/json";
    content_length = "Content-Length: 0";
}
/**
 * \brief Retrieves the Spotify taskbar position.
 */
void Spotify::get_sp_position() {
    ifstream taskbar_config_file(R"(.\config\taskbar.ini)");
    string line;
    while (getline(taskbar_config_file, line)) {
        size_t found_spotify = line.find("spotify");
        if (found_spotify != string::npos) {
            sp_position = stoi(line.substr(0, 1));
            break;
        }
    }
}
/**
 * \brief Activates the Spotify window.
 */
void Spotify::activate() {
    send_winkey(sp_position);
}
/**
 * \brief Retrieves Spotify devices.
 */
void Spotify::get_devices() {
    ifstream rc(devices_path);
    getline(rc, desktop_device_id);
    getline(rc, mobile_device_id);
    rc.close();
}
/**
 * \brief Updates Spotify devices.
 */
void Spotify::update_devices() {
    string url = "https://api.spotify.com/v1/me/player/devices";
    auto response = Get(
        Url {url},
        Header {{authorization_header, content_type}}
    );
    bool device_id_changed = false;
    auto devices = parse(response.text);
    for (const auto& device : devices["devices"]) {
        if (device.contains("name") && device.contains("id")) {
            string device_name = device["name"].get<string>();
            string device_id = device["id"].get<string>();

            if (device_name == "DESKTOP-DNP5C1N" && desktop_device_id != device_id) {
                desktop_device_id = device_id;
                device_id_changed = true;
            }
            else if (device_name == "iPhone" && mobile_device_id != device_id) {
                mobile_device_id = device_id;
                device_id_changed = true;
            }
        }
    }
    if (device_id_changed) {
        ofstream rc(devices_path);
        if (rc.is_open()) {
            oss os;
            os << desktop_device_id << "\n" << mobile_device_id;
            rc << os.str();
            rc.close();
        }
    }
}

/**
 * \brief Retrieves Spotify credentials.
 */
void Spotify::get_credentials() {
    ifstream rc(codes_path);
    getline(rc, client_id);
    getline(rc, client_secret);
    string credentials = client_id + ":" + client_secret;
    credentials_64 = base64_encode(credentials);
    rc.close();
}
/**
 * \brief Extracts Spotify tokens.
 */
void Spotify::extract_tokens() {
    get_credentials();
    get_devices();
    ifstream rc(tokens_path);
    getline(rc, access_token);
    getline(rc, refresh_token);
    string time_str;
    getline(rc, time_str);
    start_stamp = stoi(time_str);
    rc.close();
}
/**
 * \brief Checks if the timerate has been reached.
 * \return True if timerate has been reached, false otherwise.
 */
bool Spotify::check_timerate() {
    minutes_stamp = get_minutes_stamp();
    if (minutes_stamp - start_stamp >= timerate) {
        return true;
    }
    else if (minutes_stamp - start_stamp > 0) {
        return false;
    }
    else if ((minutes_stamp + 1440) - start_stamp < timerate) {
        return false;
    }
    else {
        return true;
    }
}
/**
 * \brief Refreshes Spotify tokens.
 * \return True if tokens were refreshed, false otherwise.
 */
bool Spotify::refresh_tokens() {
    if (access_token.empty()) {
        extract_tokens();
    }
    if (!check_timerate()) {
        authorization_header = "Authorization: Bearer " + access_token;
        return true;
    }
    start_stamp = minutes_stamp - 1;
    Response response = Post(Url {"https://accounts.spotify.com/api/token"},
        Header {{"Content-Type", "application/x-www-form-urlencoded"},
                    {"Authorization", "Basic " + credentials_64}},
        Payload {{"grant_type", "refresh_token"},
                     {"refresh_token", refresh_token}});
    if (response.status_code == 200) {
        auto response_json = parse(response.text);
        if (response_json.contains("access_token")) {
            access_token = response_json["access_token"];
            ofstream rc(tokens_path);
            if (rc.is_open()) {
                oss os;
                os << access_token << "\n"
                    << refresh_token << "\n"
                    << to_string(start_stamp);
                rc << os.str();
                rc.close();
            }
        }
        authorization_header = "Authorization: Bearer " + access_token;
        sp_logger.logg_and_logg("refresh_tokens() - tokens refreshed");
        return true;
    }
    else if (response.status_code == 429) {
        sp_logger.logg_and_print("Error: Rate Limit Reached\nStatus Code {} - {}", response.status_code, response.text);
        return false;
    }
    else {
        sp_logger.logg_and_print("Error: \nStatus Code {} - {}", response.status_code, response.text);
        return false;
    }
}

import <sqlite3.h>;


/**
 * \brief Formats the artist name(s).
 * \param artists JSON array of artists.
 * \return Formatted artist name(s) as a string.
 */
string Spotify::format_artist_name(const json& artists) {
    string artist {};
    if (artists.size() == 1) {
        artist = artists[0]["name"];
    }
    else {
        for (size_t i = 0; i < artists.size(); ++i) {
            artist += artists[i]["name"];
            if (i < artists.size() - 1) {
                artist += ", ";
            }
        }
    }
    return artist;
}

string get_datetime_stamp_local() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    char buffer[20];  // "YYYY-MM-DD HH:MM:SS" = 19 characters + null
    sprintf_s(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
        st.wYear, st.wMonth, st.wDay,
        st.wHour, st.wMinute, st.wSecond);
    return std::string(buffer);
}

SongMetadata Spotify::extract_song_metadata(const json& song_details) {
    SongMetadata meta;
    meta.name = song_details["name"];
    meta.artist = format_artist_name(song_details["artists"]);
    meta.album = song_details["album"]["name"];
    meta.duration_seconds = song_details["duration_ms"] / 1000;
    return meta;
}

/*
\todo run a test for speed
track_spotify_history
vs.
track_spotify_history_update_or_insert
*/
void track_spotify_history_update_or_insert(const SongMetadata& meta) {
    sp_logger.logg("track_spotify_history_update_or_insert() called");
    static const std::string db_path = R"(.\star\sp_history.db)";
    const std::string timestamp = get_datetime_stamp_local();

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        sp_logger.logg("Error opening database: {}", sqlite3_errmsg(db));
        return;
    }

    // Step 1: Check if the item exists
    const char* select_sql = R"sql(
        SELECT id, playcount FROM track_history
        WHERE name = ? AND artist = ? AND album = ?
    )sql";

    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) != SQLITE_OK) {
        sp_logger.logg("Error preparing SELECT: {}", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, meta.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, meta.artist.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, meta.album.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    bool exists = (rc == SQLITE_ROW);
    int id = 0;
    int playcount = 0;

    if (exists) {
        id = sqlite3_column_int(stmt, 0);
        playcount = sqlite3_column_int(stmt, 1);
    }

    sqlite3_finalize(stmt); // always finalize before reuse

    if (exists) {
        // Step 2: Update if it exists
        const char* update_sql = R"sql(
            UPDATE track_history
            SET playcount = ?, last_played = ?
            WHERE id = ?
        )sql";

        if (sqlite3_prepare_v2(db, update_sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, playcount + 1);
            sqlite3_bind_text(stmt, 2, timestamp.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, id);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                sp_logger.logg("Error updating track: {}", sqlite3_errmsg(db));
            }
        }
        else {
            sp_logger.logg("Error preparing UPDATE: {}", sqlite3_errmsg(db));
        }
    }
    else {
        // Step 3: Insert new row
        const char* insert_sql = R"sql(
            INSERT INTO track_history
            (name, artist, album, duration, playcount, created_at, last_played)
            VALUES (?, ?, ?, ?, 1, ?, ?)
        )sql";

        if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, meta.name.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, meta.artist.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, meta.album.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 4, meta.duration_seconds);
            sqlite3_bind_text(stmt, 5, timestamp.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 6, timestamp.c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                sp_logger.logg("Error inserting new track: {}", sqlite3_errmsg(db));
            }
        }
        else {
            sp_logger.logg("Error preparing INSERT: {}", sqlite3_errmsg(db));
        }
    }

    if (stmt) sqlite3_finalize(stmt);
    sqlite3_close(db);
    sp_logger.logg("track_spotify_history_update_or_insert() finished");
}

/*
\todo run a test for speed
track_spotify_history
vs.
track_spotify_history_update_or_insert
*/
void track_spotify_history(const SongMetadata& meta) {
    sp_logger.logg("track_spotify_history() called");
    static const string db_path = R"(.\star\sp_history.db)";

    const string timestamp = get_datetime_stamp_local();

    // Open database
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        sp_logger.logg("Error opening database: {}", sqlite3_errmsg(db));
        return;
    }

    const char* upsert_sql = R"sql(
        INSERT INTO track_history (name, artist, album, duration, playcount, created_at, last_played)
        VALUES (?, ?, ?, ?, 1, ?, ?)
        ON CONFLICT(name, artist, album) DO UPDATE SET
            playcount = track_history.playcount + 1,
            last_played = excluded.last_played;
    )sql";

    if (sqlite3_prepare_v2(db, upsert_sql, -1, &stmt, nullptr) != SQLITE_OK) {
        sp_logger.logg("Error preparing UPSERT: {}", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Bind values (only 5, since playcount is hardcoded to 1)
    sqlite3_bind_text(stmt, 1, meta.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, meta.artist.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, meta.album.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, meta.duration_seconds);
    sqlite3_bind_text(stmt, 5, timestamp.c_str(), -1, SQLITE_STATIC); // created_at
    sqlite3_bind_text(stmt, 6, timestamp.c_str(), -1, SQLITE_STATIC); // last_played

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sp_logger.logg("Error executing UPSERT: {}\n", sqlite3_errmsg(db));
    }

    if (stmt) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    sp_logger.logg("track_spotify_history() finished");
}

/**
 * \brief Retrieves the current Spotify song.
 */
void Spotify::get_current_song() {
    sp_logger.logg("get_current_song()");
    if (!refresh_tokens()) {
        return;
    }
    string url = "https://api.spotify.com/v1/me/player/currently-playing";
    auto response = Get(
        Url {url},
        Header {{authorization_header, content_type}}
    );
    last_status_code = 1;
    if (response.status_code != 200) {
        if (response.status_code == 204) {
            sp_logger.logg("{} status code", response.status_code);
            is_playing = false;
        }
        else {
            sp_logger.logg_and_print("{} status code", response.status_code);
        }
        return;
    }
    auto song_details = parse(response.text);
    is_playing = song_details["is_playing"];
    if (!song_details["item"].contains("name")) {
        sp_logger.logg_and_logg("djai++ is talking");
        last_status_code = 15;
        return;
    }

    SongMetadata meta = extract_song_metadata(song_details["item"]);
    string current_song = format_song_title(meta);
    if (current_song == last_song) {
        return;
    }
    calculate_remaining_song_duration_ms(song_details);
    sp_logger.loggnl_and_loggnl("now playing: ");
    sp_logger.logg_and_print(current_song);
    last_song = current_song;
    song_history.push_back(current_song);
    track_spotify_history(meta);
    return;
}
/**
 * \brief Calculates the remaining song duration in milliseconds.
 * \param song_details JSON object containing song details.
 */
void Spotify::calculate_remaining_song_duration_ms(const json& song_details) {
    int progress_ms = song_details["progress_ms"];
    int duration_ms = song_details["item"]["duration_ms"];
    remaining_song_duration_ms = duration_ms - progress_ms;
}

/**
 * \brief Formats the song title.
 * \param song_details JSON object containing song details.
 * \return Formatted song title as a string.
 */
string Spotify::format_song_title(const SongMetadata& meta) {
    oss output;
    oss dur;
    dur << meta.duration_seconds / 60 << ":" << setw(2) << setfill('0') << meta.duration_seconds % 60;
    output << '[' << meta.name << "] [" << meta.artist << "] [" << meta.album << "] [" << dur.str() << ']';
    return output.str();
}

string Spotify::format_song_title_user_queue(const json& song_details) {
    const string name = song_details["name"];
    const string artist = format_artist_name(song_details["artists"]);
    const string album = song_details["album"]["name"];
    int duration = song_details["duration_ms"] / 1000;
    oss output;
    oss dur;
    dur << duration / 60 << ":" << setw(2) << setfill('0') << duration % 60;
    output << '[' << name << "] [" << artist << "] [" << album << "] [" << dur.str() << ']';
    return output.str();
}

/**
 * \brief Retrieves the user's Spotify queue.
 * \return Formatted user queue as a string.
 */
string Spotify::get_user_queue() {
    try {
        if (!refresh_tokens()) {
            return "Unable to refresh tokens";
        }
        string url = "https://api.spotify.com/v1/me/player/queue";
        auto response = Get(
            Url {url},
            Header {{authorization_header, content_type}}
        );
        if (response.status_code != 200) {
            sp_logger.logg_and_print("Failed to retrieve queue");
            return "";
        }
        json queue_details = parse(response.text);
        oss output;
        string current_song;
        if (!queue_details["currently_playing"].is_null()) {
            current_song = format_song_title_user_queue(queue_details["currently_playing"]);
            song_history_contains(current_song);
        }
        for (const auto& item : queue_details["queue"]) {
            string song = format_song_title_user_queue(item);
            if (!song_history_contains(song)) {
                output << song << '\n';
            }
        }
        output << current_song;
        return output.str();
    }
    catch (...) {
        sp_logger.logg_and_print("An exception occurred in get_user_queue");
        return "";
    }
}
/**
 * \brief Downloads the current song's album cover.
 * \return True if the album cover was downloaded successfully, false otherwise.
 */
bool Spotify::download_album_cover() {
    if (!refresh_tokens()) {
        return false;
    }
    string url = "https://api.spotify.com/v1/me/player/currently-playing";
    auto response = Get(
        Url {url},
        Header {{authorization_header, content_type}}
    );
    if (response.status_code != 200) {
        sp_logger.logg_and_print("Failed to retrieve song");
        return "";
    }
    auto song_details = parse(response.text);
    string album_cover_url;
    if (!song_details["item"]["album"]["images"].empty()) {
        album_cover_url = song_details["item"]["album"]["images"][0]["url"].get<string>();
    }
    else {
        return false;
    }
    auto album_response = Get(Url {album_cover_url});
    string filepath = "cover.jpg";
    if (album_response.status_code == 200) {
        ofstream file(filepath, ios::binary);
        file.write(album_response.text.c_str(), album_response.text.size());
        file.close();
        return true;
    }
    else {
        return false;
    }
}
/**
 * \brief Pauses the current Spotify song.
 * \return Status code of the pause request.
 */
int Spotify::pause_song() {
    if (!refresh_tokens()) {
        return 429;
    }
    auto response = Put(
        Url {"https://api.spotify.com/v1/me/player/pause"},
        Header {
            {"Authorization", "Bearer " + access_token},
            {"Content-Type", "application/json"},
            {"Content-Length", "0"}
        });
    if (response.status_code != 204) {
        sp_logger.logg_and_print("Error: Status Code {} - {}", response.status_code, response.text);
    }
    return response.status_code;
}
/**
 * \brief Transfers playback to the specified device.
 * \param device_id The device ID to transfer playback to.
 */
void Spotify::transfer_playback(string device_id) {
    if (!refresh_tokens()) {
        return;
    }
    string url = "https://api.spotify.com/v1/me/player";
    string body = format(R"({{"device_ids": ["{}"], "play": true}})", device_id);
    auto response = Put(
        Url {url},
        Header {{authorization_header, content_type}},
        Body {body}
    );
    if (response.status_code != 204) {
        sp_logger.logg_and_print("Error: Status Code {} - {}", response.status_code, response.text);
    }
}
/**
 * \brief Starts playback on the desktop device.
 */
void Spotify::start_playback_on_desktop() {
    transfer_playback(desktop_device_id);
}
/**
 * \brief Starts playback on the mobile device.
 */
void Spotify::start_playback_on_mobile() {
    transfer_playback(mobile_device_id);
}
/**
 * \brief Switches playback between desktop and mobile devices.
 */
void Spotify::switch_player() {
    if (!refresh_tokens()) {
        return;
    }
    update_devices();
    string url = "https://api.spotify.com/v1/me/player";
    auto response = Get(
        Url {url},
        Header {{authorization_header, content_type}}
    );
    if (response.status_code == 204) {
        play_pause();
        return;
    }
    if (response.status_code != 200) {
        sp_logger.logg_and_print("Error: Status Code {} - {}", response.status_code, response.text);
        return;
    }
    auto playback_details = parse(response.text);
    string current_device_id = playback_details["device"]["id"];
    if (current_device_id == desktop_device_id) {
        start_playback_on_mobile();
    }
    else if (current_device_id == mobile_device_id) {
        start_playback_on_desktop();
    }
    else {
        sp_logger.logg_and_print("Device not added");
    }
}
/**
 * \brief Plays the current Spotify song.
 * \return Status code of the play request.
 */
int Spotify::play_song() {
    if (!refresh_tokens()) {
        return 429;
    }
    auto response = Put(
        Url {"https://api.spotify.com/v1/me/player/play"},
        Header {
            {"Authorization", "Bearer " + access_token},
            {"Content-Type", "application/json"},
            {"Content-Length", "0"},
        }
    );
    if (response.status_code == 404) {
        sp_logger.logg_and_print("no active device");
    }
    else if (response.status_code != 204) {
        sp_logger.logg_and_print("Error: Status Code {} - {}", response.status_code, response.text);
    }
    return response.status_code;
}
/**
 * \brief Checks if the Spotify application is open.
 * \return True if Spotify is open, false otherwise.
 */
bool Spotify::is_spotify_open() {
    bool spotifyWindowFound = false;
    EnumWindows(enum_spotify_premium_window, reinterpret_cast<LPARAM>(&spotifyWindowFound));
    return spotifyWindowFound;
}
/**
 * \brief Checks if Spotify is currently playing a song.
 * \return True if a song is playing, false otherwise.
 */
bool Spotify::is_spotify_playing() {
    if (!refresh_tokens()) {
        return false;
    }
    string url = "https://api.spotify.com/v1/me/player/currently-playing";
    auto response = Get(
        Url {url},
        Header {{authorization_header, content_type}}
    );
    if (response.status_code != 200) {
        return false;
    }
    auto playback_details = parse(response.text);
    if (!playback_details.contains("is_playing")) {
        return false;
    }
    bool is_playing = playback_details["is_playing"];
    return is_playing;
}
/**
 * \brief Starts the Spotify playback thread.
 * \todo Develop a more responsive display the current song when starting Spotify.
 */
void start_playback_sp_thread() {
    ac_spotify.activate();
    int total_sleep_time {};
    int sleep_duration = 500;
    int time_limit_ms = 10000;
    int processing_delay = 1500;
    while (!ac_spotify.is_spotify_open() && total_sleep_time < time_limit_ms) {
        this_thread::sleep_for(chrono::milliseconds(sleep_duration));
        total_sleep_time += sleep_duration;
    }
    this_thread::sleep_for(chrono::milliseconds(processing_delay));
    ac_spotify.start_playback_on_desktop();
    Sleep(1500);
    ac_spotify.get_current_song();
}
/**
 * \brief Plays or pauses the Spotify playback.
 */
void Spotify::play_pause() {
    if (!refresh_tokens()) {
        sp_logger.logg_and_print("tokens not refreshed in Spotify::play_pause()");
        return;
    }
    if (is_spotify_playing()) {
        sp_logger.logg_and_logg("is_spotify_playing() == true");
        pause_song();
        return;
    }
    if (play_song() == 204) {
        sp_logger.logg_and_logg("play_song() == 204");
        return;
    }
    if (is_spotify_open()) {
        sp_logger.logg_and_logg("is_spotify_open() == true");
        start_playback_on_desktop();
    }
    else {
        sp_logger.logg_and_logg("starting Spotify");
        thread t(start_playback_sp_thread);
        t.detach();
    }
}
/**
 * \brief Sends a POST request to skip to the next or previous song.
 * \param url URL for the POST request.
 */
void Spotify::post_next_or_prev(string url) {
    if (!refresh_tokens()) {
        return;
    }
    auto response = Post(
        Url {url},
        Header {{authorization_header, content_type}}
    );
    if (response.status_code != 204) {
        sp_logger.logg_and_print("post_next_or_prev() - Error: Status Code {} - {}", response.status_code, response.text);
    }
}
/**
 * \brief Skips to the next Spotify song.
 */
void Spotify::next_song() {
    string url = "https://api.spotify.com/v1/me/player/next";
    post_next_or_prev(url);
}
/**
 * \brief Skips to the previous Spotify song.
 */
void Spotify::prev_song() {
    string url = "https://api.spotify.com/v1/me/player/previous";
    post_next_or_prev(url);
}
/**
 * \brief Checks if the song history contains the specified song.
 * \param current_song Song to check.
 * \return True if the song is in the history, false otherwise.
 */
bool Spotify::song_history_contains(string current_song) {
    for (const auto& song : song_history_array) {
        if (song == current_song) {
            return true;
        }
    }
    song_history_array[song_history_index] = current_song;
    song_history_index += 1;
    if (song_history_index == string_array_size) {
        song_history_index = 0;
    }
    return false;
}
bool get_user_queue_thread_started = false; /// \todo refactor the code

/**
 * \brief Retrieves the user's Spotify queue in a separate thread.
 */
void get_user_sp_queue_thread() {
    string user_queue = ac_spotify.get_user_queue();
    sp_logger.logg_and_print(user_queue);
    set_clipboard_text(str_to_wstr(user_queue) + L"\n\n");
    Sleep(50);
    paste_from_clipboard();
    get_user_queue_thread_started = false;
}

/**
* \brief Retrieves the user's Spotify queue.
* \runtime
*/
void get_user_sp_queue() {
    sp_logger.logg_and_logg("get_user_sp_queue()");
    if (!get_user_queue_thread_started) {
        get_user_queue_thread_started = true;
        thread t([=]() {run_with_exception_handling(get_user_sp_queue_thread); });
        t.detach();
    }
}

/**
* \brief Prints the Spotify songs to the console.
* \runtime
*/
void print_spotify_songs() {
    sp_logger.logg_and_logg("print_spotify_songs()");
    oss song_text;
    ac_spotify.get_current_song();
    if (!ac_spotify.song_history.empty()) {
        for (const auto& song : ac_spotify.song_history) {
            if (song != ac_spotify.last_song) {
                song_text << song << '\n';
            }
        }
        ac_spotify.song_history.clear();
    }
    song_text << ac_spotify.last_song << '\n';
    string song_text_str = song_text.str();
    sp_logger.loggnl_and_printnl(song_text_str);
    set_clipboard_text(str_to_wstr(song_text_str) + L"\n");
    Sleep(50);
    paste_from_clipboard();
}
/**
* \brief Toggles Spotify play/pause in a separate thread.
*/
void spotify_play_pause_thread() {
    ac_spotify.play_pause();
}
/**
* \brief Toggles Spotify play/pause.
*
* \runtime
*/
void spotify_play_pause() {
    sp_logger.logg_and_logg("spotify_play_pause()");
    thread t(spotify_play_pause_thread);
    t.detach();
}
/**
 * \brief Skips to the previous Spotify song.
 */
void spotify_prev_song() {
    ac_spotify.prev_song();
}

/**
* \brief Switches Spotify playback device.
* \runtime
*/
void sp_switch_player() {
    sp_logger.logg_and_logg("sp_switch_player()");
    ac_spotify.switch_player();
}
/**
 * \brief Downloads the album cover of the current song.
 */
void download_album_cover() {
    ac_spotify.download_album_cover();
}