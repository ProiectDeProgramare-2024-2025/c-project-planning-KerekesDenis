#ifndef FOODDELIVERYAPP_H
#define FOODDELIVERYAPP_H

#include <string>
#include <vector>
#include <map>

struct FoodItem {
    std::string name;
    double price;
};

struct Restaurant {
    std::string name;
    std::vector<FoodItem> menu;
};

class FoodDeliveryApp {
private:
    std::map<std::string, Restaurant> restaurants;
    const std::string dataFile = "foodapp_data.txt";

    std::string trim(const std::string& str);
    bool isValidPrice(const std::string& priceStr);
    bool isValidName(const std::string& name);
    void saveData();
    void loadData();

public:
    FoodDeliveryApp();
    ~FoodDeliveryApp();


    void modifyRestaurantName(const std::string& oldName, const std::string& newName);
    void modifyFoodName(const std::string& restaurant, const std::string& oldFoodName, const std::string& newFoodName);
    void modifyFoodPrice(const std::string& restaurant, const std::string& food, const std::string& newPrice);

    bool addRestaurant(const std::string& name);
    bool removeRestaurant(const std::string& name);
    bool addFood(const std::string& restaurantName, const std::string& foodName, const std::string& priceStr);
    bool removeFood(const std::string& restaurantName, const std::string& foodName);
    void list();
};

#endif
