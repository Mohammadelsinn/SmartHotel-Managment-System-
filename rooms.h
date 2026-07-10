#ifndef ROOMS_H
#define ROOMS_H

#include <string>
#include <iostream>

class rooms {
private:
    int id;
    std::string type;   // Single, Double, Suite
    double price;

public:
    rooms(int id = 0,
          const std::string &type = "",
          double price = 0.0)
        : id(id), type(type), price(price) {}

    ~rooms() {}

    // Getters
    int getId() const { return id; }
    std::string getType() const { return type; }
    double getPrice() const { return price; }

    // Setters
    void setType(const std::string &t) { type = t; }
    void setPrice(double p) { price = p; }

    // Display room information
    void display() const {
        std::cout << "Room ID: " << id
                  << " | Type: " << type
                  << " | Price: $" << price
                  << std::endl;
    }
};

#endif
