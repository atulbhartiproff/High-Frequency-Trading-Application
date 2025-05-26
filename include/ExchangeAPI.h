#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "MarketData.h"
#include "Order.h"

struct ExchangeCredentials {
    std::string apiKey;
    std::string secretKey;
    std::string passphrase; // For some exchanges
    bool sandboxMode = true; // Start in test mode
};

struct ExchangeOrder {
    std::string exchangeOrderId;
    std::string symbol;
    std::string side; // "buy" or "sell"
    double quantity;
    double price;
    std::string status; // "open", "filled", "cancelled"
    std::string timestamp;
};

// Base class for all exchange connections
class ExchangeAPI {
public:
    virtual ~ExchangeAPI() = default;
    
    // Authentication
    virtual bool authenticate(const ExchangeCredentials& creds) = 0;
    
    // Market data
    virtual bool getMarketPrice(const std::string& symbol, double& price) = 0;
    virtual bool subscribeToMarketData(const std::string& symbol) = 0;
    
    // Order management
    virtual std::string placeOrder(const std::string& symbol, const std::string& side, 
                                 double quantity, double price) = 0;
    virtual bool cancelOrder(const std::string& orderId) = 0;
    virtual std::vector<ExchangeOrder> getOpenOrders() = 0;
    
    // Account info
    virtual bool getAccountBalance(std::map<std::string, double>& balances) = 0;
    
    // Connection status
    virtual bool isConnected() const = 0;
    virtual std::string getLastError() const = 0;
    
protected:
    std::string lastError;
    bool connected = false;
};

// Simulated exchange for testing (before connecting to real exchanges)
class SimulatedExchange : public ExchangeAPI {
private:
    std::map<std::string, double> accountBalances;
    std::vector<ExchangeOrder> openOrders;
    std::map<std::string, double> marketPrices;
    int nextOrderId = 1;
    
public:
    SimulatedExchange();
    
    bool authenticate(const ExchangeCredentials& creds) override;
    bool getMarketPrice(const std::string& symbol, double& price) override;
    bool subscribeToMarketData(const std::string& symbol) override;
    
    std::string placeOrder(const std::string& symbol, const std::string& side, 
                          double quantity, double price) override;
    bool cancelOrder(const std::string& orderId) override;
    std::vector<ExchangeOrder> getOpenOrders() override;
    
    bool getAccountBalance(std::map<std::string, double>& balances) override;
    bool isConnected() const override;
    std::string getLastError() const override;
    
    // Simulation helpers
    void setMarketPrice(const std::string& symbol, double price);
    void simulateOrderFill(const std::string& orderId);
};
