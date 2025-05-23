#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "MarketData.h"
#include "Order.h"
#include "RiskManager.h"

int getValidChoice() {
    std::string input;
    int choice;
    
    while (true) {
        std::getline(std::cin, input);
        
        try {
            choice = std::stoi(input);
            
            if (choice >= 1 && choice <= 10) {  
                return choice;
            } else {
                std::cout << "Please enter a number between 1-10: ";
            }
        }
        catch (std::invalid_argument&) {
            std::cout << "Invalid input! Please enter a number (1-10): ";
        }
        catch (std::out_of_range&) {
            std::cout << "Number too large! Please enter a number (1-10): ";
        }
    }
}

void placeOrderWithRiskCheck(OrderManager& orderManager, RiskManager& riskManager, 
                           const std::vector<MarketData>& marketData) {
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
    
    // Create order for validation
    Order testOrder(symbol, type, quantity, price);
    
    // Risk validation before placing order
    if (riskManager.validateOrder(testOrder, price)) {
        orderManager.placeOrder(symbol, type, quantity, price);
        
        // Simulate order fill and update position
        testOrder.status = OrderStatus::FILLED;
        riskManager.updatePosition(testOrder);
        
        std::cout << " Order passed risk checks and executed!" << std::endl;
    } else {
        std::cout << " Order rejected due to risk limits!" << std::endl;
    }
    
    std::cin.ignore(); 
}

int main() {
    std::cout << "Loading market data..." << std::endl;
    
    std::vector<MarketData> marketData;
    OrderManager orderManager;
    RiskManager riskManager(5000.0, 25000.0);  
    
    if (!loadData(marketData)) {
        std::cout << "Error: Could not load market data!" << std::endl;
        return 1;
    }
    
    std::cout << "Loaded " << marketData.size() << " records successfully!" << std::endl;
    
    riskManager.updateMarketPrices(marketData);
    
    while (true) {
        std::cout << "\n======= HFT Trading Platform =======" << std::endl;
        std::cout << "1. View AAPL price data" << std::endl;
        std::cout << "2. View MSFT price data" << std::endl;
        std::cout << "3. Get AAPL trading signal" << std::endl;
        std::cout << "4. Get MSFT trading signal" << std::endl;
        std::cout << "5. Place new order (with risk check)" << std::endl;
        std::cout << "6. View all orders" << std::endl;
        std::cout << "7. View orders by symbol" << std::endl;
        std::cout << "8. View current positions & P&L" << std::endl;
        std::cout << "9. View risk exposure" << std::endl;
        std::cout << "10. Exit" << std::endl;
        std::cout << "Choose option (1-10): ";
        
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
                placeOrderWithRiskCheck(orderManager, riskManager, marketData);
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
                riskManager.updateMarketPrices(marketData);  // Refresh P&L
                riskManager.showPositions();
                break;
            case 9: {
                double exposure = riskManager.getTotalExposure();
                std::cout << "\n Total Portfolio Exposure: $" << exposure << std::endl;
                break;
            }
            case 10:
                std::cout << "Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
                break;
        }
    }
}
