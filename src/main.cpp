#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    cout<<"Well Market Data hmmmm..."<<endl;
    ifstream file("../src/market_data/market_data.csv");
    string line;

    if(file.is_open()){
        while(getline(file,line)){
            cout<<line<<endl;
        }
        file.close();
    }
    else
    {
        cout<<"That is weird, Where da file it?";
    }
    return 0;
}
