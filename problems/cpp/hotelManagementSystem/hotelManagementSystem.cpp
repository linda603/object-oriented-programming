#include "hotelManagementSystem.hpp"
#include <iostream>

User::User(std::string userId, std::string name, std::string email, std::string phone)
    : userId(userId), name(name), email(email), phone(phone) {}

std::string User::getId() const { return userId; }
std::string User::getName() const { return name; }

Room::Room(std::string roomId, RoomType type, double pricePerDay, int capacity)
    : roomId(roomId), type(type), pricePerDay(pricePerDay), capacity(capacity) {}

std::string Room::getId() const { return roomId; }
RoomType Room::getType() const { return type; }
double Room::getPricePerDay() const { return pricePerDay; }
int Room::getCapacity() const { return capacity; }
void Room::displayInfo() const {
    std::cout << "Room: " << roomId << " - with capacity: " << capacity << std::endl;
    std::cout << "Type: ";
    if (type == RoomType::STANDARD) {
        std::cout << "Standard " << std::endl;
    }
    else if (type == RoomType::DELUXE) {
        std::cout << "Deluxe " << std::endl; 
    }
    else {
        std::cout << "Suite " << std::endl; 
    }
    std::cout << " - Price per day: " << pricePerDay << "\n" << std::endl;
}

RoomInventory::RoomInventory(Room* room, int totalDates)
    : room(room), dates(totalDates, false) {}

Room* RoomInventory::getRoom() const { return room; }
bool RoomInventory::isRoomAvailable(int startDate, int endDate) { 
    for (int date = startDate; date <= endDate; date++) {
        if (dates[date]) {
            return false;
        }
    }
    return true;
}

void RoomInventory::bookRoom(int startDate, int endDate) {
    for (int date = startDate; date <= endDate; date++) {
        dates[date] = true;
    }
}

Booking::Booking(std::string bookingId, User* user, Room* room, int checkInDate, int checkOutDate)
    : bookingId(bookingId), user(user), room(room), checkInDate(checkInDate), checkOutDate(checkOutDate), numberOfDates(checkOutDate - checkInDate), 
    status(BookingStatus::CONFIRMED) {
        calculateTotalPrice();
    }

std::string Booking::getId() const { return bookingId; }
User* Booking::getUser() const { return user; }
Room* Booking::getRoom() const { return room; }
int Booking::getCheckInDate() const { return checkInDate; }
int Booking::getCheckOutDate() const { return checkOutDate; }
int Booking::getNumberOfDates() const { return numberOfDates; }
double Booking::getTotalPrice() const { return totalPrice; }
BookingStatus Booking::getStatus() const { return status; }

void Booking::calculateTotalPrice() {
    totalPrice = room->getPricePerDay() * numberOfDates;
}

void Booking::setStatus(BookingStatus newStatus) {
    status = newStatus;
}

HotelManager::HotelManager() : bookingIdCounter(1) {}

void HotelManager::addRoom(Room* room) {
    roomInventory_map[room->getId()] = new RoomInventory(room, 60);
}

void HotelManager::addUser(User* user) {
    users_map[user->getId()] = user;
}

Booking* HotelManager::createBooking(std::string userId, std::string roomId, int checkInDate, int checkOutDate) {
    User* user = users_map[userId];
    RoomInventory* roomInventory = roomInventory_map[roomId];

    if (!user || !roomInventory) return nullptr;
    // Check if date from checkIn till checkOut are available
    if (!roomInventory->isRoomAvailable(checkInDate, checkOutDate)) return nullptr;

    // Book the date
    roomInventory->bookRoom(checkInDate, checkOutDate);

    // Create a booking
    Booking* booking = new Booking(generateBookingId(), user, roomInventory->getRoom(), checkInDate, checkOutDate);
    //booking->setStatus(BookingStatus::CONFIRMED);
    bookings_map[booking->getId()] = booking;
    return booking;
}

std::string HotelManager::generateBookingId() {
    return "B" + std::to_string(bookingIdCounter++);
}

void HotelManager::displayAvailableRooms(int checkInDate, int checkOutDate) const {

    std::cout << "Available rooms: \n" << std::endl;
    for (auto it = roomInventory_map.begin(); it != roomInventory_map.end(); it++) {
        RoomInventory* currRoomInventory = it->second;
        if (currRoomInventory->isRoomAvailable(checkInDate, checkOutDate)) {
            currRoomInventory->getRoom()->displayInfo();
        }
    }
}

int main() {
    HotelManager hotelmanager;
    Room* room1 = new Room("101", RoomType::STANDARD, 80.0, 2);
    Room* room2 = new Room("102", RoomType::DELUXE, 100.0, 2);
    Room* room3 = new Room("103", RoomType::SUITE, 120.0, 4);

    hotelmanager.addRoom(room1);
    hotelmanager.addRoom(room2);
    hotelmanager.addRoom(room3);

    User* user1 = new User("U1", "User 1", "user1@gmail.com", "XXXX1234");
    hotelmanager.addUser(user1);

    hotelmanager.displayAvailableRooms(1, 5);
    // Create a booking
    Booking* booking = hotelmanager.createBooking(user1->getId(), room1->getId(), 1, 5);
    std::cout << "Booking is create successfully " << booking->getId() << std::endl;
    std::cout << "Room: " << booking->getRoom()->getId() << "\n" << std::endl;
}