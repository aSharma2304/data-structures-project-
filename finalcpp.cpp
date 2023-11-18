#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <bits/stdc++.h>
#include <string>

using namespace std;

unordered_map<string, double> fareCategories = {
    {"0-2 km", 10.0},
    {"3-5 km", 20.0},
    {"6-12 km", 30.0},
    {"13-21 km", 40.0},
    {"22-32 km", 50.0},
    {"33 km and above", 60.0}
};

void printCenteredMessage(const string& message) {
    int width = 80; 
    int messageWidth = message.size();

    if (messageWidth >= width) {
        cout << message << endl;
    } else {
        int padding = (width - messageWidth) / 2;
        cout << string(padding, ' ') << " " << message << " " << string(padding, ' ') << endl;
    }
}


struct Station {
    string name;
    string line;
    unordered_map<string, int> neighbors;

    Station(string name, string line) : name(name), line(line) {}

    Station() : name(""), line("") {}
};


const string adminUsername = "admin";
const string adminPassword = "password";

bool isAdmin = false; 

void changeStationName(unordered_map<string, Station>& stations) {
    if (!isAdmin) {
        cout << "You must be an admin to perform this action." << endl;
        return;
    }

    string oldName, newName;
    cout << "Enter the current name of the station: ";
    cin.ignore();
    getline(cin, oldName);

    
    if (stations.find(oldName) == stations.end()) {
        cout << "Station '" << oldName << "' does not exist." << endl;
        return;
    }

    cout << "Enter the new name for the station: ";
    getline(cin, newName);

    
    if (stations.find(newName) != stations.end()) {
        cout << "Station '" << newName << "' already exists. Please choose a different name." << endl;
        return;
    }

    
    Station station = stations[oldName];
    station.name = newName;
    stations[newName] = station;
    stations.erase(oldName);

    
    for (auto& pair : stations) {
        if (pair.second.neighbors.find(oldName) != pair.second.neighbors.end()) {
            int distance = pair.second.neighbors[oldName];
            pair.second.neighbors.erase(oldName);
            pair.second.neighbors[newName] = distance;
        }
    }

    cout << "Station name has been changed from '" << oldName << "' to '" << newName << "'." << endl;
}




void adminLogin() {
    string enteredUsername, enteredPassword;
    cout << "Enter admin username: ";
    cin >> enteredUsername;
    cout << "Enter admin password: ";
    cin >> enteredPassword;

    if (enteredUsername == adminUsername && enteredPassword == adminPassword) {
        isAdmin = true;
        printCenteredMessage("Admin successfully logged in.");
        cout << "Admin successfully logged in." << endl;
    } else {
        cout << "Invalid admin credentials. Access denied." << endl;
    }
}

// 
void addNewStation(unordered_map<string, Station>& stations) {
    if (!isAdmin) {
        cout << "You must be an admin to perform this action." << endl;
        return;
    }

    string name, line;
    cout << "Enter the name of the new station: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter the line for the new station: ";
    getline(cin, line);

    Station newStation(name, line);
    stations[name] = newStation;

    int numNeighbors;
    cout << "Enter the number of neighboring stations: ";
    cin >> numNeighbors;
    cin.ignore();

    for (int i = 0; i < numNeighbors; i++) {
        string neighborName;
        int distance;
        cout << "Enter the name of neighboring station " << i+1 << ": ";
        getline(cin, neighborName);
        cout << "Enter the distance to " << neighborName << ": ";
        cin >> distance;
        cin.ignore();

        stations[name].neighbors[neighborName] = distance;
        stations[neighborName].neighbors[name] = distance; // 
    }

    cout << "New station '" << name << "' on line '" << line << "' has been added." << endl;
}


// Function to update fares
void updateFares() {
    if (!isAdmin) {
        cout << "You must be an admin to perform this action." << endl;
        return;
    }

    // Display the current fare structure
    cout << "Current Fare Structure:" << endl;
    for (const auto& category : fareCategories) {
        cout << category.first << " : Rs." << category.second << endl;
    }

    // 
    cout << "Enter the new fares for each category." << endl;
    for (auto& category : fareCategories) {
        double newFare;
        cout << category.first << " (current fare: Rs." << category.second << "): Rs.";
        cin >> newFare;
        category.second = newFare; // Update the fare
    }

    // 
    cout << "Updated Fare Structure:" << endl;
    for (const auto& category : fareCategories) {
        cout << category.first << " : $" << category.second << endl;
    }
}


