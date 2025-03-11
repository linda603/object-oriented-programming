#ifndef MOVIEBOOKINGSYSTEM_HPP
#define MOVIEBOOKINGSYSTEM_HPP

#include <string>
#include <vector>
#include <unordered_map>

enum class MovieGenre {
    ACTION,
    COMEDY,
    DRAMA,
    SCIFI
};

enum class ShowStatus {
    SCHEDULED,
    RUNNING,
    COMPLETED,
    CANCELLED
};

enum class BookingStatus {
    PENDING,
    CONFIRMED,
    CANCELLED
};

class Movie {
private:
    std::string movieId;
    std::string title;
    std::string description;
    MovieGenre genre;
    int durationInMinutes;
    std::string language;
    std::vector<std::string> casts;
    bool active;

public:
    Movie(std::string movieId, std::string title, std::string description,
        MovieGenre genre, int durationInMinutes, std::string language);

    std::string getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    MovieGenre getGenre() const;
    int getDuration() const;
    bool isActive() const;

    void addCast(const std::string& actor);
    void setActive(bool status);
};

class Show {
private:
    std::string showId;
    Movie* movie;
    std::string date;
    std::string startTime;
    double price;
    std::vector<bool> seats;
    ShowStatus status;

public:
    Show(std::string showId, Movie* movie, std::string date,
    std::string startTime, double price, int totalSeats);

    std::string getId() const;
    Movie* getMovie() const;
    std::string getDate() const;
    std::string getStartTime() const;
    double getPrice() const;
    ShowStatus getStatus() const;

    bool isSeatAvailable(int seatNumber) const;
    bool bookSeat(int seatNumber);
    void cancelSeatBooking(int seatNumber);
    void setStatus(ShowStatus status);
    int getAvailableSeats() const;
    void displayInfo() const;
};

class Theater {
private:
    std::string theaterId;
    std::string name;
    std::string location;
    int totalSeats;
    std::vector<Show*> shows;
    bool active;

public:
    Theater(std::string theaterId, std::string name, std::string location,
        int totalSeats);
    ~Theater();

    std::string getId() const;
    std::string getName() const;
    std::string getLocation() const;
    int getTotalSeats() const;
    bool isActive() const;
    const std::vector<Show*>& getShows() const;

    void addShow(Show* show);
    void setStatus(bool status);
};

class Booking {
private:
    std::string bookingId;
    Show* show;
    std::string userName;
    std::string userPhone;
    std::vector<int> seatNumbers;
    double totalPrice;
    BookingStatus status;
    std::string timestamp;

public:
    Booking(std::string bookingId, Show* show, std::string userName, 
        std::string userPhone, std::vector<int> seatNumbers);

    std::string getId() const;
    Show* getShow() const;
    std::string getUserName() const;
    std::string getUserPhone() const;
    const std::vector<int>& getSeatNumbers() const;
    double getTotalPrice() const;
    BookingStatus getStatus() const;

    void calculateTotalPrice();
    void setStatus(BookingStatus status);
};

class BookingSystem {
private:
    std::unordered_map<std::string, Movie*> movies_map;
    std::unordered_map<std::string, Theater*> theaters_map;
    std::unordered_map<std::string, Booking*> bookings_map;
    int bookingIdCounter;

public:
    BookingSystem();
    ~BookingSystem();

    void addMovie(Movie* movie);
    void addTheater(Theater* theater);

    Booking* createBooking(std::string showId, std::string userName, std::string userPhone, std::vector<int>& seats);
    void displayShows(std::string movieId);

private:
    Show* findShow(const std::string& showId);
    //Booking* findBooking(const std::string& bookingId);
    std::string generateBookingId();
};

#endif