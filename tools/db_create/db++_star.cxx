import print_b;
import <iostream>;
import <sqlite3.h>;

using namespace std;

int main() {
    sqlite3* db;
    char* errMsg = 0;
    int result;
    result = sqlite3_open("star.db", &db);
    if (result) {
        print("Error opening/creating the database: {}", sqlite3_errmsg(db));
        return result;
    }
    else {
        print("Opened/created database successfully");
    }
    const char* sql = "CREATE TABLE IF NOT EXISTS counter (value INTEGER);";
    result = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (result != SQLITE_OK) {
        print("SQL error: {}", errMsg);
        sqlite3_free(errMsg);
    }
    else {
        print("Table created successfully");
    }
    const char* insertSQL = "INSERT INTO counter (value) VALUES (1);";
    result = sqlite3_exec(db, insertSQL, 0, 0, &errMsg);
    if (result != SQLITE_OK) {
        print("SQL error: {}", errMsg);
        sqlite3_free(errMsg);
    }
    else {
        print("Initial value inserted successfully");
    }
    sqlite3_close(db);
    print("Press any key to exit...");
    cin.get();
    return 0;
}
