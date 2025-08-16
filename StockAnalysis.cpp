#include "StockAnalysis.h"
#include <numeric>
#include <iomanip>

namespace {
	static std::string formatNumber(double value) {
		std::ostringstream stream;
		stream << std::fixed << std::setprecision(2) << value;
		return stream.str();
	}
}

namespace StockAnalysis {

	double calculateMACD(const std::vector<double>& prices) {
		if (prices.size() < 26) {
			return 0.0;
		}
		double ema12 = prices.front();
		double ema26 = prices.front();
		const double multiplier12 = 2.0 / (12.0 + 1.0);
		const double multiplier26 = 2.0 / (26.0 + 1.0);
		for (size_t i = 1; i < prices.size(); ++i) {
			ema12 = (prices[i] * multiplier12) + (ema12 * (1 - multiplier12));
			ema26 = (prices[i] * multiplier26) + (ema26 * (1 - multiplier26));
		}
		return ema12 - ema26;
	}

	double calculateRSI(const std::vector<double>& prices) {
		if (prices.size() < 14) {
			return 50.0;
		}
		std::vector<double> gains;
		std::vector<double> losses;
		gains.reserve(prices.size());
		losses.reserve(prices.size());
		for (size_t i = 1; i < prices.size(); ++i) {
			double priceChange = prices[i] - prices[i - 1];
			if (priceChange > 0) {
				gains.push_back(priceChange);
				losses.push_back(0.0);
			} else {
				gains.push_back(0.0);
				losses.push_back(-priceChange);
			}
		}
		if (gains.size() < 14) {
			return 50.0;
		}
		double averageGain = std::accumulate(gains.begin(), gains.begin() + 14, 0.0) / 14.0;
		double averageLoss = std::accumulate(losses.begin(), losses.begin() + 14, 0.0) / 14.0;
		if (averageLoss == 0.0) {
			return 100.0;
		}
		double relativeStrength = averageGain / averageLoss;
		return 100.0 - (100.0 / (1.0 + relativeStrength));
	}

	double calculateRelativeVolume(const std::vector<double>& volumes) {
		if (volumes.size() < 20) {
			return 1.0;
		}
		double averageVolume = std::accumulate(volumes.begin(), volumes.begin() + 20, 0.0) / 20.0;
		if (averageVolume == 0.0) {
			return 1.0;
		}
		return volumes.front() / averageVolume;
	}

	int calculateMACDScore(double macdValue, const std::vector<double>& prices) {
		if (prices.size() < 2) return 0;
		std::vector<double> previousPrices = prices;
		previousPrices.pop_back();
		double previousMACD = calculateMACD(previousPrices);
		double signalLine = (macdValue + previousMACD) / 2.0;
		if (previousMACD <= signalLine && macdValue > signalLine) {
			return 2;
		} else if (previousMACD >= signalLine && macdValue < signalLine) {
			return -2;
		} else if (macdValue > signalLine) {
			return 1;
		} else if (macdValue < signalLine) {
			return -1;
		}
		return 0;
	}

	int calculateRSIScore(double rsiValue) {
		if (rsiValue < 20) return 2;
		if (rsiValue < 30) return 1;
		if (rsiValue > 80) return -2;
		if (rsiValue > 70) return -1;
		return 0;
	}

	int calculateVolumeScore(double relativeVolume, const std::vector<double>& prices) {
		if (prices.size() < 2) return 0;
		double priceChange = prices.back() - prices[prices.size() - 2];
		if (relativeVolume > 1.5 && priceChange > 0) {
			return 1;
		} else if (relativeVolume > 1.5 && priceChange < 0) {
			return -1;
		}
		return 0;
	}

	int calculatePositionScore(double position52Week) {
		if (position52Week < 10) return 2;
		if (position52Week < 30) return 1;
		if (position52Week > 90) return -2;
		if (position52Week > 70) return -1;
		return 0;
	}

