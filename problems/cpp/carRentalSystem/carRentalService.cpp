#include "carRentalService.hpp"
#include <iostream>

User::User(std::string userId, std::string name, std::string email, std::string phone)
    : userId(userId), name(name), email(email), phone(phone) {}

std::string User::getUserId() const { return userId; }
std::string User::getName() const { return name; }

Car::Car(std::string carId, std::string brand, std::string model, double pricePerDay)
    : carId(carId), brand(brand), model(model), pricePerDay(pricePerDay) {}

std::string Car::getCarId() const { return carId; }
std::string Car::getBrand() const { return brand; }
std::string Car::getModel() const { return model; }
double Car::getPricePerDay() const { return pricePerDay; }

CarInventory::CarInventory(Car* car, int totalDates)
    : car(car), dates(totalDates, false) {}

Car* CarInventory::getCar() const { return car; }
bool CarInventory::isCarAvailable(int date) { 
    if (date > dates.size()) return false;
    return dates[date];
}

void CarInventory::bookCar(int date) {
    dates[date] = true;
}

Booking::Booking(std::string bookingId, User* user, Car* car, int startDate, int days)
    : bookingId(bookingId), user(user), car(car), startDate(startDate), days(days) {
        totalPrice = car->getPricePerDay() * days;
    }

std::string Booking::getBookingId() const { return bookingId; }
User* Booking::getUser() const { return user; }
Car* Booking::getCar() const { return car; }
int Booking::getStartDate() const { return startDate; }


CarRentalService::CarRentalService() : bookingIdCounter(1) {}

CarRentalService::~CarRentalService() {}

void CarRentalService::addUser(User* user) {
    users_map[user->getUserId()] = user;
}

void CarRentalService::addCar(Car* car) {
    carInventory_map[car->getCarId()] = new CarInventory(car, 60);
}

std::string CarRentalService::rentCar(User* user, Car* car, int startDate, int days) {
    CarInventory* carInventory = carInventory_map[car->getCarId()];

    // check if all dates are available
    for (int date = startDate; date < startDate + days; date++) {
        if (!carInventory->isCarAvailable(date)) {
            return "";
        }
    }

    // book all dates
    for (int date = startDate; date < startDate + days; date++) {
        carInventory->bookCar(date);
    }
    std::string bookingId = "B" + std::to_string(bookingIdCounter++);
    Booking* booking = new Booking(bookingId, user, car, startDate, days);
    bookings_map[bookingId] = booking;
    return bookingId;
}

bool returnCar(std::string bookingId) {
    return true;
}

int main() {
    CarRentalService carRentalService;

    // Add cars, users
    Car* car1 = new Car("C1", "BMW", "Model 1", 100.0);
    Car* car2 = new Car("C2", "Audi", "Model 2", 150.0);
    Car* car3 = new Car("C3", "Toyota", "Model 3", 80.0);
    User* user1 = new User("U1", "User1", "user1@gmail.com", "XXXX1234");
    User* user2 = new User("U2", "User2", "user2@gmail.com", "XXXX1290");
    carRentalService.addCar(car1);
    carRentalService.addCar(car2);
    carRentalService.addCar(car3);
    carRentalService.addUser(user1);
    carRentalService.addUser(user2);

    // Rent cars
    std::string booking1 = carRentalService.rentCar(user1, car1, 1, 5);
    std::string booking2 = carRentalService.rentCar(user2, car2, 7, 9);
    if (!booking1.empty()) {
        std::cout << "Rental booking created successfully: " << booking1 << std::endl;
    }
    if (!booking2.empty()) {
        std::cout << "Rental booking created successfully: " << booking2 << std::endl;
    }
}
