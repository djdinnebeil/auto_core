import print_b;
import <iostream>;
import <sqlite3.h>;

using namespace std;

int main() {
    sqlite3* db;
    char* errMsg = 0;
    int result;

    result = sqlite3_open("sp_history.db", &db);
    if (result) {
        print("Error opening/creating the database: {}", sqlite3_errmsg(db));
        return result;
    }
    else {
        print("Opened/created database successfully");
    }

    const char* sql = R"sql(
        CREATE TABLE IF NOT EXISTS spotify_history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            artist TEXT NOT NULL,
            album TEXT NOT NULL,
            playcount INTEGER NOT NULL DEFAULT 1,
            last_played TEXT NOT NULL
        );
    )sql";

    result = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (result != SQLITE_OK) {
        print("SQL error: {}", errMsg);
        sqlite3_free(errMsg);
    }
    else {
        print("Table 'spotify_history' created successfully");
    }

    sqlite3_close(db);
    print("Press any key to exit...");
    cin.get();
    return 0;
}
