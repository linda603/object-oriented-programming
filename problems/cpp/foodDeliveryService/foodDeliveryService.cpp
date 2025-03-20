#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iomanip>

enum class OrderStatus {
    PLACED,
    PREPARING,
    OUT_FOR_DELIVERY,
    DELIVERED,
    CANCELLED
};

class User {
private:
    std::string userId;
    std::string name;
    std::string email;
    std::string phone;

public:
    User(std::string userId, std::string name, std::string email, std::string phone)
        : userId(userId), name(name), email(email), phone(phone) {}

    std::string getId() const { return userId; }
    std::string getName() const { return name; }
};

class MenuItem {
private:
    std::string itemId;
    std::string name;
    std::string description;
    double price;
    bool available;

public:
    MenuItem(std::string itemId, std::string name, std::string description, double price)
        : itemId(itemId), name(name), description(description), price(price) {}

    std::string getId() const { return itemId; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    double getPrice() const { return price; }
    bool isAvailable() const { return available; }

    void setStatus(bool status) { available = status; }
};

class Restaurant {
private:
    std::string restaurantId;
    std::string name;
    std::string address;
    std::vector<MenuItem*> menu;
    bool isOpen;

public:
    Restaurant(std::string restaurantId, std::string name, std::string address)
        : restaurantId(restaurantId), name(name), address(address), isOpen(true) {}
    ~Restaurant() {
        for (auto item: menu) {
            delete item;
        }
    }

    std::string getId() const { return restaurantId; }
    std::string getName() const { return name; }
    bool isAvailable() const { return isOpen; }

    void addMenuItem(MenuItem* item) {
        menu.push_back(item);
    }

    void removeMenuItem(MenuItem* item) {
        auto it = std::find(menu.begin(), menu.end(), item);
        if (it != menu.end()) {
            menu.erase(it);
        }
    }

    MenuItem* findMenuItem(const std::string& itemId) const {
        for (auto item : menu) {
            if (item->getId() == itemId) {
                return item;
            }
        }
        return nullptr;
    }

    void setStatus(bool status) { isOpen = status; }
};

class Order {
private:
    std::string orderId;
    User* user;
    Restaurant* restaurant;
    std::unordered_map<MenuItem*, int> items; // item: quantity
    OrderStatus status;
    double totalAmount;
    std::string timestamp;

public:
    Order(std::string orderId, User* user, Restaurant* restaurant, std::unordered_map<MenuItem*, int> items)
        : orderId(orderId), user(user), restaurant(restaurant), status(OrderStatus::PLACED), items(items) {
        auto now = std::time(nullptr);
        timestamp = std::ctime(&now);
        calculateTotalAmount();
    }

    std::string getId() const { return orderId; }
    User* getUser() const { return user; }
    Restaurant* getRestaurant() const { return restaurant; }

    void setStatus(OrderStatus updated_status) {
        status = updated_status;
    }

    void calculateTotalAmount() {
        totalAmount = 0.0;
        for (const auto& pair: items) {
            totalAmount += pair.first->getPrice() * pair.second;
        }
    }
};

class DeliveryService {
private:
    std::unordered_map<std::string, User*> users;
    std::unordered_map<std::string, Restaurant*> restaurants;
    std::unordered_map<std::string, Order*> orders;
    int orderIdCounter;

public:
    DeliveryService() : orderIdCounter(1) {}
    ~DeliveryService() {
        for (auto pair : users) users.erase(pair.first);
        for (auto pair : restaurants) restaurants.erase(pair.first);
        for (auto pair : orders) orders.erase(pair.first);
    }

    void addUser(User* user) { users[user->getId()] = user; }
    void addRestaurant(Restaurant* restaurant) { restaurants[restaurant->getId()] = restaurant; }

    Order* createOrder(std::string userId, std::string restaurantId, std::unordered_map<MenuItem*, int> items) {
        User* user = users[userId];
        Restaurant* restaurant = restaurants[restaurantId];
        if (!user || !restaurant || !restaurant->isAvailable()) return nullptr;

        Order* order = new Order(generateOrderId(), user, restaurant, items);
        std::cout << user->getName() << " ordered from " << restaurant->getName() << " successfully! " << std::endl;
        std::cout << " Order ID: " << order->getId() << std::endl; 
        orders[order->getId()] = order;
        return order;
    }

    bool updateOrderStatus(std::string orderId, OrderStatus status) {
        Order* order = orders[orderId];
        if (!order) return false;
        order->setStatus(status);
        std::cout << "Order ID: " << order->getId() << "'s status changed to ";
        if (status == OrderStatus::PREPARING) {
            std::cout << "PREPARING" << std::endl;
        }
        else if (status == OrderStatus::OUT_FOR_DELIVERY) {
            std::cout << "OUT_FOR_DELIVERY" << std::endl;
        }
        else if (status == OrderStatus::DELIVERED) {
            std::cout << "DELIVDERED" << std::endl;
        }
        return true;
    }

private:
    std::string generateOrderId() {
        return "O" + std::to_string(orderIdCounter++);
    }
};

int main() {
    DeliveryService deliveryService;

    // Create user
    User* user1 = new User("U1", "User 1", "user1@gmail.com", "XXXX1234");
    deliveryService.addUser(user1);

    // Create restaurant
    Restaurant* restaurant1 = new Restaurant("R1", "Restaurant 1", "123 Main St");
    MenuItem* item1 = new MenuItem("I1", "Item1", "Description 1", 12.00);
    MenuItem* item2 = new MenuItem("I2", "Item2", "Description 2", 15.00);
    restaurant1->addMenuItem(item1);
    restaurant1->addMenuItem(item2);
    deliveryService.addRestaurant(restaurant1);

    std::unordered_map<MenuItem*, int> items {{item1, 2}, {item2, 1}};
    Order* order = deliveryService.createOrder("U1", "R1", items);
    deliveryService.updateOrderStatus(order->getId(), OrderStatus::PREPARING);
    deliveryService.updateOrderStatus(order->getId(), OrderStatus::OUT_FOR_DELIVERY);
    deliveryService.updateOrderStatus(order->getId(), OrderStatus::DELIVERED);
    return 0;
}