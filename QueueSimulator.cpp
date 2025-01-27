// QueueSimulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
using namespace std;

struct Customer {
    std::string name;
    int arrival_hour;
    int arrival_minute;
    int checkout_time;

    // caculate the customer arrived time
    int getArrivalTimeInMinutes() const {
        return arrival_hour * 60 + arrival_minute;
    }
};

// read the info about customers from customer.txt
void readCustomersFromFile(queue<Customer>& customerQueue) {
    ifstream inputFile("C:/Users/pty99/Downloads/customer.txt");
    string line;

    if (!inputFile.is_open()) {
        cout << "Failed to open customers.txt!" << endl;
        return;
    }

    // Read the file line by line
    while (getline(inputFile, line)) {
        cout << "Reading line: " << line << endl;  

        stringstream ss(line); // Create a stringstream from the current line
        Customer customer; // Declare a Customer variable to store the data
        char colon;// This variable is used to handle the colon in the time (e.g., 10:15)

        string firstName, lastName;
        getline(ss, firstName, ' '); 
        getline(ss, lastName, ' '); 
        customer.name = firstName + " " + lastName;  

        // read other part
        string remaining;
        getline(ss, remaining); //read other part which include arrive time and checkout time

        //parse the remaining part
        stringstream remainingStream(remaining);  // Create a new stringstream to parse the remaining part
        remainingStream >> customer.arrival_hour >> colon >> customer.arrival_minute >> customer.checkout_time;


        // Check if the stringstream failed to parse the remaining data
        if (ss.fail()) {
            cout << "Error reading line: " << line << endl;
            continue;
        }

        // Push the parsed customer into the queue
        customerQueue.push(customer);
    }
    cout << "Finished reading customers." << endl;
}

// Function to write completed customers' data to a file
void writeCompletedCustomersToFile(const vector<Customer>& completedCustomers) {
    ofstream outputFile("completed_customers.txt");

    // Check if the file opened successfully
    if (!outputFile.is_open()) {
        cout << "Failed to open completed_customers.txt for writing!" << endl;
        return;
    }

    // Loop through each completed customer in the vector
    for (const auto& customer : completedCustomers) {
        // Write each customer's data (name, arrival time, checkout time) to the file
        outputFile << customer.name << " "
            << customer.arrival_hour << ":" << customer.arrival_minute << " "
            << customer.checkout_time << "\n";
    }

    // After writing, close the file to ensure all data is saved correctly
    outputFile.close();
    cout << "Finished writing completed customers." << endl;
}

// Function to simulate the checkout process for customers in the queue
void processQueue(queue<Customer>& customerQueue, vector<Customer>& completedCustomers) {
    // Loop until there are no more customers in the queue
    while (!customerQueue.empty()) {

        // Get the first customer from the queue (the one who will check out next)
        Customer customer = customerQueue.front();

        // Remove this customer from the queue (they're now being processed)
        customerQueue.pop(); 

        // Simulate checkout
        completedCustomers.push_back(customer);  // Add the customer to the completed list after checkout
    }

    // Print a message after all customers have been processed
    cout << "Finished processing queue." << endl;
}

int main() {
    queue<Customer> customerQueue;
    vector<Customer> completedCustomers;

    readCustomersFromFile(customerQueue);
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
