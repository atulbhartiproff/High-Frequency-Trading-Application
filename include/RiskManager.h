#pragma once
#include "Order.h"
#include "MarketData.h"
#include <vector>
#include <map>

struct Position {
    std::string symbol;
    int quantity;        // Positive = long, negative = short
    double avgPrice;     // Average purchase price
    double currentPrice; // Current market price
    double unrealizedPnL; // Profit/Loss
};

class RiskManager {
private:
    std::map<std::string, Position> positions;
    double maxPositionSize;
    double maxTotalExposure;
    
public:
    RiskManager(double maxPosSize = 10000.0, double maxExposure = 50000.0);
    
    // Update positions based on filled orders
    void updatePosition(const Order& order);
    
    // Update current market prices and calculate P&L
    void updateMarketPrices(const std::vector<MarketData>& marketData);
    
    // Check if order passes risk limits
    bool validateOrder(const Order& order, double currentPrice);
    
    // Display all positions
    void showPositions() const;
    
    // Calculate total portfolio value
    double getTotalExposure() const;
    
    // Get position for specific symbol
    Position* getPosition(const std::string& symbol);
};
