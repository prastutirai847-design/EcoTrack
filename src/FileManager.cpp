#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

FileManager::FileManager(const std::string& dir) : dataDir(dir) {}

std::string FileManager::sanitizeField(const std::string& s) const {
    std::string out = s;
    std::replace(out.begin(), out.end(), '|', '/');
    std::replace(out.begin(), out.end(), '\n', ' ');
    std::replace(out.begin(), out.end(), '\r', ' ');
    return out;
}

void FileManager::ensureFilesExist() const {
    const char* files[] = {"students.txt","reports.txt","collections.txt","recycling.txt","ewaste.txt"};
    for (auto f : files) {
        std::ifstream in(dataDir + "/" + f);
        if (!in.good()) {
            std::ofstream out(dataDir + "/" + f);
        }
    }
    std::ifstream rcfg(dataDir + "/rewards.cfg");
    if (!rcfg.good()) {
        std::ofstream out(dataDir + "/rewards.cfg");
        out << "Recyclable|5\nBiodegradable|2\nHazardous|8\nElectronic / E-waste|10\nNon-recyclable|0\n";
    }
}

std::vector<Student> FileManager::loadStudents() const {
    std::vector<Student> v;
    std::ifstream in(dataDir + "/students.txt");
    if (!in) return v;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id,name,dept,semStr,pointsStr;
        if (!std::getline(ss,id,'|')) continue;
        if (!std::getline(ss,name,'|')) continue;
        if (!std::getline(ss,dept,'|')) continue;
        if (!std::getline(ss,semStr,'|')) continue;
        if (!std::getline(ss,pointsStr,'|')) pointsStr="0";
        try {
            int sem = std::stoi(semStr);
            double pts = std::stod(pointsStr);
            v.emplace_back(id,name,dept,sem,pts);
        } catch (...) { continue; }
    }
    return v;
}

std::vector<WasteReport> FileManager::loadReports() const {
    std::vector<WasteReport> v;
    std::ifstream in(dataDir + "/reports.txt");
    if (!in) return v;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string wid,sid,cat,qtyStr,unit,loc,date,desc,status,verStr;
        if (!std::getline(ss,wid,'|')) continue;
        if (!std::getline(ss,sid,'|')) continue;
        if (!std::getline(ss,cat,'|')) continue;
        if (!std::getline(ss,qtyStr,'|')) continue;
        if (!std::getline(ss,unit,'|')) continue;
        if (!std::getline(ss,loc,'|')) continue;
        if (!std::getline(ss,date,'|')) continue;
        if (!std::getline(ss,desc,'|')) continue;
        if (!std::getline(ss,status,'|')) continue;
        if (!std::getline(ss,verStr,'|')) verStr="0";
        WasteCategory wc;
        if (!stringToCategory(cat, wc)) continue;
        try {
            double qty = std::stod(qtyStr);
            bool ver = (verStr == "1" || verStr == "true" || verStr == "Verified");
            v.emplace_back(wid,wc,qty,unit,sid,loc,date,desc,status,ver);
        } catch (...) { continue; }
    }
    return v;
}

std::vector<CollectionRequest> FileManager::loadCollections() const {
    std::vector<CollectionRequest> v;
    std::ifstream in(dataDir + "/collections.txt");
    if (!in) return v;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string rid,wid,date,status;
        if (!std::getline(ss,rid,'|')) continue;
        if (!std::getline(ss,wid,'|')) continue;
        if (!std::getline(ss,date,'|')) continue;
        if (!std::getline(ss,status,'|')) continue;
        v.emplace_back(rid,wid,date,status);
    }
    return v;
}

std::vector<RecyclingRecord> FileManager::loadRecycling() const {
    std::vector<RecyclingRecord> v;
    std::ifstream in(dataDir + "/recycling.txt");
    if (!in) return v;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string wid,cat,cqStr,rqStr,date;
        if (!std::getline(ss,wid,'|')) continue;
        if (!std::getline(ss,cat,'|')) continue;
        if (!std::getline(ss,cqStr,'|')) continue;
        if (!std::getline(ss,rqStr,'|')) continue;
        if (!std::getline(ss,date,'|')) continue;
        WasteCategory wc;
        if (!stringToCategory(cat,wc)) continue;
        try {
            v.emplace_back(wid,wc,std::stod(cqStr),std::stod(rqStr),date);
        } catch (...) { continue; }
    }
    return v;
}

