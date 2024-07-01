import base;
import print_b;
import <cpr/cpr.h>;
import <nlohmann/json.hpp>;
import <CivetServer.h>;

string client_id = "95ad6c6b821345dfac3d997e68d7c4f4";
string client_secret = "55c15214a79a4d47b2e563263675d07e";
string redirect_uri = "http://localhost:8080/callback";
//string client_id = "47c664dcf1304d488fa798923057892e";
//string client_secret = "379a0b38ac944ec5821302fe3259007c";
//string redirect_uri = "http://localhost:8585/callback";
string account_url = "https://accounts.spotify.com";
string scope = "user-follow-read%20ugc-image-upload%20user-read-playback-state%20user-modify-playback-state%20user-read-currently-playing%20user-read-private%20user-read-email%20user-follow-modify%20user-follow-read%20user-library-modify%20user-library-read%20streaming%20app-remote-control%20user-read-playback-position%20user-top-read%20user-read-recently-played%20playlist-modify-private%20playlist-read-collaborative%20playlist-read-private%20playlist-modify-public";
string authorization_link;
string authorization_code;
string access_token;
string refresh_token;

using namespace cpr;

// Atomic flag to signal the server thread to terminate
std::atomic<bool> terminateServer(false);

// Encode to Base64
string base64_encode(const string& in) {
    string out;
    int val = 0, valb = -6;
    for (uint8_t c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

void saveTokens() {
    ofstream outFile("tokens.rc");
    if (outFile.is_open()) {
        outFile << access_token + "\n" << refresh_token;
        outFile.close();
        cout << "Tokens saved to tokens.rc\n";
    }
    else {
        cerr << "Unable to open file for writing: tokens.rc\n";
    }
}

// Callback handler for handling Spotify authorization
class SpotifyCallbackHandler : public CivetHandler {
public:
    bool handleGet(CivetServer* server, mg_connection* conn) {
        const mg_request_info* req_info = mg_get_request_info(conn);
        string query = req_info->query_string ? req_info->query_string : "";
        size_t code_pos = query.find("code=");
        if (code_pos != string::npos) {
            authorization_code = query.substr(code_pos + 5);
            cout << "Authorization code received: " << authorization_code << "\n";

            string credentials = client_id + ":" + client_secret;
            string encoded_credentials = base64_encode(credentials);
            cpr::Response r = cpr::Post(cpr::Url{"https://accounts.spotify.com/api/token"},
                cpr::Header{{"Content-Type", "application/x-www-form-urlencoded"},
                            {"Authorization", "Basic " + encoded_credentials}},
                cpr::Payload{{"grant_type", "authorization_code"},
                             {"code", authorization_code},
                             {"redirect_uri", redirect_uri}});
            if (r.status_code == 200) {
                auto response_json = nlohmann::json::parse(r.text);
                access_token = response_json["access_token"];
                refresh_token = response_json["refresh_token"];
                saveTokens();
                retrieveCurrentSong();
            }
            else {
                cout << "Failed to retrieve tokens. Status Code: " << r.status_code << "\nResponse: " << r.text << "\n";
            }

            terminateServer.store(true);
            mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nAuthorization successful. You can close this page.");
            return true;
        }
        return false;
    }

    void retrieveCurrentSong() {
        cpr::Response r = cpr::Get(cpr::Url{"https://api.spotify.com/v1/me/player/currently-playing"},
            cpr::Header{{"Authorization", "Bearer " + access_token}});
        if (r.status_code == 200) {
            auto response_json = nlohmann::json::parse(r.text);
            cout << "Song Name: " << response_json["item"]["name"] << "\n";
            cout << "Artist: " << response_json["item"]["artists"][0]["name"] << "\n";
            cout << "Album: " << response_json["item"]["album"]["name"] << "\n";
        }
        else {
            cout << "Failed to retrieve current song. Status Code: " << r.status_code << "\nResponse: " << r.text << "\n";
        }
    }
};

void runServer() {
    const char* options[] ={"listening_ports", "8080", "document_root", ".", nullptr};
    CivetServer server(options);
    SpotifyCallbackHandler spotifyCallbackHandler;
    server.addHandler("/callback", spotifyCallbackHandler);
    while (!terminateServer.load()) {
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void set_authorization_link() {
    authorization_link = account_url + "/authorize?client_id=" + client_id + "&response_type=code&redirect_uri=" + redirect_uri + "&scope=" + scope;
}

int main() {
    thread serverThread(runServer);
    cout << "Server started on http://localhost:8080\n";
    set_authorization_link();
    cout << "Authorization link:\n" << authorization_link << "\n";
    cin.get();
    serverThread.join(); // Properly join the thread before exiting
    return 0;
}
