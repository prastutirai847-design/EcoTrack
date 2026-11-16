#include "EWasteRecord.h"
#include <sstream>
#include <iomanip>

EWasteRecord::EWasteRecord() : Waste("", WasteCategory::ElectronicEwaste, 0.0, "kg"),
    itemType(""), collectionStatus("Reported"), handoverDetails("") {}

EWasteRecord::EWasteRecord(const std::string& id, double qty, const std::string& u,
                           const std::string& type, const std::string& status, const std::string& handover)
    : Waste(id, WasteCategory::ElectronicEwaste, qty, u),
      itemType(type), collectionStatus(status), handoverDetails(handover) {}

std::string EWasteRecord::getItemType() const { return itemType; }
std::string EWasteRecord::getCollectionStatus() const { return collectionStatus; }
std::string EWasteRecord::getHandoverDetails() const { return handoverDetails; }

bool EWasteRecord::setItemType(const std::string& type) {
    if (type.empty()) return false; itemType = type; return true;
}
bool EWasteRecord::setCollectionStatus(const std::string& status) {
    if (status.empty()) return false; collectionStatus = status; return true;
}
bool EWasteRecord::setHandoverDetails(const std::string& details) {
    if (details.empty()) return false; handoverDetails = details; return true;
}

std::string EWasteRecord::getSummary() const {
    std::ostringstream oss;
    oss << wasteId << " | " << itemType << " | " << std::fixed << std::setprecision(2)
        << quantity << " " << unit << " | " << collectionStatus << " | " << handoverDetails;
    return oss.str();
}
