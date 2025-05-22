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
        
        cout << "=== AAPL Analysis ===" << endl;
        cout << "Avg Price: Rs." << calcAvgPrice(data, "AAPL") << endl;
        cout << "Highest Price: Rs." << findHighestPrice(data, "AAPL") << endl;
        cout << "Lowest Price: Rs." << findLowestPrice(data, "AAPL") << endl;
        cout << "Total Volume: " << calcTotalVolume(data, "AAPL") << endl;
        cout << "2-Period Moving Avg: $" << calcMovingAvg(data, "AAPL", 2) << endl;
        
        cout << "\n=== MSFT Analysis ===" << endl;
        cout << "Avg Price: Rs." << calcAvgPrice(data, "MSFT") << endl;
        cout << "Highest Price: Rs." << findHighestPrice(data, "MSFT") << endl;
        cout << "Lowest Price: Rs." << findLowestPrice(data, "MSFT") << endl;
        cout << "Total Volume: " << calcTotalVolume(data, "MSFT") << endl;
        cout << "2-Period Moving Avg: $" << calcMovingAvg(data, "MSFT", 2) << endl;
        file.close();

    }
    else
    {
        cout<<"That is weird, Where da file it?";
    }
    return 0;
}
