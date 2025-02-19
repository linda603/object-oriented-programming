import datetime
import math

class Vehicle:
    def __init__(self, spot_size):
        self.spot_size = spot_size
    
    def get_spot_size(self):
        return self.spot_size

class Car(Vehicle):
    def __init__(self):
        super().__init__(1)

class Limo(Vehicle):
    def __init__(self):
        super().__init__(2)

class Truck(Vehicle):
    def __init__(self):
        super().__init__(3)

class Driver:
    def __init__(self, id, vehicle):
        self.id = id
        self.vehicle = vehicle
        self.payment_due = 0
    
    def get_id(self):
        return self.id

    def get_vehicle(self):
        return self.vehicle

    def charge(self, amount):
        self.payment_due += amount

class ParkingFloor:
    def __init__(self, number_of_spots):
        self.spots = [0] * number_of_spots
        self.vehicle_map = {}

    def park_vehicle(self, vehicle):
        vehicle_spot_size = vehicle.get_spot_size()
        l = r = 0

        while r < len(self.spots):
            if self.spots[r] != 0:
                l = r + 1
            # able to park this vehicle at sport [l, r]
            if r - l + 1 == vehicle_spot_size:
                for k in range(l, r + 1):
                    self.spots[k] = 1
                self.vehicle_map[vehicle] = [l, r]
                return True
            r += 1
        return False
    
    def remove_vehicle(self, vehicle):
        l, r = self.vehicle_map[vehicle]
        for k in range(l, r + 1):
            self.spots[k] = 0

        del self.vehicle_map[vehicle]
    
    def get_parking_spots(self):
        return self.spots

    def get_vehicle_spots(self, vehicle):
        return self.vehicle_map.get(vehicle, [])

class ParkingGarage:
    def __init__(self, number_of_floors, spots_per_floor):
        self.parking_floors = [ParkingFloor(spots_per_floor) for _ in range(number_of_floors)]

    def park_vehicle(self, vehicle):
        for floor in self.parking_floors:
            if floor.park_vehicle(vehicle):
                return True
        return False

    def remove_vehicle(self, vehicle):
        for floor in self.parking_floors:
            if floor.get_vehicle_spots(vehicle):
                floor.remove_vehicle(vehicle)
                return True
        return False

class ParkingSystem:
    def __init__(self, parking_garage, hourly_rate):
        self.parking_garage = parking_garage
        self.hourly_rate = hourly_rate
        self.checkedin_time = {} # map driver id: time

    def park_vehicle(self, driver):
        curr_hour = datetime.datetime.now().hour
        is_parked = self.parking_garage.park_vehicle(driver.get_vehicle())
        if is_parked:
            self.checkedin_time[driver.get_id()] = curr_hour
        return is_parked
    
    def remove_vehicle(self, driver):
        if driver.get_id() not in self.checkedin_time:
            return False
        curr_hour = datetime.datetime.now().hour
        checkedin_hour = self.checkedin_time[driver.get_id()]
        driver.charge((curr_hour - checkedin_hour) * self.hourly_rate)

        del self.checkedin_time[driver.get_id()]
        return self.parking_garage.remove_vehicle(driver.get_vehicle())

parkingGarage = ParkingGarage(2, 3)
parkingSystem = ParkingSystem(parkingGarage, 1)

driver1 = Driver(1, Car())
driver2 = Driver(2, Limo())
driver3 = Driver(3, Truck())
driver4 = Driver(4, Truck())

print(parkingSystem.park_vehicle(driver1))
print(parkingSystem.park_vehicle(driver2))
print(parkingSystem.park_vehicle(driver3))
print(parkingSystem.park_vehicle(driver4))

print(parkingSystem.remove_vehicle(driver1))
print(parkingSystem.remove_vehicle(driver4))