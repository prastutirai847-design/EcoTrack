#include "WasteReport.h"
#include <sstream>
#include <iomanip>
#include <vector>

WasteReport::WasteReport() : Waste(), studentId(""), location(""), date(""), description(""),
    status("Reported"), verified(false) {}

WasteReport::WasteReport(const std::string& id, WasteCategory cat, double qty, const std::string& u,
                         const std::string& sid, const std::string& loc, const std::string& dt,
                         const std::string& desc, const std::string& st, bool ver)
    : Waste(id, cat, qty, u), studentId(sid), location(loc), date(dt), description(desc),
      status(st), verified(ver) {}

std::string WasteReport::getStudentId() const { return studentId; }
std::string WasteReport::getLocation() const { return location; }
std::string WasteReport::getDate() const { return date; }
std::string WasteReport::getDescription() const { return description; }
std::string WasteReport::getStatus() const { return status; }
bool WasteReport::isVerified() const { return verified; }

bool WasteReport::setStudentId(const std::string& sid) {
    if (sid.empty()) return false;
    studentId = sid; return true;
}
bool WasteReport::setLocation(const std::string& loc) {
    if (loc.empty()) return false;
    location = loc; return true;
}
bool WasteReport::setDate(const std::string& dt) {
    if (dt.empty()) return false;
    date = dt; return true;
}
bool WasteReport::setDescription(const std::string& desc) {
    if (desc.empty()) return false;
    description = desc; return true;
}
void WasteReport::setVerified(bool v) { verified = v; }

bool WasteReport::isValidStatusTransition(const std::string& from, const std::string& to) {
    const std::vector<std::string> seq = {"Reported","Pending","Assigned","Collected","Processed","Closed"};
    int fi = -1, ti = -1;
    for (size_t i = 0; i < seq.size(); ++i) {
        if (seq[i] == from) fi = (int)i;
        if (seq[i] == to) ti = (int)i;
    }
    if (fi == -1 || ti == -1) return false;
    return ti == fi + 1;
}

bool WasteReport::setStatus(const std::string& newStatus) {
    if (isValidStatusTransition(status, newStatus)) {
        status = newStatus;
        return true;
    }
    return false;
}

std::string WasteReport::getSummary() const {
    std::ostringstream oss;
    oss << wasteId << " | " << studentId << " | " << categoryToString(category)
        << " | " << std::fixed << std::setprecision(2) << quantity << " " << unit
        << " | " << location << " | " << date << " | " << status
        << " | " << (verified ? "Verified" : "Not Verified");
    return oss.str();
}
