// QueueSimulator.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

struct Customer {
    string name;
    string appliance; 
    int arrival_hour;
    int arrival_minute;
    int package_search_time;
    int checkout_time;

    int getArrivalTimeInMinutes() const {
        return arrival_hour * 60 + arrival_minute;
    }

    int getTotalTimeInStore() const {
        return package_search_time + checkout_time; 
    }
};

struct CompareCustomerArrival {
    bool operator()(const Customer& c1, const Customer& c2) {
        return c1.getArrivalTimeInMinutes() > c2.getArrivalTimeInMinutes();
    }
};


int generateRandomInt(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

void readCustomersFromFile(priority_queue<Customer, vector<Customer>, CompareCustomerArrival>& customerQueue, const string& filename) {
    ifstream inputFile("C:/Users/pty99/Downloads/customer.txt");
    string line;

    if (!inputFile.is_open()) {
        cout << "Failed to open " << filename << "!" << endl;
        return;
    }

    int customerId = 1;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        Customer customer;
        getline(ss, customer.name, ','); 
        getline(ss, customer.appliance, ','); 

        customer.arrival_hour = generateRandomInt(9, 22);  
        customer.arrival_minute = generateRandomInt(0, 59);
        customer.package_search_time = generateRandomInt(2, 15);
        customer.checkout_time = generateRandomInt(2, 10); 

        customerQueue.push(customer);
        customerId++;
    }

    inputFile.close();
    cout << "Finished reading customers from file." << endl;
}

void processQueue(priority_queue<Customer, vector<Customer>, CompareCustomerArrival>& customerQueue, vector<Customer>& completedCustomers) {
    while (!customerQueue.empty()) {
        Customer customer = customerQueue.top();
        customerQueue.pop();
        completedCustomers.push_back(customer);
    }
    cout << "Finished processing queue." << endl;
}

void writeCompletedCustomersToFile(const vector<Customer>& completedCustomers) {
    ofstream outputFile("completed_customers.txt");

    if (!outputFile.is_open()) {
        cout << "Failed to open completed_customers.txt for writing!" << endl;
        return;
    }

    //print out the head of list
    outputFile << left << setw(15) << "Name"
        << setw(20) << "Appliance"
        << setw(10) << "Arrival"
        << setw(20) << "Package Search (min)"
        << setw(15) << "Checkout (min)"
        << setw(20) << "Total Time in Store (min)" << "\n";
    outputFile << string(150, '-') << "\n";

    for (const auto& customer : completedCustomers) {
        outputFile << left << setw(15) << customer.name
            << setw(20) << customer.appliance
            << setw(2) << setfill(' ') << customer.arrival_hour << ":"
            << setw(2) << setfill('0') << customer.arrival_minute << "  "
            << setw(20) << setfill(' ') << customer.package_search_time
            << setw(15) << customer.checkout_time
            << setw(20) << customer.getTotalTimeInStore() << "\n";
    }

    outputFile.close();
    cout << "Finished writing completed customers." << endl;
}



int main() {
    priority_queue<Customer, vector<Customer>, CompareCustomerArrival> customerQueue;
    vector<Customer> completedCustomers;

    
    readCustomersFromFile(customerQueue, "C:/Users/pty99/Downloads/customer.txt");

    if (customerQueue.empty()) {
        cout << "No customers in the queue!" << endl;
        return 1;
    }

    
    processQueue(customerQueue, completedCustomers);

    if (completedCustomers.empty()) {
        cout << "No completed customers!" << endl;
        return 1;
    }

    
    writeCompletedCustomersToFile(completedCustomers);

    return 0;
}