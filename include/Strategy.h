#pragma once
#include "MarketData.h"
#include <vector>
#include <string>

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual int generateSignal(const std::vector<MarketData>& data, const std::string& symbol) = 0;
    virtual std::string getStratName() const = 0;
};

class MovingAvgStrat : public Strategy {
private:
    int shortP;
    int longP;
    
public:
    MovingAvgStrat(int short_P, int long_P);
    int generateSignal(const std::vector<MarketData>& data, const std::string& symbol) override;
    std::string getStratName() const override;
};