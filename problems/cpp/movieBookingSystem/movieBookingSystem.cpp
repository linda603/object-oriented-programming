#include "movieBookingSystem.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <ctime>
#include <sstream>

Movie::Movie(std::string movieId, std::string title, std::string description,
    MovieGenre genre, int DurationInMinutes, std::string language)
    : movieId(movieId), title(title), description(description), genre(genre),
    durationInMinutes(durationInMinutes), language(language), active(true) {}

std::string Movie::getId() const { return movieId; }
std::string Movie::getTitle() const { return title; }
std::string Movie::getDescription() const { return description; }
MovieGenre Movie::getGenre() const { return genre; }
int Movie::getDuration() const { return durationInMinutes; }
bool Movie::isActive() const { return active; }

void Movie::addCast(const std::string& actor) {
    casts.push_back(actor);
}

void Movie::setActive(bool status) {
    active = status;
}

Show::Show(std::string showId, Movie* movie, std::string date,
    std::string startTime, double price, int totalSeats)
    : showId(showId), movie(movie), date(date), startTime(startTime), price(price), 
    seats(totalSeats, false), status(ShowStatus::SCHEDULED) {}

std::string Show::getId() const { return showId; }
Movie* Show::getMovie() const { return movie; }
std::string Show::getDate() const { return date; }
std::string Show::getStartTime() const { return startTime; }
double Show::getPrice() const { return price; }
ShowStatus Show::getStatus() const { return status; }

bool Show::isSeatAvailable(int seatNumber) const {
    if (seatNumber > seats.size()) {
        return false;
    }
    return !seats[seatNumber];
}

bool Show::bookSeat(int seatNumber) {
    if (seatNumber > seats.size()) {
        return false;
    }
    seats[seatNumber] = true;
    return true;
}

void Show::cancelSeatBooking(int seatNumber) {
    if (seatNumber < seats.size()) {
        seats[seatNumber] = false;
    }
}

void Show::setStatus(ShowStatus status) {
    this->status = status;
}

int Show::getAvailableSeats() const {
    int count = 0;
    for (bool seat : seats) {
        if (!seat) count ++;
    }
    return count;
}

void Show::displayInfo() const {
    std::cout << "Movie: " << movie->getTitle() << std::endl;
    std::cout << "Date: " << date << std::endl;
    std::cout << "Start Time: " << startTime << std::endl;
}

Theater::Theater(std::string theaterId, std::string name, std::string location,
    int totalSeats) : theaterId(theaterId), name(name), location(location), 
    totalSeats(totalSeats), active(true) {}
Theater::~Theater() {
    for (auto show: shows) {
        delete show;
    }
}

std::string Theater::getId() const { return theaterId; }
std::string Theater::getName() const { return name; }
std::string Theater::getLocation() const { return location; }
int Theater::getTotalSeats() const { return totalSeats; }
bool Theater::isActive() const { return active; }
const std::vector<Show*>& Theater::getShows() const { return shows; }

void Theater::addShow(Show* show) {
    shows.push_back(show);
}

void Theater::setStatus(bool status) {
    active = status;
}

Booking::Booking(std::string bookingId, Show* show, std::string userName, 
    std::string userPhone, std::vector<int> seatNumbers)
    : bookingId(bookingId), show(show), userName(userName), userPhone(userPhone),
    seatNumbers(seatNumbers), status(BookingStatus::PENDING) {
        auto now = std::time(nullptr);
        auto time = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&time, "%Y-%m-%d %H:%M:%S");
        timestamp = oss.str();

        calculateTotalPrice();
    }

std::string Booking::getId() const { return bookingId; }
Show* Booking::getShow() const { return show; }
std::string Booking::getUserName() const { return userName; }
std::string Booking::getUserPhone() const { return userPhone; }
const std::vector<int>& Booking::getSeatNumbers() const { return seatNumbers; }
double Booking::getTotalPrice() const { return totalPrice; }
BookingStatus Booking::getStatus() const { return status; }

void Booking::calculateTotalPrice() {
    totalPrice = show->getPrice() * seatNumbers.size();
}

void Booking::setStatus(BookingStatus status) {
    this->status = status;
}

BookingSystem::BookingSystem() : bookingIdCounter(1) {}
BookingSystem::~BookingSystem() {}

void BookingSystem::addMovie(Movie* movie) {
    movies_map[movie->getId()] = movie;
}

void BookingSystem::addTheater(Theater* theater) {
    theaters_map[theater->getId()] = theater;
}

Booking* BookingSystem::createBooking(std::string showId, std::string userName, std::string userPhone, std::vector<int>& seats) {
    Show* show = findShow(showId);
    if (!show || show->getStatus() != ShowStatus::SCHEDULED) {
        return nullptr;
    }

    // check if seats are available
    for (int seatNumber : seats) {
        if (!show->isSeatAvailable(seatNumber)) {
            return nullptr;
        }
    }

    // book seats
    for (int seatNumber : seats) {
        show->bookSeat(seatNumber);
    }

    Booking* booking = new Booking(generateBookingId(), show, userName, userPhone, seats);
    bookings_map[booking->getId()] = booking;
    return booking;
}

void BookingSystem::displayShows(std::string movieId) {
    Movie* movie = movies_map[movieId];
    std::cout << "\nShows for " << movie->getTitle() << ":\n" << std::endl;
    for (auto it = theaters_map.begin(); it != theaters_map.end(); it++) {
        Theater* theater = it->second;
        for (const auto& show :  theater->getShows()) {
            if ((show->getStatus() == ShowStatus::SCHEDULED) && (show->getMovie()->getId() == movieId)) {
                show->displayInfo();
                std::cout << "================" << std::endl;
            }
        }
    }
}

Show* BookingSystem::findShow(const std::string& showId) {
    for (auto it = theaters_map.begin(); it != theaters_map.end(); it++) {
        Theater* theater = it->second;
        for (const auto& show :  theater->getShows()) {
            if (show->getId() == showId) {
                return show;
            }
        }
    }
    return nullptr;
}

std::string BookingSystem::generateBookingId() {
    return "B" + std::to_string(bookingIdCounter++);
}

int main() {
    BookingSystem system;

    Movie* movie1 = new Movie("M1", "Movie 1", "A sci-fi movie", MovieGenre::SCIFI, 120, "English");
    Movie* movie2 = new Movie("M2", "Movie 2", "A comedy movie", MovieGenre::COMEDY, 120, "English");
    system.addMovie(movie1);
    system.addMovie(movie2);

    Theater* theater1 = new Theater("T1", "Theater 1", "Location 1", 100);
    Theater* theater2 = new Theater("T2", "Theater 2", "Location 2", 200);
    system.addTheater(theater1);
    system.addTheater(theater2);

    Show* show1 = new Show("S1", movie1, "2025-03-15", "18:00", 12.00, 100);
    Show* show2 = new Show("S2", movie1, "2025-03-15", "21:00", 12.00, 100);
    Show* show3 = new Show("S3", movie2, "2025-03-15", "15:00", 12.00, 100);
    theater1->addShow(show1);
    theater1->addShow(show2);
    theater1->addShow(show3);

    system.displayShows("M1");

    // Create a booking
    std::vector<int> seats = {1, 2, 3};
    Booking* booking = system.createBooking("S1", "User 1", "XXXX1234", seats);
    if (booking) {
        std::cout << "\nBooking created successfully." << std::endl;
    }
    return 0;
}
