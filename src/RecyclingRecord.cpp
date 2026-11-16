#include "RecyclingRecord.h"

RecyclingRecord::RecyclingRecord() : wasteId(""), category(WasteCategory::Recyclable),
    collectedQty(0.0), recycledQty(0.0), date("") {}

RecyclingRecord::RecyclingRecord(const std::string& id, WasteCategory cat, double collected, double recycled, const std::string& dt)
    : wasteId(id), category(cat), collectedQty(collected), recycledQty(recycled), date(dt) {}

std::string RecyclingRecord::getWasteId() const { return wasteId; }
WasteCategory RecyclingRecord::getCategory() const { return category; }
double RecyclingRecord::getCollectedQty() const { return collectedQty; }
double RecyclingRecord::getRecycledQty() const { return recycledQty; }
std::string RecyclingRecord::getDate() const { return date; }
