#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "../include/MarketData.h"
using namespace std;

struct MarketData{
    string Time;
    string Abb;
    double price;
    int volume;
};

vector<MarketData> Globaldata;

bool loadData(){
    ifstream file("../src/market_data/market_data.csv");
    string line;

    if(file.is_open()){
        getline(file,line);
        
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

            Globaldata.push_back(md);
        }

        file.close();
        return true;

    }
    else
    {
        cout<<"That is weird, Where da file it?";
        return false;
    }
}

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

void showMenu() {
    cout << "\n======= HFT Trading Platform =======" << endl;
    cout << "1. View AAPL price data" << endl;
    cout << "2. View MSFT price data" << endl;
    cout << "3. Get AAPL trading signal" << endl;
    cout << "4. Get MSFT trading signal" << endl;
    cout << "5. Exit" << endl;
    cout << "Choose option (1-5): ";
}

void showPriceData(const string& symbol) {
    cout << "\n=== " << symbol << " Price Data ===" << endl;
    for (const auto& md : Globaldata) {
        if (md.Abb == symbol) {
            cout << md.Time << " - $" << md.price << " (Vol: " << md.volume << ")" << endl;
        }
    }
}

void generateSignal(const string& symbol) {
    double shortMA = calcMovingAvg(Globaldata,symbol, 2);
    double longMA = calcMovingAvg(Globaldata, symbol, 3);
    
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

int main() {
    cout << "Loading market data..." << endl;
    
    if (!loadData()) {
        cout << "Error: Could not load market data!" << endl;
        return 1;
    }
    
    cout << "Loaded " <<Globaldata.size() << " records successfully!" << endl;
    while (true) {
        showMenu();
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                showPriceData("AAPL");
                break;
            case 2:
                showPriceData("MSFT");
                break;
            case 3:
                generateSignal("AAPL");
                break;
            case 4:
                generateSignal("MSFT");
                break;
            case 5:
                cout << "Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
