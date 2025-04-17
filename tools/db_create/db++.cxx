import print_b;
import <iostream>;
import <sqlite3.h>;

using namespace std;

int main() {
    sqlite3* db = nullptr;
    char* errMsg = nullptr;

    // Open or create the database
    if (sqlite3_open("sp_history.db", &db) != SQLITE_OK) {
        print("Error opening/creating the database: {}", sqlite3_errmsg(db));
        return 1;
    }
    print("Opened/created database successfully");

    // SQL to create table + index
    const char* sql = R"sql(
        BEGIN;
        CREATE TABLE IF NOT EXISTS track_history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            artist TEXT NOT NULL,
            album TEXT NOT NULL,
            duration INTEGER NOT NULL,
            playcount INTEGER NOT NULL DEFAULT 1,
            created_at TEXT NOT NULL,
            last_played TEXT NOT NULL
        );

        -- Composite index for fast lookups and update checks
        CREATE UNIQUE INDEX IF NOT EXISTS idx_song_identity 
        ON track_history(name, artist, album);
        COMMIT;
    )sql";

    // Execute SQL
    int result = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (result != SQLITE_OK) {
        print("SQL error: {}", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    print("Table and index created successfully");
    sqlite3_close(db);

    print("Press Enter to exit...");
    cin.get();
    return 0;
}
