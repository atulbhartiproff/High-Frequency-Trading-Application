#pragma once
#include <string>
#include <vector>
#include <ctime>

enum class OrderType {
    BUY,
    SELL
};

enum class OrderStatus {
    PENDING,
    FILLED,
    CANCELLED
};

class Order {
private:
    static int nextOrderId;  // Auto-increment order IDs
    
public:
    int orderId;
    std::string symbol;
    OrderType type;
    int quantity;
    double price;
    OrderStatus status;
    std::string timestamp;
    
    // Constructor
    Order(const std::string& symbol, OrderType type, int quantity, double price);
    
    // Display order details
    void displayOrder() const;
    
    // Get order type as string
    std::string getTypeString() const;
    std::string getStatusString() const;
};

// Order Manager class
class OrderManager {
private:
    std::vector<Order> orders;
    
public:
    // Place a new order
    void placeOrder(const std::string& symbol, OrderType type, int quantity, double price);
    
    // Show all orders
    void showAllOrders() const;
    
    // Show orders for specific symbol
    void showOrdersForSymbol(const std::string& symbol) const;
    
    // Cancel an order by ID
    bool cancelOrder(int orderId);
};
