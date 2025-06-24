#include "FoodDeliveryApp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define BOLD    "\033[1m"

using namespace std;

// Implementations of FoodDeliveryApp methods exactly as in your original code,
// with the private helper functions and public methods, unchanged.

string FoodDeliveryApp::trim(const string& str) {
    string result = str;
    result.erase(result.begin(), find_if(result.begin(), result.end(), [](unsigned char c) { return !isspace(c); }));
    result.erase(find_if(result.rbegin(), result.rend(), [](unsigned char c) { return !isspace(c); }).base(), result.end());
    return result;
}

bool FoodDeliveryApp::isValidPrice(const string& priceStr) {
    try {
        size_t pos;
        double price = stod(priceStr, &pos);
        if (pos != priceStr.length() || price < 0) return false;
        return true;
    } catch (...) {
        return false;
    }
}

bool FoodDeliveryApp::isValidName(const string& name) {
    string trimmed = trim(name);
    if (trimmed.empty()) return false;
    return all_of(trimmed.begin(), trimmed.end(), [](char c) {
        return isalnum(c) || c == ' ' || c == '-' || c == '\'';
    });
}

void FoodDeliveryApp::saveData() {
    ofstream outFile(dataFile);
    if (!outFile) {
        cout << RED << "Error: Could not save data to file." << RESET << endl;
        return;
    }
    for (const auto& entry : restaurants) {
        const Restaurant& restaurant = entry.second;
        outFile << "RESTAURANT:" << restaurant.name << "\n";
        for (const auto& item : restaurant.menu) {
            outFile << "ITEM:" << item.name << "," << item.price << "\n";
        }
        outFile << "ENDRESTAURANT\n";
    }
    outFile.close();
}

void FoodDeliveryApp::loadData() {
    ifstream inFile(dataFile);
    if (!inFile) {
        return;
    }
    string line;
    Restaurant currentRestaurant;
    bool inRestaurant = false;
    while (getline(inFile, line)) {
        if (line.find("RESTAURANT:") == 0) {
            currentRestaurant.name = line.substr(11);
            currentRestaurant.menu.clear();
            inRestaurant = true;
        }
        else if (line.find("ITEM:") == 0 && inRestaurant) {
            size_t commaPos = line.find(',', 5);
            if (commaPos != string::npos) {
                string itemName = line.substr(5, commaPos - 5);
                double price = stod(line.substr(commaPos + 1));
                currentRestaurant.menu.push_back({itemName, price});
            }
        }
        else if (line == "ENDRESTAURANT" && inRestaurant) {
            restaurants[currentRestaurant.name] = currentRestaurant;
            inRestaurant = false;
        }
    }
    inFile.close();
}

FoodDeliveryApp::FoodDeliveryApp() {
    loadData();
}

FoodDeliveryApp::~FoodDeliveryApp() {
    saveData();
}

bool FoodDeliveryApp::addRestaurant(const string& name) {
    string trimmedName = trim(name);
    if (!isValidName(trimmedName)) {
        cout << RED << "Error: Invalid restaurant name. Use alphanumeric characters, spaces, hyphens, or apostrophes." << RESET << endl;
        return false;
    }
    if (restaurants.find(trimmedName) != restaurants.end()) {
        cout << RED << "Error: Restaurant '" << trimmedName << "' already exists." << RESET << endl;
        return false;
    }
    restaurants[trimmedName] = Restaurant{trimmedName, {}};
    saveData();
    cout << GREEN << "Restaurant '" << trimmedName << "' added successfully." << RESET << endl;
    return true;
}

bool FoodDeliveryApp::removeRestaurant(const string& name) {
    string trimmedName = trim(name);
    if (restaurants.find(trimmedName) == restaurants.end()) {
        cout << RED << "Error: Restaurant '" << trimmedName << "' not found." << RESET << endl;
        return false;
    }
    restaurants.erase(trimmedName);
    saveData();
    cout << GREEN << "Restaurant '" << trimmedName << "' removed successfully." << RESET << endl;
    return true;
}

bool FoodDeliveryApp::addFood(const string& restaurantName, const string& foodName, const string& priceStr) {
    string trimmedRestaurant = trim(restaurantName);
    string trimmedFood = trim(foodName);

    if (!isValidName(trimmedFood)) {
        cout << RED << "Error: Invalid food name. Use alphanumeric characters, spaces, hyphens, or apostrophes." << RESET << endl;
        return false;
    }
    if (!isValidPrice(priceStr)) {
        cout << RED << "Error: Invalid price. Must be a positive number." << RESET << endl;
        return false;
    }
    if (restaurants.find(trimmedRestaurant) == restaurants.end()) {
        cout << RED << "Error: Restaurant '" << trimmedRestaurant << "' not found." << RESET << endl;
        return false;
    }

    double price = stod(priceStr);
    auto& menu = restaurants[trimmedRestaurant].menu;
    for (const auto& item : menu) {
        if (item.name == trimmedFood) {
            cout << RED << "Error: Food item '" << trimmedFood << "' already exists in '" << trimmedRestaurant << "'." << RESET << endl;
            return false;
        }
    }

    menu.push_back({trimmedFood, price});
    saveData();
    cout << GREEN << "Food item '" << trimmedFood << "' added to '" << trimmedRestaurant << "' for $" << fixed << setprecision(2) << price << "." << RESET << endl;
    return true;
}

