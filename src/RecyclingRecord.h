#ifndef RECYCLINGRECORD_H
#define RECYCLINGRECORD_H

#include "Waste.h"
#include <string>

class RecyclingRecord {
private:
    std::string wasteId;
    WasteCategory category;
    double collectedQty;
    double recycledQty;
    std::string date;

public:
    RecyclingRecord();
    RecyclingRecord(const std::string& id, WasteCategory cat, double collected, double recycled, const std::string& dt);

    std::string getWasteId() const;
    WasteCategory getCategory() const;
    double getCollectedQty() const;
    double getRecycledQty() const;
    std::string getDate() const;
};

#endif
