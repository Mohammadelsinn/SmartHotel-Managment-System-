#ifndef RESERVATIONS_H
#define RESERVATIONS_H

#include <string>
#include <iostream>

class reservations {
private:
    int id;
    int customerId;
    int roomId;
    std::string checkIn;
    std::string checkOut;

public:
    reservations(int id = 0,
                 int customerId = 0,
                 int roomId = 0,
                 const std::string &checkIn = "",
                 const std::string &checkOut = "")
        : id(id), customerId(customerId), roomId(roomId),
          checkIn(checkIn), checkOut(checkOut) {}

    ~reservations() {}

    // Getters
    int getId() const { return id; }
    int getCustomerId() const { return customerId; }
    int getRoomId() const { return roomId; }
    std::string getCheckIn() const { return checkIn; }
    std::string getCheckOut() const { return checkOut; }

    // Setters
    void setCustomerId(int cId) { customerId = cId; }
    void setRoomId(int rId) { roomId = rId; }
    void setCheckIn(const std::string &s) { checkIn = s; }
    void setCheckOut(const std::string &s) { checkOut = s; }

    void display() const {
        std::cout << "Reservation ID: " << id
                  << " | Customer ID: " << customerId
                  << " | Room ID: " << roomId
                  << " | Check-In: " << checkIn
                  << " | Check-Out: " << checkOut
                  << std::endl;
    }
};

#endif
