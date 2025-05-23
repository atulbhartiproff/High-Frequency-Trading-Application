#include "Order.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

int Order::nextOrderId = 1;

Order::Order(const string& symbol, OrderType type, int quantity, double price) 
    : orderId(nextOrderId++), symbol(symbol), type(type), quantity(quantity), 
      price(price), status(OrderStatus::PENDING) {
    
    time_t now = time(0);
    char* timeStr = ctime(&now);
    timestamp = string(timeStr);
    timestamp.pop_back();
}

void Order::displayOrder() const {
    cout << "Order #" << orderId << " | " << symbol << " | " 
         << getTypeString() << " | Qty: " << quantity 
         << " | Price: $" << fixed << setprecision(2) << price 
         << " | Status: " << getStatusString() << endl;
}

string Order::getTypeString() const {
    return (type == OrderType::BUY) ? "BUY" : "SELL";
}

string Order::getStatusString() const {
    switch(status) {
        case OrderStatus::PENDING: return "PENDING";
        case OrderStatus::FILLED: return "FILLED";
        case OrderStatus::CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

// OrderManager implementation
void OrderManager::placeOrder(const string& symbol, OrderType type, int quantity, double price) {
    Order newOrder(symbol, type, quantity, price);
    orders.push_back(newOrder);
    
    cout << "\n✓ Order placed successfully!" << endl;
    newOrder.displayOrder();
}

void OrderManager::showAllOrders() const {
    if (orders.empty()) {
        cout << "\nNo orders found." << endl;
        return;
    }
    
    cout << "\n=== All Orders ===" << endl;
    for (const auto& order : orders) {
        order.displayOrder();
    }
}

void OrderManager::showOrdersForSymbol(const string& symbol) const {
    cout << "\n=== Orders for " << symbol << " ===" << endl;
    bool found = false;
    
    for (const auto& order : orders) {
        if (order.symbol == symbol) {
            order.displayOrder();
            found = true;
        }
    }
    
    if (!found) {
        cout << "No orders found for " << symbol << endl;
    }
}

bool OrderManager::cancelOrder(int orderId) {
    for (auto& order : orders) {
        if (order.orderId == orderId && order.status == OrderStatus::PENDING) {
            order.status = OrderStatus::CANCELLED;
            cout << "\n✓ Order #" << orderId << " cancelled successfully!" << endl;
            return true;
        }
    }
    
    cout << "\n✗ Order #" << orderId << " not found or already processed!" << endl;
    return false;
}
