#include "RiskManager.h"
#include <iostream>
#include <iomanip>
#include <cmath>

RiskManager::RiskManager(double maxPosSize, double maxExposure) 
    : maxPositionSize(maxPosSize), maxTotalExposure(maxExposure) {
}

void RiskManager::updatePosition(const Order& order) {
    if (order.status != OrderStatus::FILLED) return;
    
    std::string symbol = order.symbol;
    int orderQty = (order.type == OrderType::BUY) ? order.quantity : -order.quantity;
    
    if (positions.find(symbol) == positions.end()) {
        Position pos;
        pos.symbol = symbol;
        pos.quantity = orderQty;
        pos.avgPrice = order.price;
        pos.currentPrice = order.price;
        pos.unrealizedPnL = 0.0;
        positions[symbol] = pos;
    } else {
        Position& pos = positions[symbol];
        
        if ((pos.quantity > 0 && orderQty > 0) || (pos.quantity < 0 && orderQty < 0)) {
            double totalValue = (pos.quantity * pos.avgPrice) + (orderQty * order.price);
            pos.quantity += orderQty;
            pos.avgPrice = totalValue / pos.quantity;
        } else {
            pos.quantity += orderQty;
            if (pos.quantity == 0) {
                positions.erase(symbol);
            }
        }
    }
}

void RiskManager::updateMarketPrices(const std::vector<MarketData>& marketData) {
    std::map<std::string, double> latestPrices;
    
    for (const auto& data : marketData) {
        latestPrices[data.Abb] = data.price;
    }
    for (auto& pair : positions) {
        Position& pos = pair.second;
        if (latestPrices.find(pos.symbol) != latestPrices.end()) {
            pos.currentPrice = latestPrices[pos.symbol];
            pos.unrealizedPnL = pos.quantity * (pos.currentPrice - pos.avgPrice);
        }
    }
}

bool RiskManager::validateOrder(const Order& order, double currentPrice) {
    double orderValue = order.quantity * order.price;
    
    if (orderValue > maxPositionSize) {
        std::cout << "⚠️  RISK ALERT: Order exceeds max position size ($" 
                  << maxPositionSize << ")" << std::endl;
        return false;
    }
    double totalExposure = getTotalExposure() + orderValue;
    if (totalExposure > maxTotalExposure) {
        std::cout << "⚠️  RISK ALERT: Order would exceed max total exposure ($" 
                  << maxTotalExposure << ")" << std::endl;
        return false;
    }
    
    return true;
}

void RiskManager::showPositions() const {
    if (positions.empty()) {
        std::cout << "\n📊 No open positions." << std::endl;
        return;
    }
    
    std::cout << "\n📊 === Current Positions ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    
    double totalPnL = 0.0;
    
    for (const auto& pair : positions) {
        const Position& pos = pair.second;
        std::string direction = (pos.quantity > 0) ? "LONG" : "SHORT";
        
        std::cout << pos.symbol << " | " << direction << " " << abs(pos.quantity) 
                  << " @ $" << pos.avgPrice 
                  << " | Current: $" << pos.currentPrice
                  << " | P&L: $" << pos.unrealizedPnL << std::endl;
        
        totalPnL += pos.unrealizedPnL;
    }
    
    std::cout << "💰 Total Unrealized P&L: $" << totalPnL << std::endl;
}

double RiskManager::getTotalExposure() const {
    double total = 0.0;
    for (const auto& pair : positions) {
        const Position& pos = pair.second;
        total += abs(pos.quantity * pos.currentPrice);
    }
    return total;
}

Position* RiskManager::getPosition(const std::string& symbol) {
    auto it = positions.find(symbol);
    return (it != positions.end()) ? &it->second : nullptr;
}
