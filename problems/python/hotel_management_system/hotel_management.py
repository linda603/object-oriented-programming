from enum import Enum
from threading import Lock
from datetime import datetime, date

class RoomType(Enum):
    SINGLE = "SINGLE"
    DOUBLE = "DOUBLE"
    DELUXE = "DELUXE"
    SUITE = "SUITE"

class RoomStatus(Enum):
    AVAILABLE = "AVAILABLE"
    BOOKED = "BOOKED"
    OCCUPIED = "OCCUPIED"

class BookingStatus(Enum):
    CONFIRMED = "CONFIRMED"
    CANCELED = "CANCELED"

class Room:
    def __init__(self, id, type, price):
        self.id = id
        self.type = type
        self.price = price
        self.status = RoomStatus.AVAILABLE
        self.lock = Lock()
    
    def get_id(self):
        return self.id
    
    def book_room(self):
        with self.lock:
            if self.status == RoomStatus.AVAILABLE:
                self.status = RoomStatus.BOOKED
            else:
                raise ValueError("Room is not available for booking")
    
    def check_in_room(self):
        with self.lock:
            if self.status == RoomStatus.BOOKED:
                self.status = RoomStatus.OCCUPIED
            else:
                raise ValueError("Room is not booked")

    def check_out_room(self):
        with self.lock:
            if self.status == RoomStatus.OCCUPIED:
                self.status = RoomStatus.AVAILABLE
            else:
                raise ValueError("Room is not occupied")

class Guest:
    def __init__(self, guest_id, name, email, phone):
        self.id = guest_id
        self.name = name
        self.email = email
        self.phone = phone
        self.payment_due = 0
    
    def get_id(self):
        return self.id

    def get_name(self):
        return self.name

    def charge(self, amount):
        self.payment_due += amount

class Booking:
    def __init__(self, id, user, room, checkin_date, checkout_date):
        self.id = id
        self.user = user
        self.room = room
        self.status = BookingStatus.CONFIRMED
        self.checkin_date = checkin_date
        self.checkout_date = checkout_date
    
    def get_id(self):
        return self.id

    def cancel(self):
        with self.lock:
            if self.status == BookingStatus.CONFIRMED:
                self.status = BookingStatus.CANCELED
                self.room.check_out_room()
            else:
                raise ValueError("Reservation is not confirmed.")

class ManagementSystem:
    instance = None

    def __new__(cls):
        if not cls.instance:
            cls.instance = super().__new__(cls)
            cls.instance.guests_map = {}
            cls.instance.rooms_map= {}
            cls.instance.bookings_map = {}
            cls.instance.counter = 1
            cls.instance.lock = Lock()
        return cls.instance

    def add_guest(self, guest):
        self.guests_map[guest.get_id()] = guest
    
    def get_guest(self, guest_id):
        return self.guests_map.get(guest_id, None)

    def add_room(self, room):
        self.rooms_map[room.get_id()] = room

    def get_rooms(self, room_id):
        return self.rooms_map.get(room_id, None)

    def book_room(self, guest, room, checkin_date, checkout_date):
        with self.lock:
            if room.status == RoomStatus.AVAILABLE:
                room.book_room()
                amount = room.price * (checkout_date - checkin_date)
                guest.charge(amount)
                booking_id = self.generate_booking_id()
                booking = Booking(booking_id, guest, room, checkin_date, checkout_date)
                self.bookings_map[booking_id] = booking
                return booking
            return None
    
    def cancel_booking(self, booking_id):
        with self.lock:
            booking = self.bookings_map[booking_id]
            if booking:
                booking.cancel()
                del self.bookings_map[booking_id]
    
    def checkin(self, booking_id):
        with self.lock:
            booking = self.bookings_map[booking_id]
            if booking and booking.status == BookingStatus.CONFIRMED:
                booking.room.check_in_room()
            else:
                raise ValueError("Invalid booking")

    def checkout(self, booking_id):
        with self.lock:
            booking = self.bookings_map[booking_id]
            if booking and booking.status == BookingStatus.CONFIRMED:
                booking.room.check_out_room()
                del self.bookings_map[booking_id]
            else:
                raise ValueError("Invalid booking")

    def generate_booking_id(self):
        booking_num = str(self.counter)
        self.counter += 1
        timestamp = datetime.now().strftime("%Y%m%d%H%M%S")
        return booking_num + timestamp

class SystemDemo:
    def run(self):
        management_system = ManagementSystem()

        # Create guests
        guest1 = Guest("G01", "John Smith", "jm@gmail.com", "1234")
        guest2 = Guest("G02", "Bob Joe", "bob@gmail.com", "3456")
        management_system.add_guest(guest1)
        management_system.add_guest(guest2)

        # Create rooms
        room1 = Room("R01", RoomType.DOUBLE, 80)
        room2 = Room("R02", RoomType.DELUXE, 100)
        management_system.add_room(room1)
        management_system.add_room(room2)

        # Book a room
        checkin_date = 10
        checkout_date = 15
        booking1 = management_system.book_room(guest1, room1, checkin_date, checkout_date)
        if booking1:
            print(f"Booking is created: {booking1.get_id()}")

        # Check in
        management_system.checkin(booking1.get_id())
        print(f"Checked in {booking1.get_id()}")

        # Check out
        management_system.checkout(booking1.get_id())
        print(f"Checked out {booking1.get_id()}")

        # Cancel after checking out
        #management_system.cancel_booking(booking1.get_id())

demo = SystemDemo()
demo.run()