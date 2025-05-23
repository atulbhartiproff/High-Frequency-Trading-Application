#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "MarketData.h"
#include "Order.h"

int getValidChoice() {
    std::string input;
    int choice;
    
    while (true) {
        std::getline(std::cin, input);
        
        try {
            choice = std::stoi(input);
            
            if (choice >= 1 && choice <= 8) {  // Updated range
                return choice;
            } else {
                std::cout << "Please enter a number between 1-8: ";
            }
        }
        catch (std::invalid_argument&) {
            std::cout << "Invalid input! Please enter a number (1-8): ";
        }
        catch (std::out_of_range&) {
            std::cout << "Number too large! Please enter a number (1-8): ";
        }
    }
}

void placeOrderInteractive(OrderManager& orderManager) {
    std::string symbol;
    int typeChoice, quantity;
    double price;
    
    std::cout << "\n=== Place New Order ===" << std::endl;
    std::cout << "Enter symbol (AAPL/MSFT): ";
    std::getline(std::cin, symbol);
    
    std::cout << "Order type: 1=BUY, 2=SELL: ";
    std::cin >> typeChoice;
    
    std::cout << "Quantity: ";
    std::cin >> quantity;
    
    std::cout << "Price: $";
    std::cin >> price;
    
    OrderType type = (typeChoice == 1) ? OrderType::BUY : OrderType::SELL;
    orderManager.placeOrder(symbol, type, quantity, price);
    
    std::cin.ignore(); // Clear input buffer
}

int main() {
    std::cout << "Loading market data..." << std::endl;
    
    std::vector<MarketData> marketData;
    OrderManager orderManager;  // Create order manager
    
    if (!loadData(marketData)) {
        std::cout << "Error: Could not load market data!" << std::endl;
        return 1;
    }
    
    std::cout << "Loaded " << marketData.size() << " records successfully!" << std::endl;
    
    while (true) {
        std::cout << "\n======= HFT Trading Platform =======" << std::endl;
        std::cout << "1. View AAPL price data" << std::endl;
        std::cout << "2. View MSFT price data" << std::endl;
        std::cout << "3. Get AAPL trading signal" << std::endl;
        std::cout << "4. Get MSFT trading signal" << std::endl;
        std::cout << "5. Place new order" << std::endl;
        std::cout << "6. View all orders" << std::endl;
        std::cout << "7. View orders by symbol" << std::endl;
        std::cout << "8. Exit" << std::endl;
        std::cout << "Choose option (1-8): ";
        
        int choice = getValidChoice();
        
        switch (choice) {
            case 1:
                showPriceData(marketData, "AAPL");
                break;
            case 2:
                showPriceData(marketData, "MSFT");
                break;
            case 3:
                generateSignal(marketData, "AAPL");
                break;
            case 4:
                generateSignal(marketData, "MSFT");
                break;
            case 5:
                placeOrderInteractive(orderManager);
                break;
            case 6:
                orderManager.showAllOrders();
                break;
            case 7: {
                std::string symbol;
                std::cout << "Enter symbol (AAPL/MSFT): ";
                std::getline(std::cin, symbol);
                orderManager.showOrdersForSymbol(symbol);
                break;
            }
            case 8:
                std::cout << "Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
                break;
        }
    }
}