	int calculatePEScore(double peRatio, double industryPE) {
		if (peRatio < industryPE * 0.6) return 2;
		if (peRatio < industryPE * 0.8) return 1;
		if (peRatio > industryPE * 1.4) return -2;
		if (peRatio > industryPE * 1.2) return -1;
		return 0;
	}

	int calculateEPSScore(double earningsPerShare) {
		return earningsPerShare > 0.0 ? 1 : -1;
	}

	double calculateOverallScore(int macdScore, int rsiScore, int volumeScore,
								 int positionScore, int peScore, int epsScore) {
		const double weightMACD = 0.25;
		const double weightRSI = 0.15;
		const double weightVolume = 0.10;
		const double weightPosition = 0.15;
		const double weightPE = 0.20;
		const double weightEPS = 0.15;
		return weightMACD * macdScore + weightRSI * rsiScore + weightVolume * volumeScore +
			   weightPosition * positionScore + weightPE * peScore + weightEPS * epsScore;
	}

	std::string getRecommendation(double overallScore) {
		if (overallScore >= 0.8) {
			return "BUY";
		} else if (overallScore <= -0.8) {
			return "SELL";
		} else {
			return "HOLD";
		}
	}

	void appendAnalysisResults(std::ostringstream& output, const std::string& symbol,
							   double macdValue, double rsiValue, double relativeVolume,
							   double position52Week, double peRatio, double industryPE,
							   double earningsPerShare, int macdScore, int rsiScore,
							   int volumeScore, int positionScore, int peScore, int epsScore,
							   double overallScore, const std::string& recommendation) {
		output << std::setw(20) << "MACD" << " | " << std::setw(15) << formatNumber(macdValue) << " | ";
		if (macdScore == 2) {
			output << std::setw(40) << "Bullish crossover" << " | ";
		} else if (macdScore == -2) {
			output << std::setw(40) << "Bearish crossover" << " | ";
		} else if (macdScore == 1) {
			output << std::setw(40) << "Above signal line" << " | ";
		} else if (macdScore == -1) {
			output << std::setw(40) << "Below signal line" << " | ";
		} else {
			output << std::setw(40) << "At signal line" << " | ";
		}
		output << std::setw(30) << std::to_string(macdScore) << "\n";

		output << std::setw(20) << "RSI" << " | " << std::setw(15) << formatNumber(rsiValue) << " | ";
		output << std::setw(40) << "Momentum indicator" << " | ";
		output << std::setw(30) << std::to_string(rsiScore) << "\n";

		output << std::setw(20) << "RVOL" << " | " << std::setw(15) << formatNumber(relativeVolume) << " | ";
		output << std::setw(40) << (relativeVolume > 1.5 ? "High volume" : "Normal volume") << " | ";
		output << std::setw(30) << std::to_string(volumeScore) << "\n";

		output << std::setw(20) << "52W Pos" << " | " << std::setw(15) << formatNumber(position52Week) + "%" << " | ";
		output << std::setw(40) << "52-week position" << " | ";
		output << std::setw(30) << std::to_string(positionScore) << "\n";

		output << std::setw(20) << "P/E" << " | " << std::setw(15) << formatNumber(peRatio) + " vs " + formatNumber(industryPE) << " | ";
		output << std::setw(40) << "Valuation" << " | ";
		output << std::setw(30) << std::to_string(peScore) << "\n";

		output << std::setw(20) << "EPS" << " | " << std::setw(15) << "$" + formatNumber(earningsPerShare) << " | ";
		output << std::setw(40) << (earningsPerShare > 0.01 ? "Profitable" : (earningsPerShare < -0.01 ? "Loss-making" : "Break-even")) << " | ";
		output << std::setw(30) << std::to_string(epsScore) << "\n";

		output << "Weighted Score: " << formatNumber(overallScore) << "\n";
		output << "OVERALL: " << recommendation << "\n\n";
	}
}


