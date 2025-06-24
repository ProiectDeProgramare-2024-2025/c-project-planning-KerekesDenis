#include "FoodDeliveryApp.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

void printHelp() {
    cout << "Commands:\n";
    cout << "  modify_restaurant <old_name> <new_name>\n";
    cout << "  modify_food_name <restaurant_name> <old_food_name> <new_food_name>\n";
    cout << "  modify_food_price <restaurant_name> <food_name> <new_price>\n";
    cout << "  list (to list all restaurants and menus)\n";
    cout << "  exit (to quit)\n";
}

vector<string> split(const string& input) {
    istringstream iss(input);
    vector<string> tokens;
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    FoodDeliveryApp app;

    cout << "Food Delivery Modifier CLI\n";
    printHelp();

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;  // EOF or error

        auto args = split(line);
        if (args.empty()) continue;

        string cmd = args[0];

        if (cmd == "exit") {
            break;
        } else if (cmd == "list") {
            app.list();
        } else if (cmd == "modify_restaurant") {
            if (args.size() < 3) {
                cout << "Usage: modify_restaurant <old_name> <new_name>\n";
                continue;
            }
            string oldName = args[1];
            string newName = args[2];
            app.modifyRestaurantName(oldName, newName);
        } else if (cmd == "modify_food_name") {
            if (args.size() < 4) {
                cout << "Usage: modify_food_name <restaurant_name> <old_food_name> <new_food_name>\n";
                continue;
            }
            string restaurant = args[1];
            string oldFoodName = args[2];
            string newFoodName = args[3];
            app.modifyFoodName(restaurant, oldFoodName, newFoodName);
        } else if (cmd == "modify_food_price") {
            if (args.size() < 4) {
                cout << "Usage: modify_food_price <restaurant_name> <food_name> <new_price>\n";
                continue;
            }
            string restaurant = args[1];
            string foodName = args[2];
            string newPrice = args[3];
            app.modifyFoodPrice(restaurant, foodName, newPrice);
        } else {
            cout << "Unknown command: " << cmd << "\n";
            printHelp();
        }
    }

    cout << "Exiting Modifier CLI. Changes saved.\n";

    return 0;
}
