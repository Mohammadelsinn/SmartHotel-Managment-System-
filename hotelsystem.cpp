#include "hotelsystem.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <filesystem>
#include <limits>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

/* ================================================================
   CONSTRUCTOR
   ================================================================ */
hotelsystem::hotelsystem() {
    loadData();
}

/* ================================================================
   DATE CONVERTER
   ================================================================ */
time_t toDate(const string& d) {
    tm t = {};
    t.tm_year = stoi(d.substr(0, 4)) - 1900;
    t.tm_mon  = stoi(d.substr(5, 2)) - 1;
    t.tm_mday = stoi(d.substr(8, 2));
    return mktime(&t);
}

/* ================================================================
   LOAD DATA
   ================================================================ */
void hotelsystem::loadData() {

    // Load customers
    ifstream cFile("customers.json");
    if (cFile) {
        json j; cFile >> j;
        for (auto &item : j) {
            customerList.push_back(customers(
                item["id"], item["name"], item["phone"], item["email"]
            ));
        }
    }

    // Load rooms
    ifstream rFile("rooms.json");
    if (rFile) {
        json j; rFile >> j;
        for (auto &item : j) {
            roomList.push_back(rooms(
                item["id"], item["type"], item["price"]
            ));
        }
    }

    // Load reservations
    ifstream resFile("reservations.json");
    if (resFile) {
        json j; resFile >> j;
        for (auto &item : j) {
            reservationList.push_back(reservations(
                item["id"], item["customerId"], item["roomId"],
                item["checkIn"], item["checkOut"]
            ));
        }
    }
}

/* ================================================================
   SAVE FUNCTIONS
   ================================================================ */
void hotelsystem::saveCustomers() {
    json j;
    for (auto &c : customerList) {
        j.push_back({
            {"id", c.getId()},
            {"name", c.getName()},
            {"phone", c.getPhone()},
            {"email", c.getEmail()}
        });
    }
    ofstream f("customers.json");
    f << j.dump(4);
}

void hotelsystem::saveReservations() {
    json j;
    for (auto &r : reservationList) {
        j.push_back({
            {"id", r.getId()},
            {"customerId", r.getCustomerId()},
            {"roomId", r.getRoomId()},
            {"checkIn", r.getCheckIn()},
            {"checkOut", r.getCheckOut()}
        });
    }
    ofstream f("reservations.json");
    f << j.dump(4);
}

/* ================================================================
   ADD CUSTOMER
   ================================================================ */
void hotelsystem::addCustomer() {
    int id;
    string name, phone, email;

    cout << "Customer ID: ";
    cin >> id;

    cin.ignore();
    cout << "Name: ";
    getline(cin, name);

    cout << "Phone: ";
    getline(cin, phone);

    cout << "Email: ";
    getline(cin, email);

    customerList.push_back(customers(id, name, phone, email));
    saveCustomers();

    cout << "✔ Customer added successfully!\n";
}

/* ================================================================
   DELETE RESERVATION
   ================================================================ */
void hotelsystem::deleteReservation() {
    int id;
    cout << "Reservation ID to delete: ";
    cin >> id;

    for (int i = 0; i < reservationList.size(); i++) {
        if (reservationList[i].getId() == id) {
            reservationList.erase(reservationList.begin() + i);
            saveReservations();
            cout << "✔ Reservation deleted.\n";
            return;
        }
    }
    cout << "❌ Reservation not found!\n";
}

/* ================================================================
   PRINT INVOICE
   ================================================================ */
void hotelsystem::printInvoice(int resId) {
    cout << "\n========== INVOICE ==========\n";

    for (auto &r : reservationList) {
        if (r.getId() == resId) {

            // find room
            rooms selectedRoom;
            for (auto &rm : roomList)
                if (rm.getId() == r.getRoomId()) selectedRoom = rm;

            // days
            int days = difftime(
                toDate(r.getCheckOut()),
                toDate(r.getCheckIn())
            ) / (60 * 60 * 24);

            double total = days * selectedRoom.getPrice();

            cout << "Reservation ID: " << r.getId() << "\n";
            cout << "Customer ID: " << r.getCustomerId() << "\n";
            cout << "Room ID: " << r.getRoomId() << "\n";
            cout << "Room Type: " << selectedRoom.getType() << "\n";
            cout << "Nights: " << days << "\n";
            cout << "Total Price: $" << total << "\n";
            break;
        }
    }

    cout << "Thank you for using our hotel!\n";
    cout << "=============================\n";
}

/* ================================================================
   ADD RESERVATION (AUTO ALLOCATION + INVOICE)
   ================================================================ */
void hotelsystem::addReservation() {
    int cus;
    string in, out;

    int newId = 1;
    for (auto &r : reservationList)
        if (r.getId() >= newId) newId = r.getId() + 1;

    cout << "\n✔ Auto-generated Reservation ID: " << newId << "\n";

    cout << "Customer ID: ";
    cin >> cus;

    cout << "Check-In (YYYY-MM-DD): ";
    cin >> in;

    cout << "Check-Out (YYYY-MM-DD): ";
    cin >> out;

    int days = difftime(toDate(out), toDate(in)) / (60*60*24);
    if (days <= 0) {
        cout << "❌ Invalid range.\n";
        return;
    }

    int assignedRoom = -1;
    double price = 0;
    string type;

    for (auto &rm : roomList) {

        bool freeRoom = true;

        for (auto &r : reservationList) {
            if (r.getRoomId() == rm.getId()) {

                if (!(toDate(out) <= toDate(r.getCheckIn()) ||
                      toDate(in) >= toDate(r.getCheckOut()))) {
                    freeRoom = false;
                    break;
                }
            }
        }

        if (freeRoom) {
            assignedRoom = rm.getId();
            type = rm.getType();
            price = rm.getPrice();
            break;
        }
    }

    if (assignedRoom == -1) {
        cout << "❌ No rooms available.\n";
        return;
    }

    reservationList.push_back(reservations(newId, cus, assignedRoom, in, out));
    saveReservations();

    cout << "\n✔ Reservation added!\n";
    cout << "Room: " << assignedRoom << " (" << type << ")\n";

    // PRINT INVOICE
    printInvoice(newId);
}

