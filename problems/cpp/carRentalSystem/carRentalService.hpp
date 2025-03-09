#ifndef CARRENTALSERVICE.HPP
#define CARRENTALSERVICE.HPP

#include <string>
#include <vector>
#include <unordered_map>


class User {
private:
    std::string userId;
    std::string name;
    std::string email;
    std::string phone;

public:
    User(std::string userId, std::string name, std::string email, std::string phone);

    std::string getUserId() const;
    std::string getName() const;
};

class Car {
private:
    std::string carId;
    std::string brand;
    std::string model;
    double pricePerDay;

public:
    Car(std::string carId, std::string brand, std::string model, double pricePerDay);

    std::string getCarId() const;
    std::string getBrand() const;
    std::string getModel() const;
    double getPricePerDay() const;
};

class CarInventory {
private:
    Car* car;
    std::vector<bool> dates; // true if date is booked

public:
    CarInventory(Car* car, int totalDates);

    Car* getCar() const;
    bool isCarAvailable(int date);
    void bookCar(int date);
};

class Booking {
private:
    std::string bookingId;
    User* user;
    Car* car;
    int startDate;
    int days;
    double totalPrice;

public:
    Booking(std::string rentalId, User* user, Car* car, int startDate, int days);

    std::string getBookingId() const;
    User* getUser() const;
    Car* getCar() const;
    int getStartDate() const;
    int getDays() const;
    double getTotalPrice() const;
};

class CarRentalService {
private:
    std::unordered_map<std::string, User*> users_map;
    std::unordered_map<std::string, CarInventory*> carInventory_map;
    std::unordered_map<std::string, Booking*> bookings_map;
    int bookingIdCounter;

public:
    CarRentalService();
    ~CarRentalService();

    void addUser(User* user);
    void addCar(Car* car);
    std::string rentCar(User* user, Car* car, int startDate, int days);
    bool returnCar(std::string bookingId);

    void checkAvailCars() const;
};

#endif