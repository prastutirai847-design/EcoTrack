#ifndef WASTE_H
#define WASTE_H

#include <string>

enum class WasteCategory {
    Biodegradable,
    Recyclable,
    NonRecyclable,
    Hazardous,
    ElectronicEwaste
};

std::string categoryToString(WasteCategory category);
bool stringToCategory(const std::string& str, WasteCategory& out);

class Waste {
protected:
    std::string wasteId;
    WasteCategory category;
    double quantity;
    std::string unit;

public:
    Waste();
    Waste(const std::string& id, WasteCategory cat, double qty, const std::string& u);
    virtual ~Waste();

    std::string getWasteId() const;
    WasteCategory getCategory() const;
    double getQuantity() const;
    std::string getUnit() const;

    bool setWasteId(const std::string& id);
    bool setCategory(WasteCategory cat);
    bool setQuantity(double qty);
    bool setUnit(const std::string& u);

    virtual std::string getSummary() const;
};

#endif
