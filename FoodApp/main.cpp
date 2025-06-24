#include "FoodDeliveryApp.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define BOLD    "\033[1m"

using namespace std;

int main() {
    FoodDeliveryApp app;
    string command;

    cout << BLUE << BOLD << "Welcome to the Food Delivery App!" << RESET << endl;
    cout << "Commands: addrestaurant <name>, removerestaurant <name>, addfood <restaurant> <food> <price>, removefood <restaurant> <food>, list, exit" << endl;

    while (true) {
        cout << "\n> ";
        getline(cin, command);

        stringstream ss(command);
        vector<string> tokens;
        string token;
        while (ss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            cout << RED << "Error: Empty command." << RESET << endl;
            continue;
        }

        string cmd = tokens[0];
        transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        if (cmd == "exit") {
            cout << GREEN << "Exiting Food Delivery App. Goodbye!" << RESET << endl;
            break;
        } else if (cmd == "addrestaurant" && tokens.size() >= 2) {
            string name = command.substr(command.find(tokens[1]));
            app.addRestaurant(name);
        } else if (cmd == "removerestaurant" && tokens.size() >= 2) {
            string name = command.substr(command.find(tokens[1]));
            app.removeRestaurant(name);
        } else if (cmd == "addfood" && tokens.size() >= 4) {
            string restaurant = tokens[1];
            string price = tokens[tokens.size() - 1];
            string food = command.substr(command.find(tokens[1]) + tokens[1].length() + 1,
                                      command.rfind(tokens[tokens.size() - 1]) - command.find(tokens[1]) - tokens[1].length() - 2);
            app.addFood(restaurant, food, price);
        } else if (cmd == "removefood" && tokens.size() >= 3) {
            string restaurant = tokens[1];
            string food = command.substr(command.find(tokens[1]) + tokens[1].length() + 1);
            app.removeFood(restaurant, food);
        } else if (cmd == "list") {
            app.list();
        } else {
            cout << RED << "Error: Invalid command or arguments. Use: addrestaurant <name>, removerestaurant <name>, addfood <restaurant> <food> <price>, removefood <restaurant> <food>, list, exit" << RESET << endl;
        }
    }

    return 0;
}
