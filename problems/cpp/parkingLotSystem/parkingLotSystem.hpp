#ifndef PARKINGLOTSYSTEM_HPP
#define PARKINGLOTSYSTEM_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>


enum class VehicleType {
    MOTORCYCLE,
    CAR,
    TRUCK,
};

enum class SpotType {
    COMPACT,
    REGULAR,
    LARGE
};

class Vehicle {
private:
    std::string licensePlate;
    VehicleType type;
    std::string color;

public:
    Vehicle(std::string licensePlate, VehicleType type, std::string color);

    std::string getLicensePlate() const;
    VehicleType getType() const;
    std::string getColor() const;
};

class Motorcycle : public Vehicle {
public:
    Motorcycle(std::string licensePlate, std::string color);
};

class Car : public Vehicle {
public:
    Car(std::string licensePlate, std::string color);
};

class Truck : public Vehicle {
public:
    Truck(std::string licensePlate, std::string color);
};

class ParkingSpot {
private:
    int spotId;
    SpotType type;
    Vehicle* vehicle;
    bool available;

public:
    ParkingSpot(int spotId, SpotType type);

    int getSpotId() const;
    SpotType getType() const;
    Vehicle* getVehicle() const;
    bool isAvailable() const;

    bool canFitVehicle(Vehicle* vehicle);
    bool parkVehicle(Vehicle* vehicle);
    Vehicle* removeVehicle();
};

class ParkingLot {
private:
    std::vector<ParkingSpot*> spots;
    std::unordered_map<std::string, ParkingSpot*> occupiedSpots;
    int capacity;
    int availableSpots; 

public:
    ParkingLot(int numCompact, int numRegular, int numLarge);
    ~ParkingLot();

    int getCapacity() const;
    int getAvailableSpots() const;
    bool parkVehicle(Vehicle* vehicle);
    Vehicle* removeVehicle(const std::string& licensePlate);
    ParkingSpot* findVehicle(const std::string& licensePlate);

private:
    ParkingSpot* findAvailableSpot(Vehicle* vehicle);
};



#endif