vector<string> findShortestPath(const unordered_map<string, Station>& stations, string startStation, string endStation, int &totalDistance, int &lineSwitches){
    
    // 
    if (stations.find(startStation) == stations.end()) {
        cout << "The station '" << startStation << "' does not exist." << endl;
        return {};
    }
    if (stations.find(endStation) == stations.end()) {
        cout << "The station '" << endStation << "' does not exist." << endl;
        return {};
    }

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

    unordered_map<string, int> distance;
    unordered_map<string, string> parent;
    unordered_map<string, string> line;

    for (const auto& station : stations) {
        distance[station.first] = INT_MAX;
    }
    
    distance[startStation] = 0;
    line[startStation] = stations.at(startStation).line;


    pq.push({0, startStation});
    
    while (!pq.empty()) {
        string currentStation = pq.top().second;
        int currentDistance = pq.top().first;
        pq.pop();

        if (currentStation == endStation) {
            break;
        }

        for (const auto& neighbor : stations.at(currentStation).neighbors) {
            string neighborStation = neighbor.first;
            int neighborDistance = neighbor.second;
            int newDistance = currentDistance + neighborDistance;

            if (newDistance < distance[neighborStation]) {
                distance[neighborStation] = newDistance;
                parent[neighborStation] = currentStation;
                line[neighborStation] = stations.at(neighborStation).line;
                pq.push({newDistance, neighborStation});
            }
        }
    }

    vector<string> shortestPath;
    string currentStation = endStation;
    totalDistance = distance[currentStation];

    while (currentStation != startStation) {
        shortestPath.push_back(currentStation);
        currentStation = parent[currentStation];
    }

    shortestPath.push_back(startStation);
    reverse(shortestPath.begin(), shortestPath.end());
    
    string currentLine = stations.at(startStation).line;
   
    for(const auto& station : shortestPath) {
        if(stations.at(station).line != currentLine) {
            lineSwitches++;
            currentLine = stations.at(station).line;
        }
    }

    return shortestPath;
}


