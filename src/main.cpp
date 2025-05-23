#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "MarketData.h"
using namespace std;


int getValidChoice() {
    string input;
    int choice;
    
    while (true) {
        getline(cin, input); // Read entire line as string
        
        // Try to convert string to integer
        try {
            choice = stoi(input);
            
            // Check if choice is in valid range
            if (choice >= 1 && choice <= 5) {
                return choice;
            } else {
                cout << "Please enter a number between 1-5: ";
            }
        }
        catch (invalid_argument&) {
            cout << "Invalid input! Please enter a number (1-5): ";
        }
        catch (out_of_range&) {
            cout << "Number too large! Please enter a number (1-5): ";
        }
    }
}

int main() {
    cout << "Loading market data..." << endl;
    
    if (!loadData()) {
        cout << "Error: Could not load market data!" << endl;
        return 1;
    }
    
    while (true) {
        showMenu();
        int choice=getValidChoice();
        
        switch (choice) {
            case 1:
                showPriceData("AAPL");
                break;
            case 2:
                showPriceData("MSFT");
                break;
            case 3:
                generateSignal("AAPL");
                break;
            case 4:
                generateSignal("MSFT");
                break;
            case 5:
                cout << "Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
        }
    }
}
