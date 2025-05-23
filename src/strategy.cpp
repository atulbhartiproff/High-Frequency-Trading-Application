#include "Strategy.h"
#include "MarketData.h"
#include <iostream>
using namespace std;

MovingAvgStrat::MovingAvgStrat(int short_P, int long_P) : shortP(short_P), longP(long_P) {
}

int MovingAvgStrat::generateSignal(const vector<MarketData>& data, const string& symbol) {
    double shortMA = calculateMovingAverage(data, symbol, shortP);
    double longMA = calculateMovingAverage(data, symbol, longP);

    cout << "  " << getStratName() << endl;
    cout << "  Short MA (" << shortP << "-period): $" << shortMA << endl;
    cout << "  Long MA (" << longP << "-period): $" << longMA << endl;
    
    if (shortMA > longMA) {
        return 1;  // Buy signal
    } else if (shortMA < longMA) {
        return -1; // Sell signal
    } else {
        return 0;  // Hold
    }
}

string MovingAvgStrat::getStratName() const {
    return "Moving Average Crossover Strategy (" + to_string(shortP) + "/" + to_string(longP) + ")";
}