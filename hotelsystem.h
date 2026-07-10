#ifndef HOTELSYSTEM_H
#define HOTELSYSTEM_H

#include <vector>
#include <string>
#include "customers.h"
#include "rooms.h"
#include "reservations.h"

class hotelsystem {
private:
    std::vector<customers> customerList;
    std::vector<rooms> roomList;
    std::vector<reservations> reservationList;

public:
    hotelsystem();

    void loadData();
    void saveCustomers();
    void saveReservations();

    void addCustomer();
    void addReservation();
    void deleteReservation();

    void listCustomers();
    void listRooms();
    void listReservations();

    void viewCustomer();
    void editCustomer();
    void deleteCustomer();

    void adminPanel();
    void createBackup();
    void restoreBackup();

    void printInvoice(int resId);      // ✅ ADD THIS

    void menu();
};

#endif
