#include "parkingLotSystem.hpp"

Vehicle::Vehicle(std::string licensePlate, VehicleType type, std::string color)
    : licensePlate(licensePlate), type(type), color(color) {}

std::string Vehicle::getLicensePlate() const { return licensePlate; }
VehicleType Vehicle::getType() const { return type; }
std::string Vehicle::getColor() const { return color; }

Motorcycle::Motorcycle(std::string licensePlate, std::string color)
    : Vehicle(licensePlate, VehicleType::MOTORCYCLE, color) {}

Car::Car(std::string licensePlate, std::string color)
    : Vehicle(licensePlate, VehicleType::CAR, color) {}

Truck::Truck(std::string licensePlate, std::string color)
    : Vehicle(licensePlate, VehicleType::TRUCK, color) {}

ParkingSpot::ParkingSpot(int spotId, SpotType type)
    : spotId(spotId), type(type), vehicle(nullptr), available(true) {}

int ParkingSpot::getSpotId() const { return spotId; }
SpotType ParkingSpot::getType() const { return type; }
Vehicle* ParkingSpot::getVehicle() const { return vehicle; }
bool ParkingSpot::isAvailable() const { return available; }

bool ParkingSpot::canFitVehicle(Vehicle* vehicle) {
    if (!vehicle) return false;

    if (vehicle->getType() == VehicleType::MOTORCYCLE) {
        return type == SpotType::COMPACT;
    }
    else if (vehicle->getType() == VehicleType::CAR) {
        return type != SpotType::COMPACT;
    }
    else {
        return type == SpotType::LARGE;
    }
    return false;
}

bool ParkingSpot::parkVehicle(Vehicle* vehicle) {
    if (!vehicle or !available) return false;

    available = false;
    this->vehicle = vehicle;
    return true;
}

Vehicle* ParkingSpot::removeVehicle() {
    if (!vehicle) return nullptr;

    Vehicle* removedVehicle = vehicle;
    this->vehicle = nullptr;
    available = true;
    return removedVehicle;
}

ParkingLot::ParkingLot(int numCompact, int numRegular, int numLarge)
    : capacity(numCompact + numRegular + numLarge), availableSpots(capacity) {
    int id = 1;

    for (int i = 0; i < numCompact; i++) {
        spots.push_back(new ParkingSpot(id++, SpotType::COMPACT));
    }

    for (int i = 0; i < numRegular; i++) {
        spots.push_back(new ParkingSpot(id++, SpotType::REGULAR));
    }

    for (int i = 0; i < numCompact; i++) {
        spots.push_back(new ParkingSpot(id++, SpotType::LARGE));
    }
}

ParkingLot::~ParkingLot() {
    for (auto spot : spots) {
        delete spot;
    }
}

int ParkingLot::getCapacity() const { return capacity; }
int ParkingLot::getAvailableSpots() const { return availableSpots; }

bool ParkingLot::parkVehicle(Vehicle* vehicle) {
    if (!vehicle) return false;

    ParkingSpot* spot = findAvailableSpot(vehicle);
    if (!spot) return false;

    if (spot->parkVehicle(vehicle)) {
        occupiedSpots[vehicle->getLicensePlate()] = spot;
        availableSpots--;
        return true;
    }
    return false;
}

Vehicle* ParkingLot::removeVehicle(const std::string& licensePlate) {
    auto it = occupiedSpots.find(licensePlate);
    if (it == occupiedSpots.end()) return nullptr;

    ParkingSpot* spot = it->second;
    Vehicle* vehicle = spot->removeVehicle();
    if (vehicle) {
        occupiedSpots.erase(it);
        availableSpots++;
    }
    return vehicle;
}

ParkingSpot* ParkingLot::findAvailableSpot(Vehicle* vehicle) {
    for (auto spot : spots) {
        if (spot->isAvailable() && spot->canFitVehicle(vehicle)) {
            return spot;
        }
    }
    return nullptr;
}

int main() {
    ParkingLot parkingLot(10, 5, 2);

    // Create vehicles
    Motorcycle* motorcycle = new Motorcycle("M1", "Blue");
    Car* car = new Car("C1", "White");
    Truck* truck = new Truck("T1", "Black");

    // Park vehicles
    if (parkingLot.parkVehicle(motorcycle)) {
        std::cout << "Parked successfully: " << motorcycle->getLicensePlate() << std::endl;
    }
    if (parkingLot.parkVehicle(car)) {
        std::cout << "Parked successfully: " << car->getLicensePlate() << std::endl;
    }
    if (parkingLot.parkVehicle(truck)) {
        std::cout << "Parked successfully: " << truck->getLicensePlate() << std::endl;
    }
    std::cout << parkingLot.getAvailableSpots() << " available spots" << std::endl;
    return 0;
}