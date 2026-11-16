#ifndef WASTEREPORT_H
#define WASTEREPORT_H

#include "Waste.h"
#include <string>

class WasteReport : public Waste {
private:
    std::string studentId;
    std::string location;
    std::string date;
    std::string description;
    std::string status; // Reported, Pending, Assigned, Collected, Processed, Closed
    bool verified;

public:
    WasteReport();
    WasteReport(const std::string& id, WasteCategory cat, double qty, const std::string& u,
                const std::string& sid, const std::string& loc, const std::string& dt,
                const std::string& desc, const std::string& st = "Reported", bool ver = false);

    std::string getStudentId() const;
    std::string getLocation() const;
    std::string getDate() const;
    std::string getDescription() const;
    std::string getStatus() const;
    bool isVerified() const;

    bool setStudentId(const std::string& sid);
    bool setLocation(const std::string& loc);
    bool setDate(const std::string& dt);
    bool setDescription(const std::string& desc);
    bool setStatus(const std::string& newStatus);
    void setVerified(bool v);

    static bool isValidStatusTransition(const std::string& from, const std::string& to);
    std::string getSummary() const override;
};

#endif
