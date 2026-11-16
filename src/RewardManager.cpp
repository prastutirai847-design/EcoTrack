#include "RewardManager.h"

RewardManager::RewardManager() {
    pointsPerKg["Recyclable"] = 5.0;
    pointsPerKg["Biodegradable"] = 2.0;
    pointsPerKg["Hazardous"] = 8.0;
    pointsPerKg["Electronic / E-waste"] = 10.0;
    pointsPerKg["Non-recyclable"] = 0.0;
}

void RewardManager::setRate(const std::string& category, double rate) {
    pointsPerKg[category] = rate;
}

double RewardManager::getRate(const std::string& category) const {
    auto it = pointsPerKg.find(category);
    if (it != pointsPerKg.end()) return it->second;
    return 0.0;
}

const std::map<std::string, double>& RewardManager::getRates() const { return pointsPerKg; }

bool RewardManager::isDuplicateReward(const std::string& wasteId) const {
    return rewardedWasteIds.find(wasteId) != rewardedWasteIds.end();
}

double RewardManager::awardPoints(const std::string& wasteId, const std::string& category, double verifiedQuantity) {
    if (isDuplicateReward(wasteId) || verifiedQuantity <= 0) return 0.0;
    double rate = getRate(category);
    double points = verifiedQuantity * rate;
    rewardedWasteIds.insert(wasteId);
    return points;
}

void RewardManager::markRewarded(const std::string& wasteId) {
    rewardedWasteIds.insert(wasteId);
}

const std::set<std::string>& RewardManager::getRewardedIds() const { return rewardedWasteIds; }
