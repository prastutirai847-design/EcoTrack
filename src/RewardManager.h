#ifndef REWARDMANAGER_H
#define REWARDMANAGER_H

#include "Waste.h"
#include <map>
#include <set>
#include <string>

class RewardManager {
private:
    std::map<std::string, double> pointsPerKg;
    std::set<std::string> rewardedWasteIds;

public:
    RewardManager();
    void setRate(const std::string& category, double rate);
    double getRate(const std::string& category) const;
    const std::map<std::string, double>& getRates() const;

    bool isDuplicateReward(const std::string& wasteId) const;
    double awardPoints(const std::string& wasteId, const std::string& category, double verifiedQuantity);
    void markRewarded(const std::string& wasteId);
    const std::set<std::string>& getRewardedIds() const;
};

#endif
