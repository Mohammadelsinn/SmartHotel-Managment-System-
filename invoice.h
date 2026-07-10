#ifndef INVOICE_H
#define INVOICE_H

#include <iostream>

class invoice {
private:
    int id;
    int reservationId;
    int customerId;
    int roomId;
    int nights;
    double pricePerNight;
    double totalAmount;

public:
    invoice(int id = 0,
            int reservationId = 0,
            int customerId = 0,
            int roomId = 0,
            int nights = 1,
            double pricePerNight = 0.0)
        : id(id), reservationId(reservationId), customerId(customerId),
          roomId(roomId), nights(nights), pricePerNight(pricePerNight),
          totalAmount(nights * pricePerNight) {}

    ~invoice() {}

    int getId() const { return id; }
    int getReservationId() const { return reservationId; }
    int getCustomerId() const { return customerId; }
    int getRoomId() const { return roomId; }
    int getNights() const { return nights; }
    double getPricePerNight() const { return pricePerNight; }
    double getTotalAmount() const { return totalAmount; }

    void display() const {
        std::cout << "\n===== INVOICE =====\n";
        std::cout << "Invoice ID: " << id << "\n";
        std::cout << "Reservation ID: " << reservationId << "\n";
        std::cout << "Customer ID: " << customerId << "\n";
        std::cout << "Room ID: " << roomId << "\n";
        std::cout << "Nights: " << nights << "\n";
        std::cout << "Price per Night: " << pricePerNight << "\n";
        std::cout << "Total Amount: " << totalAmount << "\n";
    }
};

#endif
