#include "ExchangeManager.h"
#include <iostream>
#include <iomanip>

ExchangeManager::ExchangeManager() {
    // Start with simulated exchange
    exchange = std::make_unique<SimulatedExchange>();
}

bool ExchangeManager::connectToExchange(const ExchangeCredentials& creds) {
    std::cout << "\n🌐 Connecting to exchange..." << std::endl;
    
    if (exchange->authenticate(creds)) {
        connected = true;
        std::cout << "🎉 Exchange connection established!" << std::endl;
        return true;
    } else {
        connected = false;
        std::cout << "❌ Exchange connection failed: " << exchange->getLastError() << std::endl;
        return false;
    }
}

void ExchangeManager::disconnect() {
    connected = false;
    std::cout << "🔌 Disconnected from exchange" << std::endl;
}

bool ExchangeManager::isConnected() const {
    return connected && exchange->isConnected();
}

bool ExchangeManager::getLivePrice(const std::string& symbol, double& price) {
    if (!isConnected()) {
        std::cout << "❌ Not connected to exchange" << std::endl;
        return false;
    }
    
    if (exchange->getMarketPrice(symbol, price)) {
        std::cout << "💹 Live price for " << symbol << ": $" << std::fixed << std::setprecision(2) << price << std::endl;
        return true;
    } else {
        std::cout << "❌ Failed to get price: " << exchange->getLastError() << std::endl;
        return false;
    }
}

std::string ExchangeManager::executeLiveOrder(const std::string& symbol, const std::string& side, 
                                            double quantity, double price) {
    if (!isConnected()) {
        std::cout << "❌ Not connected to exchange" << std::endl;
        return "";
    }
    
    std::cout << "\n🚀 Executing LIVE order..." << std::endl;
    std::string orderId = exchange->placeOrder(symbol, side, quantity, price);
    
    if (!orderId.empty()) {
        std::cout << "✅ Live order executed successfully! Order ID: " << orderId << std::endl;
    } else {
        std::cout << "❌ Order execution failed: " << exchange->getLastError() << std::endl;
    }
    
    return orderId;
}

void ExchangeManager::showAccountBalance() {
    if (!isConnected()) {
        std::cout << "❌ Not connected to exchange" << std::endl;
        return;
    }
    
    std::map<std::string, double> balances;
    if (exchange->getAccountBalance(balances)) {
        std::cout << "\n💰 === Live Account Balance ===" << std::endl;
        for (const auto& balance : balances) {
            if (balance.second > 0.001) { // Only show non-zero balances
                std::cout << balance.first << ": " << std::fixed << std::setprecision(4) << balance.second << std::endl;
            }
        }
    } else {
        std::cout << "❌ Failed to get balance: " << exchange->getLastError() << std::endl;
    }
}

void ExchangeManager::showLiveOrders() {
    if (!isConnected()) {
        std::cout << "❌ Not connected to exchange" << std::endl;
        return;
    }
    
    auto orders = exchange->getOpenOrders();
    
    if (orders.empty()) {
        std::cout << "\n📋 No open orders on exchange" << std::endl;
        return;
    }
    
    std::cout << "\n📋 === Live Exchange Orders ===" << std::endl;
    for (const auto& order : orders) {
        std::cout << "Order " << order.exchangeOrderId << " | " 
                  << order.side << " " << order.quantity << " " 
                  << order.symbol << " @ $" << order.price 
                  << " | Status: " << order.status << std::endl;
    }
}

std::string ExchangeManager::getStatus() const {
    if (isConnected()) {
        return "🟢 Connected to Exchange (Live Trading Mode)";
    } else {
        return "🔴 Disconnected from Exchange";
    }
}
