#ifndef HOTELMANAGEMENTSYSTEM_HPP
#define HOTELMANAGEMENTSYSTEM_HPP

#include <string>
#include <vector>
#include <unordered_map>

enum class RoomType {
    STANDARD,
    DELUXE,
    SUITE
};

enum class RoomStatus {
    AVAILABLE,
    OCCUPIED,
    UNDER_MAINTENANCE
};

enum class BookingStatus {
    CONFIRMED,
    CHECKED_IN,
    CHECKED_OUT,
    CANCELLED
};

class User
{
private:
    std::string userId;
    std::string name;
    std::string email;
    std::string phone;

public:
    User(std::string userId, std::string name, std::string email, std::string phone);

    std::string getId() const;
    std::string getName() const;
};

class Room {
private:
    std::string roomId;
    RoomType type;
    double pricePerDay;
    int capacity;

public:
    Room(std::string roomId, RoomType type, double pricePerDay, int capacity);

    std::string getId() const;
    RoomType getType() const;
    double getPricePerDay() const;
    int getCapacity() const;
    void displayInfo() const;
};

class RoomInventory {
private:
    Room* room;
    std::vector<bool> dates; // true if date is booked

public:
    RoomInventory(Room* room, int totalDates);

    Room* getRoom() const;
    bool isRoomAvailable(int startDate, int endDate);
    void bookRoom(int startDate, int endDate);
};

class Booking {
private:
    std::string bookingId;
    User* user;
    Room* room;
    int checkInDate;
    int checkOutDate;
    int numberOfDates;
    double totalPrice;
    BookingStatus status;

public:
    Booking(std::string bookingId, User* user, Room* room, int checkInDate,
        int checkOutDate);

    std::string getId() const;
    User* getUser() const;
    Room* getRoom() const;
    int getCheckInDate() const;
    int getCheckOutDate() const;
    int getNumberOfDates() const;
    double getTotalPrice() const;
    BookingStatus getStatus() const;

    void calculateTotalPrice();
    void setStatus(BookingStatus status);
};

class HotelManager {
private:
    std::unordered_map<std::string, RoomInventory*> roomInventory_map;
    std::unordered_map<std::string, User*> users_map;
    std::unordered_map<std::string, Booking*> bookings_map;
    int bookingIdCounter;

public:
    HotelManager();
    //~HotelManager();

    void addRoom(Room* room);
    void addUser(User* user);

    Booking* createBooking(std::string userId, std::string roomId, int checkInDate, int checkOutDate);
    void displayAvailableRooms(int checkInDate, int checkOutDate) const;

private:
    std::string generateBookingId();
};

#endif