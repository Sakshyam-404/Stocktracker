#include "StockAPI.h"
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
using nlohmann::json;

namespace {
    static const char* API_KEY = "Rjopaz0vhf8OcX4j6qvrO3pnj9MxhgfZ";
    std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(' ');
		if (start == std::string::npos) return std::string();      //why inside the a nameless namespace? because the main reason is to hide api key and avoid name conflicts.
        size_t end = s.find_last_not_of(' ');
        return s.substr(start, end - start + 1);
    }
    std::string toUpper(std::string s) {
        for (char& c : s) {
            if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
        }
        return s;
    }
    inline float getFloatOrZero(const json& obj, const char* key) {
        return obj.contains(key) ? static_cast<float>(obj.value(key, 0.0)) : 0.0f;
    }
    std::string makeHttpRequest(const std::string& url) {
        cpr::Response r = cpr::Get(cpr::Url{url});
        if (r.status_code >= 200 && r.status_code < 300 && !r.text.empty()) {
            return r.text;
        }
        return std::string();
    }
}

StockData StockAPI::fetchStock(const std::string& symbol) {
    std::string cleanSymbol = toUpper(trim(symbol));
    std::string url = "https://financialmodelingprep.com/api/v3/quote/" + cleanSymbol + "?apikey=" + API_KEY;
    std::string response = makeHttpRequest(url);
    if (response.empty()) return StockData(cleanSymbol);

    json j = json::parse(response, nullptr, false);
    if (!j.is_array() || j.empty()) return StockData(cleanSymbol);
    const json& o = j.at(0);
    float price = getFloatOrZero(o, "price");
    float change = getFloatOrZero(o, "change");
    float changePercent = getFloatOrZero(o, "changesPercentage");
    float high = getFloatOrZero(o, "dayHigh");
    float low = getFloatOrZero(o, "dayLow");
    float previousClose = getFloatOrZero(o, "previousClose");
    float volume = getFloatOrZero(o, "volume");
    float avgVolume = getFloatOrZero(o, "avgVolume");
    float yearHigh = getFloatOrZero(o, "yearHigh");
    float yearLow = getFloatOrZero(o, "yearLow");
    float peRatio = getFloatOrZero(o, "pe");
    float eps = getFloatOrZero(o, "eps");
    return StockData(cleanSymbol, price, change, changePercent, high, low,
                     previousClose, volume, avgVolume, yearHigh, yearLow, peRatio, eps);
}

std::vector<CandlestickData> StockAPI::fetchHistoricalData(const std::string& symbol, int days) {
    std::vector<CandlestickData> result;
    std::string cleanSymbol = toUpper(trim(symbol));
    std::string url = "https://financialmodelingprep.com/api/v3/historical-price-full/" + cleanSymbol + "?apikey=" + API_KEY;
    std::string response = makeHttpRequest(url);
    if (response.empty()) return result;

    json j = json::parse(response, nullptr, false);
    if (!j.is_object() || !j.contains("historical") || !j["historical"].is_array()) return result;
    const auto& arr = j["historical"];   //jun type right hand side ma xa tei type aafu ma aauxa tesaile lamo code bhanda auto use gareko
    int count = 0;
    for (const auto& o : arr) {
        if (count >= days) break;
        std::string date = o.value("date", "");
        float open  = static_cast<float>(o.value("open", 0.0));
        float high  = static_cast<float>(o.value("high", 0.0));
        float low   = static_cast<float>(o.value("low", 0.0));
        float close = static_cast<float>(o.value("close", 0.0));
        float volume= static_cast<float>(o.value("volume", 0.0));
        if (date.empty()) continue;
		result.emplace_back(date, open, high, low, close, volume);  //calls the constructor of CandlestickData
        ++count;
    }
    return result;
}