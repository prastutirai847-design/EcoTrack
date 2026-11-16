#include "Waste.h"
#include <sstream>
#include <iomanip>

std::string categoryToString(WasteCategory category) {
    switch (category) {
        case WasteCategory::Biodegradable: return "Biodegradable";
        case WasteCategory::Recyclable: return "Recyclable";
        case WasteCategory::NonRecyclable: return "Non-recyclable";
        case WasteCategory::Hazardous: return "Hazardous";
        case WasteCategory::ElectronicEwaste: return "Electronic / E-waste";
        default: return "Unknown";
    }
}

bool stringToCategory(const std::string& str, WasteCategory& out) {
    if (str == "Biodegradable" || str == "1") { out = WasteCategory::Biodegradable; return true; }
    if (str == "Recyclable" || str == "2") { out = WasteCategory::Recyclable; return true; }
    if (str == "Non-recyclable" || str == "NonRecyclable" || str == "3") { out = WasteCategory::NonRecyclable; return true; }
    if (str == "Hazardous" || str == "4") { out = WasteCategory::Hazardous; return true; }
    if (str == "Electronic / E-waste" || str == "ElectronicEwaste" || str == "E-waste" || str == "5") {
        out = WasteCategory::ElectronicEwaste; return true;
    }
    return false;
}

Waste::Waste() : wasteId(""), category(WasteCategory::Biodegradable), quantity(0.0), unit("kg") {}

Waste::Waste(const std::string& id, WasteCategory cat, double qty, const std::string& u)
    : wasteId(id), category(cat), quantity(qty), unit(u) {}

Waste::~Waste() {}

std::string Waste::getWasteId() const { return wasteId; }
WasteCategory Waste::getCategory() const { return category; }
double Waste::getQuantity() const { return quantity; }
std::string Waste::getUnit() const { return unit; }

bool Waste::setWasteId(const std::string& id) {
    if (id.empty()) return false;
    wasteId = id;
    return true;
}

bool Waste::setCategory(WasteCategory cat) {
    category = cat;
    return true;
}

bool Waste::setQuantity(double qty) {
    if (qty <= 0.0) return false;
    quantity = qty;
    return true;
}

bool Waste::setUnit(const std::string& u) {
    if (u.empty()) return false;
    unit = u;
    return true;
}

std::string Waste::getSummary() const {
    std::ostringstream oss;
    oss << wasteId << " | " << categoryToString(category) << " | "
        << std::fixed << std::setprecision(2) << quantity << " " << unit;
    return oss.str();
}
