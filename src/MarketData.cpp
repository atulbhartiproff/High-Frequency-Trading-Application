#include "MarketData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Global data storage (we'll use this for now)
vector<MarketData> globalData;

bool loadData(vector<MarketData>& data) {
    ifstream file("../src/market_data/market_data.csv");
    string line;
    
    if (file.is_open()) {
        getline(file, line); // Skip header
        
        while (getline(file, line)) {
            stringstream ss(line);
            string timestamp, symbol, price_str, volume_str;
            
            getline(ss, timestamp, ',');
            getline(ss, symbol, ',');
            getline(ss, price_str, ',');
            getline(ss, volume_str, ',');
            
            MarketData md;
            md.Time = timestamp;
            md.Abb = symbol;
            md.price = stod(price_str);
            md.volume = stoi(volume_str);
            
            data.push_back(md);
        }
        file.close();
        
        // Also populate global data for backward compatibility
        globalData = data;
        return true;
    }
    return false;
}

double calculateMovingAverage(const vector<MarketData>& data, const string& symbol, int periods) {
    vector<double> prices;
    for (const auto& md : data) {
        if (md.Abb == symbol) {
            prices.push_back(md.price);
        }
    }
    
    if (prices.size() < periods) return 0.0;
    
    double sum = 0.0;
    for (int i = prices.size() - periods; i < prices.size(); i++) {
        sum += prices[i];
    }
    return sum / periods;
}

void showPriceData(const vector<MarketData>& data, const string& symbol) {
    cout << "\n=== " << symbol << " Price Data ===" << endl;
    for (const auto& md : data) {
        if (md.Abb == symbol) {
            cout << md.Time << " - $" << md.price << " (Vol: " << md.volume << ")" << endl;
        }
    }
}

void generateSignal(const vector<MarketData>& data, const string& symbol) {
    double shortMA = calculateMovingAverage(data, symbol, 2);
    double longMA = calculateMovingAverage(data, symbol, 3);
    
    cout << "\n=== " << symbol << " Trading Signal ===" << endl;
    cout << "Short MA (2-period): $" << shortMA << endl;
    cout << "Long MA (3-period): $" << longMA << endl;
    
    if (shortMA > longMA) {
        cout << "SIGNAL: BUY" << endl;
    } else if (shortMA < longMA) {
        cout << "SIGNAL: SELL" << endl;
    } else {
        cout << "SIGNAL: HOLD" << endl;
    }
}

void showMenu() {
    cout << "\n======= HFT Trading Platform =======" << endl;
    cout << "1. View AAPL price data" << endl;
    cout << "2. View MSFT price data" << endl;
    cout << "3. Get AAPL trading signal" << endl;
    cout << "4. Get MSFT trading signal" << endl;
    cout << "5. Exit" << endl;
    cout << "Choose option (1-5): ";
}
