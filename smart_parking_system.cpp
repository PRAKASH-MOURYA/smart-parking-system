// smart_parking_system.cpp
#include <iostream>
#include <map>
#include <ctime>
#include <iomanip>
using namespace std;

class Vehicle {
protected:
    string number;
    string type;
    time_t entryTime;

public:
    Vehicle(string num, string typ) {
        number = num;
        type = typ;
        entryTime = time(0);
    }

    virtual double calculateBill(time_t exitTime) = 0;

    void displayDetails() {
        cout << "Vehicle Number: " << number << endl;
        cout << "Vehicle Type: " << type << endl;
        cout << "Entry Time: " << ctime(&entryTime);
    }

    string getNumber() { return number; }
    time_t getEntryTime() { return entryTime; }
    string getType() { return type; }
};

class Car : public Vehicle {
public:
    Car(string num) : Vehicle(num, "Car") {}
    double calculateBill(time_t exitTime) override {
        double hours = difftime(exitTime, entryTime) / 3600.0;
        return hours * 50;
    }
};

class Bike : public Vehicle {
public:
    Bike(string num) : Vehicle(num, "Bike") {}
    double calculateBill(time_t exitTime) override {
        double hours = difftime(exitTime, entryTime) / 3600.0;
        return hours * 20;
    }
};

class ParkingLot {
private:
    int totalSlots;
    int availableSlots;
    map<string, Vehicle*> parkedVehicles;

public:
    ParkingLot(int slots) {
        totalSlots = slots;
        availableSlots = slots;
    }

    bool parkVehicle(Vehicle* v) {
        if (availableSlots == 0) {
            cout << "No parking slots available!" << endl;
            return false;
        }
        if (parkedVehicles.find(v->getNumber()) != parkedVehicles.end()) {
            cout << "Vehicle already parked!" << endl;
            return false;
        }

        parkedVehicles[v->getNumber()] = v;
        availableSlots--;
        cout << "Vehicle parked successfully." << endl;
        return true;
    }

    bool exitVehicle(string number) {
        if (parkedVehicles.find(number) == parkedVehicles.end()) {
            cout << "Vehicle not found!" << endl;
            return false;
        }

        Vehicle* v = parkedVehicles[number];
        time_t exitTime = time(0);
        double bill = v->calculateBill(exitTime);
        cout << "\n--- Exit Details ---\n";
        v->displayDetails();
        cout << "Exit Time: " << ctime(&exitTime);
        cout << fixed << setprecision(2);
        cout << "Total Bill: Rs. " << bill << endl;

        parkedVehicles.erase(number);
        delete v;
        availableSlots++;
        return true;
    }

    void displayStatus() {
        cout << "\n--- Parking Lot Status ---" << endl;
        cout << "Total Slots: " << totalSlots << endl;
        cout << "Available Slots: " << availableSlots << endl;
        cout << "Occupied Slots: " << (totalSlots - availableSlots) << endl;

        for (auto it : parkedVehicles) {
            cout << "\nVehicle Number: " << it.first;
            cout << "\nType: " << it.second->getType() << endl;
        }
    }
};

int main() {
    ParkingLot lot(5);
    int choice;

    while (true) {
        cout << "\n--- Smart Parking System ---\n";
        cout << "1. Park Vehicle\n2. Exit Vehicle\n3. Display Status\n4. Exit Program\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string num, type;
            cout << "Enter vehicle number: ";
            cin >> num;
            cout << "Enter type (Car/Bike): ";
            cin >> type;

            Vehicle* v = nullptr;
            if (type == "Car" || type == "car") {
                v = new Car(num);
            } else if (type == "Bike" || type == "bike") {
                v = new Bike(num);
            } else {
                cout << "Invalid type!" << endl;
                continue;
            }

            lot.parkVehicle(v);
        }
        else if (choice == 2) {
            string num;
            cout << "Enter vehicle number to exit: ";
            cin >> num;
            lot.exitVehicle(num);
        }
        else if (choice == 3) {
            lot.displayStatus();
        }
        else if (choice == 4) {
            cout << "Exiting system. Goodbye!" << endl;
            break;
        }
        else {
            cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}