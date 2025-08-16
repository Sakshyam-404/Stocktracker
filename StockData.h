#pragma once
#include <string>
class StockData {
protected:
    std::string symbol;
    float price;
    float change;
    float changePercent;
    float high;
    float low;
    // Additional fields from API for analysis
    float previousClose;
    float volume;
    float avgVolume;
    float yearHigh;
    float yearLow;
    float peRatio;
    float eps;
public:
    StockData(const std::string& symbol = "",
              float price = 0.0f,
              float change = 0.0f,
              float changePercent = 0.0f,
              float high = 0.0f,
              float low = 0.0f,
              float previousClose = 0.0f,
              float volume = 0.0f,
              float avgVolume = 0.0f,
              float yearHigh = 0.0f,
              float yearLow = 0.0f,
              float peRatio = 0.0f,
              float eps = 0.0f);

    void display() const;
    std::string getSymbol() const;
    float getPrice() const;
    float getChange() const;
    float getChangePercent() const;
    float getHigh() const;
    float getLow() const;

    // New getters
    float getPreviousClose() const { return previousClose; }
    float getVolume() const { return volume; }
    float getAvgVolume() const { return avgVolume; }
    float getYearHigh() const { return yearHigh; }
    float getYearLow() const { return yearLow; }
    float getPeRatio() const { return peRatio; }
    float getEps() const { return eps; }

    void setData(float p, float c, float cp, float h, float l);
};
 