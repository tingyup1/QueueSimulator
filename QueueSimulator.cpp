// QueueSimulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

struct Customer {
    string name;
    int arrival_hour;
    int arrival_minute;
    int checkout_time;

    int getArrivalTimeInMinutes() const {
        return arrival_hour * 60 + arrival_minute;
    }
};


int generateRandomInt(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

void readCustomersFromFile(queue<Customer>& customerQueue, const string& filename) {
    ifstream inputFile("C:/Users/pty99/Downloads/customer.txt");
    string line;

    if (!inputFile.is_open()) {
        cout << "Failed to open " << "customer file" << "!" << endl;
        return;
    }

    int customerId = 1; 
    // Read each line (customer name) from the file
    while (getline(inputFile, line)) {
        Customer customer;
        customer.name = line;  // Set the customer name from the file

        customer.arrival_hour = generateRandomInt(9, 22);  
        customer.arrival_minute = generateRandomInt(0, 59); 
        customer.checkout_time = generateRandomInt(3, 20);

        // Push the generated customer into the queue
        customerQueue.push(customer);
        customerId++;
    }

    inputFile.close();
    cout << "Finished reading customers from file." << endl;
}

// Function to simulate the checkout process for customers in the queue
void processQueue(queue<Customer>& customerQueue, vector<Customer>& completedCustomers) {
    while (!customerQueue.empty()) {
        Customer customer = customerQueue.front(); // Get the first customer from the queue
        customerQueue.pop();  // Remove this customer from the queue (they're now being processed)
        completedCustomers.push_back(customer);  // Add the customer to completed list
    }
    cout << "Finished processing queue." << endl;
}


void writeCompletedCustomersToFile(const vector<Customer>& completedCustomers) {
    ofstream outputFile("completed_customers.txt");

    if (!outputFile.is_open()) {
        cout << "Failed to open completed_customers.txt for writing!" << endl;
        return;
    }

    for (const auto& customer : completedCustomers) {
        outputFile << customer.name << " "
            << customer.arrival_hour << ":" << customer.arrival_minute << " "
            << customer.checkout_time << "min" << "\n";
    }

    outputFile.close();
    cout << "Finished writing completed customers." << endl;
}




int main() {
    queue<Customer> customerQueue;
    vector<Customer> completedCustomers;

    // Read customer data from file and generate random arrival/check-out times
    readCustomersFromFile(customerQueue, "customer.txt");

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
