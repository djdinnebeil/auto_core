#include <iostream>
#include <unordered_map>
#include <string>
#include <string_view>
#include <vector>
#include <chrono>
#include <random>

// Constants
constexpr int MAP_SIZE = 100000;
constexpr int LOOKUPS = 1000000;

std::unordered_map<std::string, int> build_map() {
    std::unordered_map<std::string, int> map;
    for (int i = 0; i < MAP_SIZE; ++i) {
        map["key_" + std::to_string(i)] = i;
    }
    return map;
}

int lookup_with_string(const std::unordered_map<std::string, int>& map, const std::string& key) {
    auto it = map.find(key);
    return (it != map.end()) ? it->second : -1;
}

int lookup_with_string_view(const std::unordered_map<std::string_view, int>& map, std::string_view key) {
    auto it = map.find(key);
    return (it != map.end()) ? it->second : -1;
}

int main() {
    // Setup
    auto std_string_map = build_map();
    std::unordered_map<std::string_view, int> string_view_map(std_string_map.begin(), std_string_map.end());

    // Prepare random keys
    std::vector<std::string> lookup_keys;
    lookup_keys.reserve(LOOKUPS);
    for (int i = 0; i < LOOKUPS; ++i) {
        int n = rand() % MAP_SIZE;
        lookup_keys.push_back("key_" + std::to_string(n));
    }

    // Test const std::string& version
    auto start1 = std::chrono::high_resolution_clock::now();
    int sum1 = 0;
    for (const auto& key : lookup_keys) {
        sum1 += lookup_with_string(std_string_map, key);
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_string = end1 - start1;

    // Test std::string_view version
    auto start2 = std::chrono::high_resolution_clock::now();
    int sum2 = 0;
    for (const auto& key : lookup_keys) {
        sum2 += lookup_with_string_view(string_view_map, key);
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_string_view = end2 - start2;

    // Results
    std::cout << "string version:      " << time_string.count() << " seconds\n";
    std::cout << "string_view version: " << time_string_view.count() << " seconds\n";
    std::cout << "sanity check: " << (sum1 == sum2 ? "OK" : "Mismatch") << "\n";

    return 0;
}
