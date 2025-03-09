#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

class Vehicle {
private:
    int spotSize;

public:
    Vehicle(int spotSize) {
        this->spotSize = spotSize;
    }

    int getSpotSize() {
        return this->spotSize;
    }
};

class Car : public Vehicle {
public:
    Car(): Vehicle(1) {}
};

class Limo : public Vehicle {
public:
    Limo(): Vehicle(2) {}
};

class Truck : public Vehicle {
public:
    Truck(): Vehicle(3) {}
};

class Driver {
private:
    int id = 0;
    Vehicle* vehicle;
    int paymentDue;

public:
    Driver(int id, Vehicle* vehicle) {
        this->id = id;
        this->vehicle = vehicle;
        this->paymentDue = 0;
    }

    int getId() {
        return this->id;
    }

    Vehicle* getVehicle() {
        return this->vehicle;
    }

    void charge(int amount) {
        this->paymentDue += amount;
    }

};

class ParkingFloor {
private:
    vector<int> spots;
    unordered_map<Vehicle*, vector<int> > vehicleMap;

public:
    ParkingFloor(int numberOfSpots) {
        this->spots = vector<int>(numberOfSpots);
        this->vehicleMap = unordered_map<Vehicle*, vector<int> >();
    }

    bool parkVehicle(Vehicle* vehicle) {
        int vehicle_spot_size = vehicle->getSpotSize();
        int l = 0, r = 0;

        while (r < this->spots.size()) {
            if (this->spots[r] != 0) {
                l = r + 1;
            }
            // able to park this vehicle
            if (r - l + 1 == vehicle_spot_size) {
                for (int k = l; k <= r; k++) {
                    this->spots[k] = 1;
                }
                this->vehicleMap[vehicle] = vector<int> {l, r};
                return true;
            }
            r++;
        }
        return false;
    }

    bool removeVehicle(Vehicle* vehicle) {
        vector<int> curr_parked = this->vehicleMap[vehicle];
        for (int i = curr_parked[0]; i <= curr_parked[1]; i++) {
            this->spots[i] = 0;
        }
        this->vehicleMap.erase(vehicle);
    }

    vector<int> getParkingSpots() {
        return this->spots;
    }

    vector<int> getVehicleSpots(Vehicle* vehicle) {
        return this->vehicleMap[vehicle];
    }
};

class ParkingGarage {
private:
    vector<ParkingFloor*> parkingFloors;

public:
    ParkingGarage(int floorCounts, int spotsPerFloor) {
        this->parkingFloors = vector<ParkingFloor*>(floorCounts);
        for (int i = 0; i < floorCounts; i++) {
            this->parkingFloors[i] = new ParkingFloor(spotsPerFloor);
        }
    }

    bool parkVehicle(Vehicle* vehicle) {
        for (ParkingFloor* floor : this->parkingFloors) {
            if (floor->parkVehicle(vehicle)) {
                return true;
            }
        }
        return false;
    }

    bool removeVehicle(Vehicle* vehicle) {
        for (ParkingFloor* floor : this->parkingFloors) {
            if (floor->getVehicleSpots(vehicle).size() != 0) {
                floor->removeVehicle(vehicle);
                return true;
            }
        }
        return false;
    }
};

class ParkingSystem {
private:
    ParkingGarage* parkingGarage;
    int hourlyRate = 0;
    unordered_map<int, int> checkedInTime; // map driver id: checked in time

public:
    ParkingSystem(ParkingGarage* parkingGarage, int hourlyRate) {
        this->parkingGarage = parkingGarage;
        this->hourlyRate = hourlyRate;
        this->checkedInTime = unordered_map<int, int>();
    }

    bool parkVehicle(Driver* driver) {
        int currHour = time(0);
        bool isParked = this->parkingGarage->parkVehicle(driver->getVehicle());
        if (isParked) {
            this->checkedInTime[driver->getId()] = currHour;
            return true;
        }
        return false;
    }

    bool removeVehicle(Driver* driver) {
        if (this->checkedInTime.find(driver->getId()) == this->checkedInTime.end()) {
            cout << "Here" << endl;
            return false;
        }
        int currHour = time(0);
        int checkedInHour = this->checkedInTime[driver->getId()];
        driver->charge((currHour - checkedInHour) * this->hourlyRate);
        this->checkedInTime.erase(driver->getId());
        return this->parkingGarage->removeVehicle(driver->getVehicle());
    }
};

int main() {
    ParkingGarage* parkingGarage = new ParkingGarage(3, 3);
    ParkingSystem* parkingSystem = new ParkingSystem(parkingGarage, 1);

    Driver* driver1 = new Driver(1, new Car());
    Driver* driver2 = new Driver(2, new Limo());
    Driver* driver3 = new Driver(3, new Truck());
    Driver* driver4 = new Driver(4, new Truck());

    cout << parkingSystem->parkVehicle(driver1) << endl;
    cout << parkingSystem->parkVehicle(driver2) << endl;
    cout << parkingSystem->parkVehicle(driver3) << endl;
    cout << parkingSystem->parkVehicle(driver4) << endl;

    cout << parkingSystem->removeVehicle(driver1) << endl;
    cout << parkingSystem->removeVehicle(driver4) << endl;
    
}