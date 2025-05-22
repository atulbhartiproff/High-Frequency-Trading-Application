#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

struct MarketData{
    string Time;
    string Abb;
    double price;
    int volume;
};


double calcAvgPrice(const vector<MarketData>& data, const string& symbol)
{
    double total=0.0;
    int count=0;
    for(const auto& md:data)
    {
        if(md.Abb==symbol)
        {
            total+=md.price;
            count++;
        }
    }

    return count>0?total/count:0.0;
}

double findHighestPrice(const vector<MarketData>& data, const string& symbol) {
    double highest = 0.0;
    
    for (const auto& md : data) {
        if (md.Abb == symbol && md.price > highest) {
            highest = md.price;
        }
    }
    
    return highest;
}

double findLowestPrice(const vector<MarketData>& data, const string& symbol) {
    double lowest = 999999; 
    
    for (const auto& md : data) {
        if (md.Abb == symbol && md.price < lowest) {
            lowest = md.price;
        }
    }
    
    return lowest == 999999.0 ? 0.0 : lowest;
}

int calcTotalVolume(const vector<MarketData>& data, const string& symbol) {
    int totalVolume = 0;
    for (const auto& md : data) {
        if (md.Abb == symbol) {
            totalVolume += md.volume;
        }
    }
    return totalVolume;
}

double calcMovingAvg(const vector<MarketData>& data, const string& symbol, int periods) {
    vector<double> prices;
    
    for (const auto& md : data) {
        if (md.Abb == symbol) {
            prices.push_back(md.price);
        }
    }
    
    if (prices.size() < periods) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (int i = prices.size() - periods; i < prices.size(); i++) {
        sum += prices[i];
    }
    
    return sum / periods;
}

int genTradingSignal(const vector<MarketData>& data, const string& symbol, int shortP, int longP)
{
    double shortMA=calcMovingAvg(data,symbol,shortP);
    double longMA=calcMovingAvg(data,symbol,longP);

    //Lets use MOVING AVERAGE CROSSOVER strategy for now=> 
    //short>long= Upward trend => BUY BUY BUY
    //short<long= Downward trend => SELL SELL SELL
    //short=long= Nothing, just wait => HOOOOOLD

    if (shortMA > longMA) {
        return 1; // Buy signal
    } else if (shortMA < longMA) {
        return -1; // Sell signal
    } else {
        return 0; // Hold
    }
}




int main() {
    cout<<"Well Market Data hmmmm..."<<endl;
    vector<MarketData> data;
    ifstream file("../src/market_data/market_data.csv");
    string line;

    if(file.is_open()){
        getline(file,line);
        cout<<"COLUMNS:\n"<<line<<endl;
        
        while(getline(file,line)){
            stringstream ss(line);
            string Time, Abb, strprice, strvolume;

            getline(ss,Time,',');
            getline(ss,Abb,',');
            getline(ss,strprice,',');
            getline(ss,strvolume,',');

            MarketData md;
            md.Time=Time;
            md.Abb=Abb;
            md.price=stod(strprice);
            md.volume=stoi(strvolume);

            data.push_back(md);
        }
        cout<<"LOADED DATA"<<endl;
        vector<string> symbols={"AAPL","MSFT"};
        int shortP=2;
        int longP=3;

        for(const auto& symbol:symbols)
        {
            cout << "=== " << symbol << " Trading Analysis ===" << endl;
            int signal = genTradingSignal(data, symbol, shortP, longP);
            
            if (signal == 1) {
                cout << "  --> BUY SIGNAL! Short MA above Long MA || UPWARD TREND" << endl;
            } else if (signal == -1) {
                cout << "  --> SELL SIGNAL! Short MA below Long MA || DOWNWARD TREND" << endl;
            } else {
                cout << "  --> HOLD - Moving averages are equal || MEHH" << endl;
            }
            cout << endl;
        }

        file.close();

    }
    else
    {
        cout<<"That is weird, Where da file it?";
    }
    return 0;
}