bool FoodDeliveryApp::removeFood(const string& restaurantName, const string& foodName) {
    string trimmedRestaurant = trim(restaurantName);
    string trimmedFood = trim(foodName);

    if (restaurants.find(trimmedRestaurant) == restaurants.end()) {
        cout << RED << "Error: Restaurant '" << trimmedRestaurant << "' not found." << RESET << endl;
        return false;
    }

    auto& menu = restaurants[trimmedRestaurant].menu;
    auto it = find_if(menu.begin(), menu.end(), [&](const FoodItem& item) {
        return item.name == trimmedFood;
    });

    if (it == menu.end()) {
        cout << RED << "Error: Food item '" << trimmedFood << "' not found in '" << trimmedRestaurant << "'." << RESET << endl;
        return false;
    }

    menu.erase(it);
    saveData();
    cout << GREEN << "Food item '" << trimmedFood << "' removed from '" << trimmedRestaurant << "'." << RESET << endl;
    return true;
}

void FoodDeliveryApp::list() {
    if (restaurants.empty()) {
        cout << YELLOW << "No restaurants available." << RESET << endl;
        return;
    }

    cout << BLUE << BOLD << "\nFood Delivery App Menu" << RESET << endl;
    cout << BLUE << string(30, '-') << RESET << endl;
    for (const auto& entry : restaurants) {
        const string& name = entry.first;
        const Restaurant& restaurant = entry.second;
        cout << YELLOW << "Restaurant: " << name << RESET << endl;
        if (restaurant.menu.empty()) {
            cout << "  No items in menu." << endl;
        } else {
            for (const auto& item : restaurant.menu) {
                cout << "  - " << item.name << ": $" << fixed << setprecision(2) << item.price << endl;
            }
        }
        cout << endl;
    }
}  // <-- This closing brace was missing

void FoodDeliveryApp::modifyRestaurantName(const std::string& oldName, const std::string& newName) {
    string trimmedOld = trim(oldName);
    string trimmedNew = trim(newName);

    if (restaurants.find(trimmedOld) == restaurants.end()) {
        cout << RED << "Restaurant \"" << trimmedOld << "\" not found." << RESET << endl;
        return;
    }
    if (!isValidName(trimmedNew)) {
        cout << RED << "Invalid new restaurant name." << RESET << endl;
        return;
    }
    if (restaurants.find(trimmedNew) != restaurants.end()) {
        cout << RED << "Restaurant \"" << trimmedNew << "\" already exists." << RESET << endl;
        return;
    }

    Restaurant r = restaurants[trimmedOld];
    r.name = trimmedNew;
    restaurants.erase(trimmedOld);
    restaurants[trimmedNew] = r;

    saveData();
    cout << GREEN << "Restaurant renamed from \"" << trimmedOld << "\" to \"" << trimmedNew << "\"." << RESET << endl;
}

void FoodDeliveryApp::modifyFoodName(const std::string& restaurant, const std::string& oldFoodName, const std::string& newFoodName) {
    string trimmedRestaurant = trim(restaurant);
    string trimmedOldFood = trim(oldFoodName);
    string trimmedNewFood = trim(newFoodName);

    auto it = restaurants.find(trimmedRestaurant);
    if (it == restaurants.end()) {
        cout << RED << "Restaurant \"" << trimmedRestaurant << "\" not found." << RESET << endl;
        return;
    }
    if (!isValidName(trimmedNewFood)) {
        cout << RED << "Invalid new food name." << RESET << endl;
        return;
    }

    auto& menu = it->second.menu;

    // Check if newFoodName already exists
    for (const auto& item : menu) {
        if (item.name == trimmedNewFood) {
            cout << RED << "Food item \"" << trimmedNewFood << "\" already exists in \"" << trimmedRestaurant << "\"." << RESET << endl;
            return;
        }
    }

    for (auto& item : menu) {
        if (item.name == trimmedOldFood) {
            item.name = trimmedNewFood;
            saveData();
            cout << GREEN << "Food renamed from \"" << trimmedOldFood << "\" to \"" << trimmedNewFood << "\" in restaurant \"" << trimmedRestaurant << "\"." << RESET << endl;
            return;
        }
    }

    cout << RED << "Food item \"" << trimmedOldFood << "\" not found in restaurant \"" << trimmedRestaurant << "\"." << RESET << endl;
}

void FoodDeliveryApp::modifyFoodPrice(const std::string& restaurant, const std::string& foodName, const std::string& newPriceStr) {
    string trimmedRestaurant = trim(restaurant);
    string trimmedFood = trim(foodName);

    if (!isValidPrice(newPriceStr)) {
        cout << RED << "Invalid price. Must be a positive number." << RESET << endl;
        return;
    }

    auto it = restaurants.find(trimmedRestaurant);
    if (it == restaurants.end()) {
        cout << RED << "Restaurant \"" << trimmedRestaurant << "\" not found." << RESET << endl;
        return;
    }

    double newPrice = stod(newPriceStr);

    auto& menu = it->second.menu;
    for (auto& item : menu) {
        if (item.name == trimmedFood) {
            item.price = newPrice;
            saveData();
            cout << GREEN << "Price of \"" << trimmedFood << "\" in restaurant \"" << trimmedRestaurant << "\" changed to $" << fixed << setprecision(2) << newPrice << "." << RESET << endl;
            return;
        }
    }

    cout << RED << "Food item \"" << trimmedFood << "\" not found in restaurant \"" << trimmedRestaurant << "\"." << RESET << endl;
}

