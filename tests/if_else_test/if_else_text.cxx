#include <iostream>
#include <sstream>
#include <chrono>
#include <string>

constexpr int ITERATIONS = 10'000;
constexpr int RUNS = 10;

double version1_total;
double version2_total;
double version3_total;
double version4_total;

void version1() {
    std::ostringstream log_buffer;
    bool log_buffer_check = false;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; ++i) {
        std::string key = "key" + std::to_string(i);
        std::string primary = "primary_val";
        std::string secondary = "secondary_val";

        if (log_buffer_check) {
            log_buffer << "\n" << key << " = " << primary << ", " << secondary;
        }
        else {
            log_buffer_check = true;
            log_buffer << key << " = " << primary << ", " << secondary;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    version1_total += elapsed.count();
    std::cout << "Version 1 took: " << elapsed.count() << " seconds\n";
}

void version2() {
    std::ostringstream log_buffer;
    bool log_buffer_empty = true;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; ++i) {
        std::string key = "key" + std::to_string(i);
        std::string primary = "primary_val";
        std::string secondary = "secondary_val";

        if (log_buffer_empty) {
            log_buffer_empty = false;
            log_buffer << key << " = " << primary << ", " << secondary;
        }
        else {
            log_buffer << "\n" << key << " = " << primary << ", " << secondary;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    version2_total += elapsed.count();
    std::cout << "Version 2 took: " << elapsed.count() << " seconds\n";
}

void version3() {
    std::ostringstream log_buffer;
    bool log_buffer_empty = true;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; ++i) {
        std::string key = "key" + std::to_string(i);
        std::string primary = "primary_val";
        std::string secondary = "secondary_val";

        if (!log_buffer_empty) {
            log_buffer << "\n";
        }
        log_buffer << key << " = " << primary << ", " << secondary;
        log_buffer_empty = false;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    version3_total += elapsed.count();
    std::cout << "Version 3 took: " << elapsed.count() << " seconds\n";
}

void version4() {
    std::ostringstream log_buffer;
    bool log_buffer_add_newline = false;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; ++i) {
        std::string key = "key" + std::to_string(i);
        std::string primary = "primary_val";
        std::string secondary = "secondary_val";

        if (log_buffer_add_newline) {
            log_buffer << "\n";
        }
        log_buffer << key << " = " << primary << ", " << secondary;
        log_buffer_add_newline = true;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    version4_total += elapsed.count();
    std::cout << "Version 4 took: " << elapsed.count() << " seconds\n";
}

int main() {

    for (int i = 0; i < RUNS; i++) {
        //version1();
        version2();
        version3();
        //version4();
    }

    std::cout << "\n--- Averages ---\n";
    //std::cout << "Version 1 Average: " << version1_total / RUNS << " seconds\n";
    std::cout << "Version 2 Average: " << version2_total / RUNS << " seconds\n";
    std::cout << "Version 3 Average: " << version3_total / RUNS << " seconds\n";
    //std::cout << "Version 4 Average: " << version4_total / RUNS << " seconds\n";

    return 0;
}