std::vector<EWasteRecord> FileManager::loadEWaste() const {
    std::vector<EWasteRecord> v;
    std::ifstream in(dataDir + "/ewaste.txt");
    if (!in) return v;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string wid,type,qtyStr,status,handover;
        if (!std::getline(ss,wid,'|')) continue;
        if (!std::getline(ss,type,'|')) continue;
        if (!std::getline(ss,qtyStr,'|')) continue;
        if (!std::getline(ss,status,'|')) continue;
        if (!std::getline(ss,handover,'|')) handover="";
        try {
            v.emplace_back(wid,std::stod(qtyStr),"unit(s)",type,status,handover);
        } catch (...) { continue; }
    }
    return v;
}

void FileManager::loadRewards(RewardManager& rm) const {
    std::ifstream in(dataDir + "/rewards.cfg");
    if (!in) return;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string cat, rateStr;
        if (!std::getline(ss,cat,'|')) continue;
        if (!std::getline(ss,rateStr,'|')) continue;
        try { rm.setRate(cat, std::stod(rateStr)); } catch (...) {}
    }
}

void FileManager::saveStudents(const std::vector<Student>& students) const {
    std::ofstream out(dataDir + "/students.txt");
    for (const auto& s : students) {
        out << sanitizeField(s.getUserId()) << "|" << sanitizeField(s.getName()) << "|"
            << sanitizeField(s.getDepartment()) << "|" << s.getSemester() << "|"
            << s.getRewardPoints() << "\n";
    }
}

void FileManager::saveReports(const std::vector<WasteReport>& reports) const {
    std::ofstream out(dataDir + "/reports.txt");
    for (const auto& r : reports) {
        out << sanitizeField(r.getWasteId()) << "|" << sanitizeField(r.getStudentId()) << "|"
            << categoryToString(r.getCategory()) << "|" << r.getQuantity() << "|"
            << sanitizeField(r.getUnit()) << "|" << sanitizeField(r.getLocation()) << "|"
            << sanitizeField(r.getDate()) << "|" << sanitizeField(r.getDescription()) << "|"
            << sanitizeField(r.getStatus()) << "|" << (r.isVerified() ? "1" : "0") << "\n";
    }
}

void FileManager::saveCollections(const std::vector<CollectionRequest>& requests) const {
    std::ofstream out(dataDir + "/collections.txt");
    for (const auto& r : requests) {
        out << sanitizeField(r.getRequestId()) << "|" << sanitizeField(r.getWasteId()) << "|"
            << sanitizeField(r.getAssignedDate()) << "|" << sanitizeField(r.getStatus()) << "\n";
    }
}

void FileManager::saveRecycling(const std::vector<RecyclingRecord>& records) const {
    std::ofstream out(dataDir + "/recycling.txt");
    for (const auto& r : records) {
        out << sanitizeField(r.getWasteId()) << "|" << categoryToString(r.getCategory()) << "|"
            << r.getCollectedQty() << "|" << r.getRecycledQty() << "|"
            << sanitizeField(r.getDate()) << "\n";
    }
}

void FileManager::saveEWaste(const std::vector<EWasteRecord>& records) const {
    std::ofstream out(dataDir + "/ewaste.txt");
    for (const auto& r : records) {
        out << sanitizeField(r.getWasteId()) << "|" << sanitizeField(r.getItemType()) << "|"
            << r.getQuantity() << "|" << sanitizeField(r.getCollectionStatus()) << "|"
            << sanitizeField(r.getHandoverDetails()) << "\n";
    }
}

void FileManager::saveRewards(const RewardManager& rm) const {
    std::ofstream out(dataDir + "/rewards.cfg");
    for (const auto& kv : rm.getRates()) {
        out << kv.first << "|" << kv.second << "\n";
    }
}