int main() {
    unordered_map<string, Station> stations;

    stations["Rithala"] = Station("Rithala", "Red");
    stations["Rohini East"] = Station("Rohini East", "Red");
    stations["Netaji Subhash Place"] = Station("Netaji Subhash Place", "Red");
    stations["Kashmere Gate"] = Station("Kashmere Gate", "Red");
    stations["Chandni Chowk"] = Station("Chandni Chowk", "Yellow");
    stations["Central Secretariat"] = Station("Central Secretariat", "Yellow");
    stations["AIIMS"] = Station("AIIMS", "Yellow");
    stations["Jor Bagh"] = Station("Jor Bagh", "Yellow");
    stations["Qutub Minar"] = Station("Qutub Minar", "Yellow");
    stations["Dwarka"] = Station("Dwarka", "Blue");
    stations["Indraprastha"] = Station("Indraprastha", "Blue");
    stations["Akshardham"] = Station("Akshardham", "Blue");
    stations["Yamuna Bank"] = Station("Yamuna Bank", "Blue");
    stations["Hauz Khas"] = Station("Hauz Khas", "Aqua");
    stations["Airport"] = Station("Airport", "Aqua");
    stations["Dwarka Sector 21"] = Station("Dwarka Sector 21", "Aqua");
    stations["New Delhi"] = Station("New Delhi", "Aqua");
    

   
 stations["Rithala"].neighbors = {{"Rohini East", 8}};
    stations["Rohini East"].neighbors = {{"Rithala", 8}, {"Netaji Subhash Place", 5}};
    stations["Netaji Subhash Place"].neighbors = {{"Rohini East", 5}, {"Kashmere Gate", 10}};
    stations["Kashmere Gate"].neighbors = {{"Netaji Subhash Place", 10}, {"Chandni Chowk", 2}};
    stations["Chandni Chowk"].neighbors = {{"Kashmere Gate", 2}, {"Central Secretariat", 8}};
    stations["Central Secretariat"].neighbors = {{"Chandni Chowk", 8}, {"AIIMS", 6}};
    stations["AIIMS"].neighbors = {{"Central Secretariat", 6}, {"Jor Bagh", 4}};
    stations["Jor Bagh"].neighbors = {{"AIIMS", 4}, {"Qutub Minar", 12}};
    stations["Qutub Minar"].neighbors = {{"Jor Bagh", 12}, {"Hauz Khas", 8}};
    stations["Hauz Khas"].neighbors = {{"Qutub Minar", 8}, {"Airport", 14}};
    stations["Airport"].neighbors = {{"Hauz Khas", 14}, {"Dwarka Sector 21", 8}};
    stations["Dwarka Sector 21"].neighbors = {{"Airport", 8}, {"Dwarka", 18}};
    stations["Dwarka"].neighbors = {{"Dwarka Sector 21", 18}, {"Indraprastha", 20}};
    stations["Indraprastha"].neighbors = {{"Dwarka", 20}, {"Akshardham", 4}};
    stations["Akshardham"].neighbors = {{"Indraprastha", 4}, {"Yamuna Bank", 3}};
    stations["Yamuna Bank"].neighbors = {{"Akshardham", 3}, {"New Delhi", 8}};
    stations["New Delhi"].neighbors = {{"Yamuna Bank", 8}};

    int totalDistance = 0;
    int lineSwitches = 0;

    while (true) {
        string userRole;
        cout << "Are you an admin or a user? (admin/user/exit): ";
        cin >> userRole;
    
        if (userRole == "admin") {
    adminLogin(); // Prompt for admin login

    if (isAdmin) {
        cout << "Welcome, Admin!" << endl;
        int adminChoice;

        do {
            cout << "Admin Menu:" << endl;
            cout << "1. Add a new station" << endl;
            cout << "2. Update fares" << endl;
            cout << "3. Change station name" << endl;
            cout << "4. Exit admin mode" << endl;
            cout << "Enter your choice: ";
            cin >> adminChoice;

            switch (adminChoice) {
                case 1:
                    addNewStation(stations);
                    break;
                case 2:
                    updateFares();
                    break;
                case 3:
                    changeStationName(stations);
                    break;
                case 4:
                    cout << "Exiting admin mode." << endl;
                    break;
                default:
                    cout << " " << endl;
            }
        } while (adminChoice != 4);
    }
} if (userRole == "exit") {
            printCenteredMessage("Exiting the program. Thank you for using our navigation system!");
            break;
        } else if (userRole == "user") {
            // 
            isAdmin = false;

            string station1, station2;
            cout << "Enter boarding station: ";
            cin.ignore();
            getline(cin, station1);
            cout << "Enter destination station: ";
            getline(cin, station2);

            vector<string> shortestPath = findShortestPath(stations, station1, station2, totalDistance, lineSwitches);
            double fare = 0.0;

            if (totalDistance <= 2) {
                fare = fareCategories["0-2 km"];
            } else if (totalDistance <= 5) {
                fare = fareCategories["3-5 km"];
            } else if (totalDistance <= 12) {
                fare = fareCategories["6-12 km"];
            } else if (totalDistance <= 21) {
                fare = fareCategories["13-21 km"];
            } else if (totalDistance <= 32) {
                fare = fareCategories["22-32 km"];
            } else {
                fare = fareCategories["33 km and above"];
            }

            printCenteredMessage("\nShortest path");
            for (const string& station : shortestPath) {
                cout << station << " -> ";
            }
            cout << station2 << endl;

            cout << "\nTotal distance: " << totalDistance << " km" << endl;
            cout << "Line switches: " << lineSwitches << endl;
            cout << "Total fare: " << fare << endl;
        
        while (true) {
                string choice;
                cout << "Do you want to search for another path or exit? (search/exit): ";
                cin >> choice;

                if (choice == "exit") {
                    break;
                } else if (choice == "search") {
                    // 
                    isAdmin = false;

            string station1, station2;
            cout << "Enter boarding station: ";
            cin.ignore();
            getline(cin, station1);
            cout << "Enter destination station: ";
            getline(cin, station2);

            vector<string> shortestPath = findShortestPath(stations, station1, station2, totalDistance, lineSwitches);
            double fare = 0.0;

            if (totalDistance <= 2) {
                fare = fareCategories["0-2 km"];
            } else if (totalDistance <= 5) {
                fare = fareCategories["3-5 km"];
            } else if (totalDistance <= 12) {
                fare = fareCategories["6-12 km"];
            } else if (totalDistance <= 21) {
                fare = fareCategories["13-21 km"];
            } else if (totalDistance <= 32) {
                fare = fareCategories["22-32 km"];
            } else {
                fare = fareCategories["33 km and above"];
            }

            printCenteredMessage("\nShortest path");
            for (const string& station : shortestPath) {
                cout << station << " -> ";
            }
            cout << station2 << endl;

            cout << "\nTotal distance: " << totalDistance << " km" << endl;
            cout << "Line switches: " << lineSwitches << endl;
            cout << "Total fare: " << fare << endl;
                } else {
                    cout << "Invalid choice. Please enter 'search' or 'exit'." << endl;
                }
            }
        } else {
            cout << "Invalid role. Please specify 'admin', 'user', or 'exit'." << endl;
        }
    }

    return 0;
}