#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include "WasteReport.h"
#include "RecyclingRecord.h"
#include "CollectionRequest.h"
#include "EWasteRecord.h"
#include "Student.h"
#include <vector>
#include <map>
#include <string>

class ReportGenerator {
public:
    double totalWaste(const std::vector<WasteReport>& reports) const;
    std::map<std::string, double> categoryTotals(const std::vector<WasteReport>& reports) const;
    double recyclingRate(const std::vector<WasteReport>& reports, const std::vector<RecyclingRecord>& recs) const;
    double collectionEfficiency(const std::vector<WasteReport>& reports) const;
    double eWasteHandoverPercentage(const std::vector<EWasteRecord>& recs) const;
    double wasteReductionPercentage(double previousTotal, double currentTotal) const;
    void pendingList(const std::vector<WasteReport>& reports, const std::vector<CollectionRequest>& reqs) const;

    void summary(const std::vector<WasteReport>& reports, const std::vector<RecyclingRecord>& recs,
                 const std::vector<CollectionRequest>& reqs, const std::vector<EWasteRecord>& ewaste,
                 const std::vector<Student>& students) const;
    void summary(const std::string& category, const std::vector<WasteReport>& reports,
                 const std::vector<RecyclingRecord>& recs) const;
};

#endif
