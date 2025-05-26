#pragma once
#include "ExchangeAPI.h"
#include <memory>

class ExchangeManager {
private:
    std::unique_ptr<ExchangeAPI> exchange;
    bool connected = false;
    
public:
    ExchangeManager();
    ~ExchangeManager() = default;
    
    // Connection management
    bool connectToExchange(const ExchangeCredentials& creds);
    void disconnect();
    bool isConnected() const;
    
    // Live market data
    bool getLivePrice(const std::string& symbol, double& price);
    
    // Live order execution
    std::string executeLiveOrder(const std::string& symbol, const std::string& side, 
                               double quantity, double price);
    
    // Account management
    void showAccountBalance();
    void showLiveOrders();
    
    // Status
    std::string getStatus() const;
};