/* ================================================================
   BACKUP SYSTEM
   ================================================================ */
void hotelsystem::createBackup() {
    fs::create_directories("backup");

    fs::copy_file("customers.json", "backup/customers_backup.json", fs::copy_options::overwrite_existing);
    fs::copy_file("rooms.json", "backup/rooms_backup.json", fs::copy_options::overwrite_existing);
    fs::copy_file("reservations.json", "backup/reservations_backup.json", fs::copy_options::overwrite_existing);

    cout << "\n✔ Backup created!\n";
}

void hotelsystem::restoreBackup() {
    if (!fs::exists("backup/customers_backup.json")) {
        cout << "❌ No backup found.\n";
        return;
    }

    fs::copy_file("backup/customers_backup.json", "customers.json", fs::copy_options::overwrite_existing);
    fs::copy_file("backup/rooms_backup.json", "rooms.json", fs::copy_options::overwrite_existing);
    fs::copy_file("backup/reservations_backup.json", "reservations.json", fs::copy_options::overwrite_existing);

    cout << "✔ Backup restored!\n";

    customerList.clear();
    roomList.clear();
    reservationList.clear();
    loadData();
}

/* ================================================================
   ADMIN PANEL (FIXED INPUT)
   ================================================================ */
void hotelsystem::adminPanel() {
    string user, pass;

    cout << "\n===== ADMIN LOGIN =====\n";
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    if (user != "admin" || pass != "1234") {
        cout << "❌ Wrong login.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int choice;

    while (true) {
        cout << "\n===== ADMIN PANEL =====\n";
        cout << "1. View Reports\n";
        cout << "2. Create Backup\n";
        cout << "3. Restore Backup\n";
        cout << "4. Logout\n";
        cout << "Choice: ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            cout << "\n===== REPORTS =====\n";
            cout << "Customers: " << customerList.size() << "\n";
            cout << "Rooms: " << roomList.size() << "\n";
            cout << "Reservations: " << reservationList.size() << "\n";
        }
        else if (choice == 2) {
            createBackup();
        }
        else if (choice == 3) {
            restoreBackup();
        }
        else if (choice == 4) {
            cout << "\nThank you, Admin!\n";
            return;
        }
        else {
            cout << "Invalid.\n";
        }
    }
}

/* ================================================================
   LIST FUNCTIONS
   ================================================================ */
void hotelsystem::listCustomers() {
    for (auto &c : customerList) c.display();
}

void hotelsystem::listRooms() {
    for (auto &r : roomList) r.display();
}

void hotelsystem::listReservations() {
    for (auto &r : reservationList) r.display();
}

/* ================================================================
   CUSTOMER FUNCTIONS
   ================================================================ */
void hotelsystem::viewCustomer() {
    int id;
    cout << "Customer ID: ";
    cin >> id;

    for (auto &c : customerList) {
        if (c.getId() == id) {
            c.display();
            return;
        }
    }
    cout << "❌ Not found!\n";
}

void hotelsystem::editCustomer() {
    int id;
    cout << "Customer ID: ";
    cin >> id;

    for (auto &c : customerList) {
        if (c.getId() == id) {

            string name, phone, email;
            cin.ignore();

            cout << "New Name (" << c.getName() << "): ";
            getline(cin, name);

            cout << "New Phone (" << c.getPhone() << "): ";
            getline(cin, phone);

            cout << "New Email (" << c.getEmail() << "): ";
            getline(cin, email);

            c.setName(name);
            c.setPhone(phone);
            c.setEmail(email);
            saveCustomers();

            cout << "✔ Updated!\n";
            return;
        }
    }
    cout << "❌ Not found!\n";
}

void hotelsystem::deleteCustomer() {
    int id;
    cout << "Customer ID: ";
    cin >> id;

    for (int i = 0; i < customerList.size(); i++) {
        if (customerList[i].getId() == id) {
            customerList.erase(customerList.begin() + i);
            saveCustomers();
            cout << "✔ Deleted!\n";
            return;
        }
    }
    cout << "❌ Not found!\n";
}

/* ================================================================
   MAIN MENU (NO FREEZE)
   ================================================================ */
void hotelsystem::menu() {
    int choice;

    while (true) {
        cout << "\n===== SMART HOTEL MENU =====\n";
        cout << "1. Add Customer\n";
        cout << "2. Add Reservation\n";
        cout << "3. Delete Reservation\n";
        cout << "4. List Customers\n";
        cout << "5. View Customer\n";
        cout << "6. Edit Customer\n";
        cout << "7. Delete Customer\n";
        cout << "8. List Rooms\n";
        cout << "9. List Reservations\n";
        cout << "10. Admin Panel\n";
        cout << "0. Exit\n";
        cout << "Choose: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cout << "\n";

        switch (choice) {
            case 1: addCustomer(); break;
            case 2: addReservation(); break;
            case 3: deleteReservation(); break;
            case 4: listCustomers(); break;
            case 5: viewCustomer(); break;
            case 6: editCustomer(); break;
            case 7: deleteCustomer(); break;
            case 8: listRooms(); break;
            case 9: listReservations(); break;
            case 10: adminPanel(); break;
            case 0:
                cout << "Goodbye!\n";
                return;
            default:
                cout << "❌ Invalid!\n";
        }

        cout << "Press ENTER...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}
