#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "MarketData.h"
#include "Order.h"
#include "RiskManager.h"
#include "PerformanceMonitor.h"
#include "ExchangeManager.h"

int getValidChoice() {
    std::string input;
    int choice;
    
    while (true) {
        std::getline(std::cin, input);
        
        try {
            choice = std::stoi(input);
            
            if (choice >= 1 && choice <= 17) {  // Updated range for new options
                return choice;
            } else {
                std::cout << "Please enter a number between 1-17: ";
            }
        }
        catch (std::invalid_argument&) {
            std::cout << "Invalid input! Please enter a number (1-17): ";
        }
        catch (std::out_of_range&) {
            std::cout << "Number too large! Please enter a number (1-17): ";
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
    
    Order testOrder(symbol, type, quantity, price);
    
    if (riskManager.validateOrder(testOrder, price)) {
        orderManager.placeOrder(symbol, type, quantity, price);
        testOrder.status = OrderStatus::FILLED;
        riskManager.updatePosition(testOrder);
        std::cout << "✅ Order passed risk checks and executed!" << std::endl;
    } else {
        std::cout << "❌ Order rejected due to risk limits!" << std::endl;
    }
    
    std::cin.ignore();
}

void connectToExchange(ExchangeManager& exchangeManager) {
    std::cout << "\n=== Connect to Exchange ===" << std::endl;
    std::cout << "Enter API Key (or 'demo' for simulation): ";
    
    std::string apiKey;
    std::getline(std::cin, apiKey);
    
    ExchangeCredentials creds;
    creds.apiKey = apiKey;
    creds.sandboxMode = true;  // Always start in sandbox mode for safety
    
    if (exchangeManager.connectToExchange(creds)) {
        std::cout << "🎉 Ready for live trading!" << std::endl;
    } else {
        std::cout << "❌ Connection failed. Check your credentials." << std::endl;
    }
}

void placeLiveOrder(ExchangeManager& exchangeManager, RiskManager& riskManager) {
    if (!exchangeManager.isConnected()) {
        std::cout << "❌ Not connected to exchange. Use option 14 to connect first." << std::endl;
        return;
    }
    
    std::string symbol, side;
    double quantity, price;
    
    std::cout << "\n=== Place LIVE Order ===" << std::endl;
    std::cout << "⚠️  WARNING: This will place a REAL order with REAL money!" << std::endl;
    std::cout << "Symbol (AAPL/MSFT): ";
    std::getline(std::cin, symbol);
    
    std::cout << "Side (buy/sell): ";
    std::getline(std::cin, side);
    
    std::cout << "Quantity: ";
    std::cin >> quantity;
    
    std::cout << "Price: $";
    std::cin >> price;
    std::cin.ignore();
    
    // Create order for risk validation
    OrderType orderType = (side == "buy") ? OrderType::BUY : OrderType::SELL;
    Order testOrder(symbol, orderType, quantity, price);
    
    // Risk check before live execution
    if (!riskManager.validateOrder(testOrder, price)) {
        std::cout << "❌ Order rejected by risk management!" << std::endl;
        return;
    }
    
    // Final confirmation
    std::cout << "\n🚨 FINAL CONFIRMATION 🚨" << std::endl;
    std::cout << "About to place LIVE order: " << side << " " << quantity 
              << " " << symbol << " @ $" << price << std::endl;
    std::cout << "Type 'CONFIRM' to proceed: ";
    
    std::string confirmation;
    std::getline(std::cin, confirmation);
    
    if (confirmation == "CONFIRM") {
        std::string orderId = exchangeManager.executeLiveOrder(symbol, side, quantity, price);
        if (!orderId.empty()) {
            // Update risk manager with executed order
            testOrder.status = OrderStatus::FILLED;
            riskManager.updatePosition(testOrder);
        }
    } else {
        std::cout << "❌ Order cancelled by user." << std::endl;
    }
}

void getLiveMarketPrice(ExchangeManager& exchangeManager) {
    if (!exchangeManager.isConnected()) {
        std::cout << "❌ Not connected to exchange. Use option 14 to connect first." << std::endl;
        return;
    }
    
    std::string symbol;
    std::cout << "Enter symbol for live price (AAPL/MSFT): ";
    std::getline(std::cin, symbol);
    
    double price;
    exchangeManager.getLivePrice(symbol, price);
}

int main() {
    std::cout << "🚀 HFT Trading Platform - LIVE TRADING ENABLED" << std::endl;
    std::cout << "Loading market data..." << std::endl;
    
    std::vector<MarketData> marketData;
    OrderManager orderManager;
    RiskManager riskManager(5000.0, 25000.0);
    ExchangeManager exchangeManager;  // NEW: Exchange connectivity
    
    if (!loadData(marketData)) {
        std::cout << "Error: Could not load market data!" << std::endl;
        return 1;
    }
    
    std::cout << "Loaded " << marketData.size() << " records successfully!" << std::endl;
    riskManager.updateMarketPrices(marketData);
    
    while (true) {
        std::cout << "\n======= HFT Trading Platform =======" << std::endl;
        std::cout << exchangeManager.getStatus() << std::endl;  // Show connection status
        std::cout << "\n--- MARKET ANALYSIS ---" << std::endl;
        std::cout << "1. View AAPL price data" << std::endl;
        std::cout << "2. View MSFT price data" << std::endl;
        std::cout << "3. Get AAPL trading signal" << std::endl;
        std::cout << "4. Get MSFT trading signal" << std::endl;
        
        std::cout << "\n--- SIMULATED TRADING ---" << std::endl;
        std::cout << "5. Place simulated order (with risk check)" << std::endl;
        std::cout << "6. View all simulated orders" << std::endl;
        std::cout << "7. View orders by symbol" << std::endl;
        std::cout << "8. View current positions & P&L" << std::endl;
        std::cout << "9. View risk exposure" << std::endl;
        
        std::cout << "\n--- PERFORMANCE ---" << std::endl;
        std::cout << "10. Run basic performance benchmarks" << std::endl;
        std::cout << "11. Run advanced HFT optimizations" << std::endl;
        std::cout << "12. 🔍 VERIFY Multi-Core Threading" << std::endl;
        
        std::cout << "\n--- LIVE TRADING ---" << std::endl;
        std::cout << "13. Get live market price" << std::endl;
        std::cout << "14. Connect to exchange" << std::endl;
        std::cout << "15. View live account balance" << std::endl;
        std::cout << "16. Place LIVE order (REAL MONEY)" << std::endl;
        std::cout << "17. Exit" << std::endl;
        std::cout << "Choose option (1-17): ";
        
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
                riskManager.updateMarketPrices(marketData);
                riskManager.showPositions();
                break;
            case 9: {
                double exposure = riskManager.getTotalExposure();
                std::cout << "\n💰 Total Portfolio Exposure: $" << exposure << std::endl;
                break;
            }
            case 10:
                std::cout << "\n🚀 Running Basic Performance Benchmarks..." << std::endl;
                PerformanceMonitor::measureDataLoad();
                PerformanceMonitor::measureSignalGeneration();
                PerformanceMonitor::measureOrderPlacement();
                break;
            case 11:
                std::cout << "\n⚡ Running Advanced HFT Optimizations..." << std::endl;
                PerformanceMonitor::measureCPUAffinity();
                PerformanceMonitor::measureCacheOptimization();
                break;
            case 12:
                PerformanceMonitor::verifyMultiThreading();
                break;
            case 13:
                getLiveMarketPrice(exchangeManager);
                break;
            case 14:
                connectToExchange(exchangeManager);
                break;
            case 15:
                exchangeManager.showAccountBalance();
                break;
            case 16:
                placeLiveOrder(exchangeManager, riskManager);
                break;
            case 17:
                std::cout << "Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
                break;
        }
    }
}
