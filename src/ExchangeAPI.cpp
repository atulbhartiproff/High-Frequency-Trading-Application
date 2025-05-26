#include "ExchangeAPI.h"
#include <iostream>
#include <sstream>
#include <random>
#include <thread>
#include <chrono>

SimulatedExchange::SimulatedExchange() {
    // Initialize with some starting balances
    accountBalances["USD"] = 10000.0;  // $10,000 starting cash
    accountBalances["AAPL"] = 0.0;
    accountBalances["MSFT"] = 0.0;
    
    // Set initial market prices
    marketPrices["AAPL"] = 150.25;
    marketPrices["MSFT"] = 280.15;
    
    connected = false;
}

bool SimulatedExchange::authenticate(const ExchangeCredentials& creds) {
    std::cout << "🔐 Authenticating with simulated exchange..." << std::endl;
    
    // Simulate authentication delay
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    if (creds.apiKey.empty()) {
        lastError = "API key is required";
        return false;
    }
    
    connected = true;
    std::cout << "✅ Authentication successful!" << std::endl;
    std::cout << "🏦 Connected to Simulated Exchange (Sandbox Mode)" << std::endl;
    
    return true;
}

bool SimulatedExchange::getMarketPrice(const std::string& symbol, double& price) {
    if (!connected) {
        lastError = "Not connected to exchange";
        return false;
    }
    
    auto it = marketPrices.find(symbol);
    if (it == marketPrices.end()) {
        lastError = "Symbol not found: " + symbol;
        return false;
    }
    
    // Add small random price movement to simulate live market
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.5, 0.5);
    
    price = it->second + dis(gen);
    marketPrices[symbol] = price; // Update stored price
    
    return true;
}

bool SimulatedExchange::subscribeToMarketData(const std::string& symbol) {
    if (!connected) {
        lastError = "Not connected to exchange";
        return false;
    }
    
    std::cout << "📈 Subscribed to market data for " << symbol << std::endl;
    return true;
}

std::string SimulatedExchange::placeOrder(const std::string& symbol, const std::string& side, 
                                        double quantity, double price) {
    if (!connected) {
        lastError = "Not connected to exchange";
        return "";
    }
    
    // Generate order ID
    std::string orderId = "SIM_" + std::to_string(nextOrderId++);
    
    // Create exchange order
    ExchangeOrder order;
    order.exchangeOrderId = orderId;
    order.symbol = symbol;
    order.side = side;
    order.quantity = quantity;
    order.price = price;
    order.status = "open";
    
    // Get current timestamp
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    order.timestamp = std::ctime(&time_t);
    
    // Check if we have sufficient balance
    double required = quantity * price;
    if (side == "buy" && accountBalances["USD"] < required) {
        lastError = "Insufficient USD balance";
        return "";
    }
    
    if (side == "sell" && accountBalances[symbol] < quantity) {
        lastError = "Insufficient " + symbol + " balance";
        return "";
    }
    
    openOrders.push_back(order);
    
    std::cout << "📝 Order placed: " << orderId << " | " << side << " " 
              << quantity << " " << symbol << " @ $" << price << std::endl;
    
    // Simulate immediate fill (in real exchange, this would be partial/delayed)
    simulateOrderFill(orderId);
    
    return orderId;
}

bool SimulatedExchange::cancelOrder(const std::string& orderId) {
    if (!connected) {
        lastError = "Not connected to exchange";
        return false;
    }
    
    for (auto& order : openOrders) {
        if (order.exchangeOrderId == orderId && order.status == "open") {
            order.status = "cancelled";
            std::cout << "❌ Order cancelled: " << orderId << std::endl;
            return true;
        }
    }
    
    lastError = "Order not found or already processed: " + orderId;
    return false;
}

std::vector<ExchangeOrder> SimulatedExchange::getOpenOrders() {
    std::vector<ExchangeOrder> result;
    
    for (const auto& order : openOrders) {
        if (order.status == "open") {
            result.push_back(order);
        }
    }
    
    return result;
}

bool SimulatedExchange::getAccountBalance(std::map<std::string, double>& balances) {
    if (!connected) {
        lastError = "Not connected to exchange";
        return false;
    }
    
    balances = accountBalances;
    return true;
}

bool SimulatedExchange::isConnected() const {
    return connected;
}

std::string SimulatedExchange::getLastError() const {
    return lastError;
}

void SimulatedExchange::setMarketPrice(const std::string& symbol, double price) {
    marketPrices[symbol] = price;
}

void SimulatedExchange::simulateOrderFill(const std::string& orderId) {
    for (auto& order : openOrders) {
        if (order.exchangeOrderId == orderId && order.status == "open") {
            // Update balances
            double amount = order.quantity * order.price;
            
            if (order.side == "buy") {
                accountBalances["USD"] -= amount;
                accountBalances[order.symbol] += order.quantity;
            } else { // sell
                accountBalances["USD"] += amount;
                accountBalances[order.symbol] -= order.quantity;
            }
            
            order.status = "filled";
            std::cout << "✅ Order filled: " << orderId << std::endl;
            break;
        }
    }
}
