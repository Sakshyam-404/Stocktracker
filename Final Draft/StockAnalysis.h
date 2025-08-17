#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "CandlestickData.h"

namespace StockAnalysis {
	// Core indicators
	double calculateMACD(const std::vector<double>& prices);
	double calculateRSI(const std::vector<CandlestickData>& candlesticks);
	double calculateRelativeVolume(const std::vector<double>& volumes);

	// Scoring helpers
	int calculateMACDScore(double macdValue, const std::vector<double>& prices);
	int calculateRSIScore(double rsiValue);
	int calculateVolumeScore(double relativeVolume, const std::vector<double>& prices);
	int calculatePositionScore(double position52Week);
	int calculatePEScore(double peRatio, double industryPE);
	int calculateEPSScore(double earningsPerShare);
	double calculateOverallScore(int macdScore, int rsiScore, int volumeScore,
		int positionScore, int peScore, int epsScore);
	std::string getRecommendation(double overallScore);

	// Rendering helpers
	void appendAnalysisResults(std::ostringstream& output, const std::string& symbol,
		double macdValue, double rsiValue, double relativeVolume,
		double position52Week, double peRatio, double industryPE,
		double earningsPerShare, int macdScore, int rsiScore,
		int volumeScore, int positionScore, int peScore, int epsScore,
		double overallScore, const std::string& recommendation);
}


