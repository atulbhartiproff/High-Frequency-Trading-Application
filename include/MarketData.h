#pragma once
#include <string>
#include <vector>
using namespace std;

struct MarketData {
    string timestamp;
    string symbol;
    double price;
    int volume;
};

bool loadMarketData(vector<MarketData>& data);
double calculateMovingAverage(const vector<MarketData>& data, const string& symbol, int periods);
void showPriceData(const vector<MarketData>& data, const string& symbol);
void generateSignal(const vector<MarketData>& data, const string& symbol);
void showMenu();