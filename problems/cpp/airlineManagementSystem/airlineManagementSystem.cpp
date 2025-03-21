#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

enum class SeatType {
    STANDARD,
    BUSINESS,
};

enum class SeatStatus {
    AVAILABLE,
    BOOKED,
};

enum class BookingStatus {
    PENDING,
    CONFIRMED,
    CANCELLED
};

class Passenger {
private:
    std::string passengerId;
    std::string name;
    std::string passport;
    std::string email;

public:
    Passenger(std::string passengerId, std::string name, std::string passport, std::string email)
        : passengerId(passengerId), name(name), passport(passport), email(email) {}

    std::string getId() const { return passengerId; }
    std::string getName() const { return name; }
};

class Seat {
private:
    int seatId;
    SeatType type;

public:
    Seat(int seatId, SeatType type) : seatId(seatId), type(type) {}

    int getId() const { return seatId; }
    SeatType getType() const { return type; }
};

class AirCraft {
private:
    std::string flightNumber;
    int capacity;

public:
    AirCraft(std::string flightNumber, int capacity)
        : flightNumber(flightNumber), capacity(capacity) {}

    std::string getFlightNumber() const { return flightNumber; }
    int getCapacity() const { return capacity; }
};

class Flight {
private:
    std::string flightId;
    AirCraft* airCraft;
    std::string origin;
    std::string destination;
    std::string departureTime;
    std::unordered_map<Seat*, bool> seats; // true if booked

public:
    Flight(std::string flightId, AirCraft* airCraft, std::string origin, std::string destination, std::string departureTime)
        : flightId(flightId), airCraft(airCraft), origin(origin), destination(destination), departureTime(departureTime) {
        for (int i = 0; i < 10; i++) {
            seats[new Seat(i, SeatType::BUSINESS)] = false;
        }
        for (int i = 10; i < airCraft->getCapacity(); i++) {
            seats[new Seat(i, SeatType::STANDARD)] = false;
        }
    } 

    std::string getId() const { return flightId; }
    AirCraft* getAirCraft() const { return airCraft; }
    std::string getOrigin() const { return origin; }
    std::string getDestination() const { return destination; }
    std::string getDepartureTime() const { return departureTime; }
    std::vector<Seat*>& getAvailableSeats() { 
        std::vector<Seat*> res;
        for (auto pair : seats) {
            if (pair.second == false) {
                res.push_back(pair.first);
            }
        }
        return res;
    }

    bool bookSeat(Seat* seat) {
        if (!seat || seats[seat]) return false;
        seats[seat] = true;
        return true;
    }

    bool cancelSeat(Seat* seat) {
        if (!seat || !seats[seat]) return false;
        seats[seat] = false;
        return true;
    }

    bool isSeatAvailable(Seat* seat) {
        return seats[seat];
    }
};

class Booking {
private:
    std::string bookingId;
    Flight* flight;
    Passenger* passenger;
    Seat* seat;

public:
    Booking(std::string bookingId, Flight* flight, Passenger* passenger, Seat* seat)
        : bookingId(bookingId), flight(flight), passenger(passenger), seat(seat) {}

    std::string getId() const { return bookingId; }
    Flight* getFlight() const { return flight; }
    Passenger* getPassenger() const { return passenger; }
    Seat* getSeat() const { return seat; }
};

class AirlineManagementSytem {
private:
    std::unordered_map<std::string, AirCraft*> airCrafts;
    std::unordered_map<std::string, Flight*> flights;
    std::unordered_map<std::string, Passenger*> passengers;
    std::unordered_map<std::string, Booking*> bookings;
    int bookingIdCounter;

public:
    AirlineManagementSytem() : bookingIdCounter(1) {}

    void addAirCraft(AirCraft* airCraft) {
        airCrafts[airCraft->getFlightNumber()] = airCraft;
    }

    void addFlight(Flight* flight) {
        flights[flight->getId()] = flight;
    }

    void addPassenger(Passenger* passenger) {
        passengers[passenger->getId()] = passenger;
    }

    Booking* createBooking(Flight* flight, Passenger* passenger, Seat* seat) {
        if (!flight || !passenger || !seat || flight->isSeatAvailable(seat)) {
            return nullptr;
        }
        flight->bookSeat(seat);
        std::string bookingId = "B" + std::to_string(bookingIdCounter++);
        Booking* booking = new Booking(bookingId, flight, passenger, seat);
        bookings[bookingId] = booking;
        return booking;
    }
};

int main() {
    AirlineManagementSytem airlineManagementSytem;

    // Create flights
    AirCraft* airCraft1 = new AirCraft("AC001", 100);
    airlineManagementSytem.addAirCraft(airCraft1);

    Flight* flight1 = new Flight("F1", airCraft1, "Amsterdam", "London", "2025-03-21");
    Flight* flight2 = new Flight("F2", airCraft1, "London", "Amsterdam", "2025-03-21");
    airlineManagementSytem.addFlight(flight1);
    airlineManagementSytem.addFlight(flight2);

    // Create passengers
    Passenger* passenger1 = new Passenger("P1", "Passenger1", "XXXX1234", "p1@gmail.com");

    // Create bookings
    Booking* booking1 = airlineManagementSytem.createBooking(flight1, passenger1, flight1->getAvailableSeats()[0]);
    if (booking1) {
        std::cout << "Booking is created successfully. Booking ID: " << booking1->getId() << std::endl;
    }
    Booking* booking2 = airlineManagementSytem.createBooking(flight2, passenger1, flight2->getAvailableSeats()[0]);
    if (booking2) {
        std::cout << "Booking is created successfully. Booking ID: " << booking2->getId() << std::endl;
    }
    return 0;
}

