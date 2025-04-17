#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

using namespace std;

const string DB_FILE = "test_history.db";

// Helper to execute schema setup
void setup_schema(sqlite3* db) {
    const char* sql = R"sql(
        DROP TABLE IF EXISTS track_history;

        CREATE TABLE track_history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            artist TEXT NOT NULL,
            album TEXT NOT NULL,
            duration INTEGER NOT NULL,
            playcount INTEGER NOT NULL DEFAULT 1,
            created_at TEXT NOT NULL,
            last_played TEXT NOT NULL,
            UNIQUE(name, artist, album)
        );
    )sql";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Schema Error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

// Simulates an UPSERT, which may skip IDs
void test_upsert(sqlite3* db, const string& name) {
    const char* sql = R"sql(
        INSERT INTO track_history (name, artist, album, duration, playcount, created_at, last_played)
        VALUES (?, ?, ?, ?, 1, datetime('now'), datetime('now'))
        ON CONFLICT(name, artist, album) DO UPDATE SET
            playcount = track_history.playcount + 1,
            last_played = datetime('now');
    )sql";

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, "Test Artist", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, "Test Album", -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, 180);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

// Simulates a SELECT + UPDATE or INSERT (ID is preserved)
void test_manual_check(sqlite3* db, const string& name) {
    const char* select_sql = R"sql(
        SELECT id, playcount FROM track_history
        WHERE name = ? AND artist = ? AND album = ?
    )sql";

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, "Test Artist", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, "Test Album", -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    bool exists = (rc == SQLITE_ROW);
    int playcount = exists ? sqlite3_column_int(stmt, 1) : 0;
    sqlite3_finalize(stmt);

    if (exists) {
        const char* update_sql = R"sql(
            UPDATE track_history
            SET playcount = ?, last_played = datetime('now')
            WHERE name = ? AND artist = ? AND album = ?
        )sql";

        sqlite3_prepare_v2(db, update_sql, -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, playcount + 1);
        sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, "Test Artist", -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, "Test Album", -1, SQLITE_STATIC);
        sqlite3_step(stmt);
    }
    else {
        const char* insert_sql = R"sql(
            INSERT INTO track_history (name, artist, album, duration, playcount, created_at, last_played)
            VALUES (?, ?, ?, ?, 1, datetime('now'), datetime('now'))
        )sql";

        sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, "Test Artist", -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, "Test Album", -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, 180);
        sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
}

void print_rows(sqlite3* db, const string& label) {
    cout << "=== " << label << " ===" << endl;
    const char* sql = R"sql(
        SELECT id, name, playcount FROM track_history ORDER BY id;
    )sql";

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int playcount = sqlite3_column_int(stmt, 2);
        cout << "id: " << id << ", name: " << name << ", playcount: " << playcount << endl;
    }

    sqlite3_finalize(stmt);
}

#include <chrono>

constexpr int TEST_ITERATIONS = 10000;

void benchmark_upsert(sqlite3* db) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < TEST_ITERATIONS; ++i) {
        test_upsert(db, "Track_" + to_string(i % 250)); // force some duplicates
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "UPSERT strategy: " << duration.count() << " ms for " << TEST_ITERATIONS << " inserts\n";
}

void benchmark_manual_check(sqlite3* db) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < TEST_ITERATIONS; ++i) {
        test_manual_check(db, "Track_" + to_string(i % 250)); // force some duplicates
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Manual SELECT+UPDATE/INSERT strategy: " << duration.count() << " ms for " << TEST_ITERATIONS << " inserts\n";
}


int main() {
    sqlite3* db = nullptr;
    if (sqlite3_open(DB_FILE.c_str(), &db) != SQLITE_OK) {
        cerr << "Failed to open database." << endl;
        return 1;
    }

    setup_schema(db);

    // Test UPSERT (may skip IDs)
    test_upsert(db, "Track A");
    test_upsert(db, "Track B");
    test_upsert(db, "Track A");  // duplicate, causes conflict/update
    test_upsert(db, "Track C");
    print_rows(db, "After UPSERTs (with potential ID skips)");

    // Clear and reset
    setup_schema(db);

    // Test manual check ? update/insert
    test_manual_check(db, "Track A");
    test_manual_check(db, "Track B");
    test_manual_check(db, "Track A");  // will update, not insert
    test_manual_check(db, "Track C");
    print_rows(db, "After Manual Check (no skipped IDs)");


    setup_schema(db); // clear and reset again
    benchmark_manual_check(db);

    setup_schema(db);
    benchmark_upsert(db);



    sqlite3_close(db);
    return 0;
}
