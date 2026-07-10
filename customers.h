#ifndef CUSTOMERS_H
#define CUSTOMERS_H

#include <string>
#include <iostream>

class customers {
private:
    int id;
    std::string name;
    std::string phone;
    std::string email;

public:
    // Constructor
    customers(int id = 0,
              const std::string &name = "",
              const std::string &phone = "",
              const std::string &email = "")
        : id(id), name(name), phone(phone), email(email) {}

    // Destructor
    ~customers() {}

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }

    // Setters
    void setName(const std::string &n) { name = n; }
    void setPhone(const std::string &p) { phone = p; }
    void setEmail(const std::string &e) { email = e; }

    // Display customer info
    void display() const {
        std::cout << "Customer ID: " << id
                  << " | Name: " << name
                  << " | Phone: " << phone
                  << " | Email: " << email
                  << std::endl;
    }
};

#endif
