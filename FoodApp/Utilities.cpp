#include "Utilities.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>

std::string trim(const std::string& str) {
    std::string result = str;
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char c) { return !std::isspace(c); }));
    result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char c) { return !std::isspace(c); }).base(), result.end());
    return result;
}

bool isValidPrice(const std::string& priceStr) {
    try {
        size_t pos;
        double price = std::stod(priceStr, &pos);
        return (pos == priceStr.length() && price >= 0);
    } catch (...) {
        return false;
    }
}

bool isValidName(const std::string& name) {
    std::string trimmed = trim(name);
    if (trimmed.empty()) return false;
    return std::all_of(trimmed.begin(), trimmed.end(), [](char c) {
        return std::isalnum(c) || c == ' ' || c == '-' || c == '\'';
    });
}
