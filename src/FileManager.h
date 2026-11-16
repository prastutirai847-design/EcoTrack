#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Student.h"
#include "WasteReport.h"
#include "CollectionRequest.h"
#include "RecyclingRecord.h"
#include "EWasteRecord.h"
#include "RewardManager.h"
#include <vector>
#include <string>

class FileManager {
private:
    std::string dataDir;
    std::string sanitizeField(const std::string& s) const;
public:
    explicit FileManager(const std::string& dir = "data");
    void ensureFilesExist() const;

    std::vector<Student> loadStudents() const;
    std::vector<WasteReport> loadReports() const;
    std::vector<CollectionRequest> loadCollections() const;
    std::vector<RecyclingRecord> loadRecycling() const;
    std::vector<EWasteRecord> loadEWaste() const;
    void loadRewards(RewardManager& rm) const;

    void saveStudents(const std::vector<Student>& students) const;
    void saveReports(const std::vector<WasteReport>& reports) const;
    void saveCollections(const std::vector<CollectionRequest>& requests) const;
    void saveRecycling(const std::vector<RecyclingRecord>& records) const;
    void saveEWaste(const std::vector<EWasteRecord>& records) const;
    void saveRewards(const RewardManager& rm) const;
};

#endif
