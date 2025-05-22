#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct MarketData{
    string Time;
    string Abb;
    double price;
    int volume;
};


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
        
        cout<<"Checking if it works"<<endl;
        for (const auto& md : data) {
            cout << "Time: " << md.Time<< " | Symbol: " << md.Abb<< " | Price: $" << md.price<< " | Volume: " << md.volume << endl;
        }
        file.close();

    }
    else
    {
        cout<<"That is weird, Where da file it?";
    }
    return 0;
}
