#pragma once
#include <string>
#include <vector>
using namespace std;

struct MarketData {
    string Time;
    string Abb;
    double price;
    int volume;
};
bool loadData(vector<MarketData>& data);
double calculateMovingAverage(const vector<MarketData>& data, const string& symbol, int periods);
void showPriceData(const vector<MarketData>& data, const string& symbol);
void generateSignal(const vector<MarketData>& data, const string& symbol);
void showMenu();