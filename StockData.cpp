#include "StockData.h"
#include <iostream>

StockData::StockData(const std::string& symbol, float price, float change, float changePercent, float high, float low,
                     float previousClose, float volume, float avgVolume, float yearHigh, float yearLow, float peRatio, float eps)
    : symbol(symbol), price(price), change(change), changePercent(changePercent), high(high), low(low),
      previousClose(previousClose), volume(volume), avgVolume(avgVolume), yearHigh(yearHigh), yearLow(yearLow),
      peRatio(peRatio), eps(eps) {}

void StockData::display() const {
    std::cout << "Symbol: " << symbol << "\n"
              << "Price: " << price << "\n"
              << "Change: " << change << "\n"
              << "Change Percent: " << changePercent << "%\n"
              << "High: " << high << "\n"
              << "Low: " << low << std::endl;
}

std::string StockData::getSymbol() const { return symbol; }
float StockData::getPrice() const { return price; }
float StockData::getChange() const { return change; }
float StockData::getChangePercent() const { return changePercent; }
float StockData::getHigh() const { return high; }
float StockData::getLow() const { return low; }

 

void StockData::setData(float p, float c, float cp, float h, float l) {  //why this function is needed? even though we have already set the data in the constructor? 
    //because it is used to update the data after fetching from API.the stock data changes over time
	//instead of creating a new object,we can just update the existing one.
    price = p;
    change = c;
    changePercent = cp;
    high = h;
    low = l;
}