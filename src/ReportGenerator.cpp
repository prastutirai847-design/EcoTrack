#include "ReportGenerator.h"
#include <iostream>
#include <iomanip>

double ReportGenerator::totalWaste(const std::vector<WasteReport>& reports) const {
    double total = 0.0;
    for (const auto& r : reports) total += r.getQuantity();
    return total;
}

std::map<std::string, double> ReportGenerator::categoryTotals(const std::vector<WasteReport>& reports) const {
    std::map<std::string, double> m;
    m["Biodegradable"]=0; m["Recyclable"]=0; m["Non-recyclable"]=0; m["Hazardous"]=0; m["Electronic / E-waste"]=0;
    for (const auto& r : reports) m[categoryToString(r.getCategory())] += r.getQuantity();
    return m;
}

double ReportGenerator::recyclingRate(const std::vector<WasteReport>& reports, const std::vector<RecyclingRecord>& recs) const {
    double total = totalWaste(reports);
    if (total <= 0.0) return 0.0;
    double recycled = 0.0;
    for (const auto& r : recs) recycled += r.getRecycledQty();
    return (recycled / total) * 100.0;
}

double ReportGenerator::collectionEfficiency(const std::vector<WasteReport>& reports) const {
    double reported = totalWaste(reports);
    if (reported <= 0.0) return 0.0;
    double collected = 0.0;
    for (const auto& r : reports) {
        const std::string& s = r.getStatus();
        if (s=="Collected"||s=="Processed"||s=="Closed") collected += r.getQuantity();
    }
    return (collected / reported) * 100.0;
}

double ReportGenerator::eWasteHandoverPercentage(const std::vector<EWasteRecord>& recs) const {
    double total = 0.0, handed = 0.0;
    for (const auto& r : recs) {
        total += r.getQuantity();
        if (!r.getHandoverDetails().empty() && r.getHandoverDetails() != "Pending")
            handed += r.getQuantity();
    }
    if (total <= 0.0) return 0.0;
    return (handed / total) * 100.0;
}

double ReportGenerator::wasteReductionPercentage(double previousTotal, double currentTotal) const {
    if (previousTotal <= 0.0) return 0.0;
    return ((previousTotal - currentTotal) / previousTotal) * 100.0;
}

void ReportGenerator::pendingList(const std::vector<WasteReport>& reports, const std::vector<CollectionRequest>& reqs) const {
    std::cout << "\n--- Pending/Incomplete Reports ---\n";
    bool found = false;
    for (const auto& r : reports) {
        if (r.getStatus() != "Closed") {
            std::cout << r.getSummary() << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "None.\n";
    std::cout << "\n--- Pending Collection Requests ---\n";
    found = false;
    for (const auto& q : reqs) {
        if (q.getStatus() != "Closed") {
            std::cout << q.getRequestId() << " | Waste: " << q.getWasteId()
                      << " | Date: " << q.getAssignedDate() << " | Status: " << q.getStatus() << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "None.\n";
}

void ReportGenerator::summary(const std::vector<WasteReport>& reports, const std::vector<RecyclingRecord>& recs,
                              const std::vector<CollectionRequest>& reqs, const std::vector<EWasteRecord>& ewaste,
                              const std::vector<Student>& students) const {
    std::cout << "\n===== Overall Summary Report =====\n";
    double total = totalWaste(reports);
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total waste reported: " << total << " kg\n";
    auto cats = categoryTotals(reports);
    std::cout << "\nCategory-wise waste:\n";
    for (const auto& kv : cats) {
        double pct = (total > 0.0) ? (kv.second / total * 100.0) : 0.0;
        std::cout << "  - " << kv.first << ": " << kv.second << " kg (" << pct << "%)\n";
    }
    std::cout << "\nRecycling rate: " << recyclingRate(reports, recs) << "%\n";
    std::cout << "Collection efficiency: " << collectionEfficiency(reports) << "%\n";
    std::cout << "E-waste handover: " << eWasteHandoverPercentage(ewaste) << "%\n";
    std::cout << "Waste reduction (needs period inputs): use menu option to compute if required.\n";
    double totalPoints = 0.0;
    for (const auto& s : students) totalPoints += s.getRewardPoints();
    std::cout << "Total reward points distributed: " << totalPoints << "\n";
    pendingList(reports, reqs);
}

void ReportGenerator::summary(const std::string& category, const std::vector<WasteReport>& reports,
                              const std::vector<RecyclingRecord>& recs) const {
    std::cout << "\n===== Summary for Category: " << category << " =====\n";
    double totalCat = 0.0;
    for (const auto& r : reports) {
        if (categoryToString(r.getCategory()) == category) totalCat += r.getQuantity();
    }
    double recycledCat = 0.0;
    for (const auto& r : recs) {
        if (categoryToString(r.getCategory()) == category) recycledCat += r.getRecycledQty();
    }
    double rate = (totalCat > 0.0) ? (recycledCat / totalCat * 100.0) : 0.0;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total reported: " << totalCat << " kg\n";
    std::cout << "Total recycled: " << recycledCat << " kg\n";
    std::cout << "Recycling rate: " << rate << "%\n";
}
