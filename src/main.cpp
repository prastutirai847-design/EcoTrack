#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <sstream>
#include <set>
#include "Student.h"
#include "Admin.h"
#include "WasteReport.h"
#include "CollectionManager.h"
#include "RecyclingRecord.h"
#include "EWasteRecord.h"
#include "RewardManager.h"
#include "ReportGenerator.h"
#include "FileManager.h"

using namespace std;

static string trim(const string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

static bool containsPipe(const string& s) {
    return s.find('|') != string::npos;
}

static bool readLineSafe(const string& prompt, string& out) {
    cout << prompt;
    if (!getline(cin, out)) return false;
    out = trim(out);
    return true;
}

static bool readInt(const string& prompt, int& out) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) return false;
    stringstream ss(trim(line));
    int val; char extra;
    if (!(ss >> val) || (ss >> extra)) return false;
    out = val; return true;
}

static bool readDouble(const string& prompt, double& out) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) return false;
    stringstream ss(trim(line));
    double val; char extra;
    if (!(ss >> val) || (ss >> extra)) return false;
    out = val; return true;
}

static string generateWasteId(const vector<WasteReport>& reports, const vector<EWasteRecord>& ewaste) {
    int maxId = 0;
    auto scan = [&](const string& id) {
        if (id.size() > 1 && (id[0]=='W' || id[0]=='w')) {
            try { int n = stoi(id.substr(1)); if (n > maxId) maxId = n; } catch(...) {}
        }
    };
    for (auto& r : reports) scan(r.getWasteId());
    for (auto& e : ewaste) scan(e.getWasteId());
    return "W" + to_string(maxId + 1);
}

static Student* findStudent(vector<Student>& students, const string& id) {
    for (auto& s : students) if (s.getUserId() == id) return &s;
    return nullptr;
}

static WasteReport* findReport(vector<WasteReport>& reports, const string& id) {
    for (auto& r : reports) if (r.getWasteId() == id) return &r;
    return nullptr;
}

static void saveAll(FileManager& fm, vector<Student>& students, vector<WasteReport>& reports,
                    CollectionManager& cm, vector<RecyclingRecord>& recycling,
                    vector<EWasteRecord>& ewaste, RewardManager& rm) {
    fm.saveStudents(students);
    fm.saveReports(reports);
    fm.saveCollections(cm.getRequests());
    fm.saveRecycling(recycling);
    fm.saveEWaste(ewaste);
    fm.saveRewards(rm);
}

static void studentProfile(Student& s) {
    cout << "\n--- Profile ---\n";
    cout << "ID: " << s.getUserId() << "\nName: " << s.getName()
         << "\nDepartment: " << s.getDepartment() << "\nSemester: " << s.getSemester()
         << "\nReward Points: " << fixed << setprecision(2) << s.getRewardPoints() << "\n";
}

static void viewMyReports(const vector<WasteReport>& reports, const string& studentId) {
    cout << "\n--- My Reports ---\n";
    bool any = false;
    for (auto& r : reports) {
        if (r.getStudentId() == studentId) { cout << r.getSummary() << "\n"; any = true; }
    }
    if (!any) cout << "No reports found.\n";
}

static void submitWasteReport(vector<Student>& students, vector<WasteReport>& reports,
                              vector<EWasteRecord>& ewaste, FileManager& fm) {
    string sid, catStr, unit, loc, date, desc;
    if (!readLineSafe("Enter your Student ID: ", sid) || sid.empty()) { cout << "Invalid Student ID.\n"; return; }
    if (!findStudent(students, sid)) { cout << "Student not registered.\n"; return; }
    cout << "Select Waste Category:\n1. Biodegradable\n2. Recyclable\n3. Non-recyclable\n4. Hazardous\n5. Electronic / E-waste\n";
    if (!readLineSafe("Choice (1-5): ", catStr)) { cout << "Invalid input.\n"; return; }
    WasteCategory cat;
    if (!stringToCategory(catStr, cat)) { cout << "Invalid category.\n"; return; }
    double qty;
    if (!readDouble("Enter quantity: ", qty) || qty <= 0.0) { cout << "Quantity must be greater than zero.\n"; return; }
    if (!readLineSafe("Enter unit (e.g., kg): ", unit) || unit.empty() || containsPipe(unit)) { cout << "Invalid unit.\n"; return; }
    if (!readLineSafe("Enter campus location: ", loc) || loc.empty() || containsPipe(loc)) { cout << "Invalid location.\n"; return; }
    if (!readLineSafe("Enter date (YYYY-MM-DD): ", date) || date.empty() || containsPipe(date)) { cout << "Invalid date.\n"; return; }
    if (!readLineSafe("Enter short description: ", desc) || desc.empty() || containsPipe(desc)) { cout << "Invalid description (and '|' not allowed).\n"; return; }

    bool possibleDup = false;
    for (auto& r : reports) {
        if (r.getStudentId()==sid && r.getCategory()==cat && r.getQuantity()==qty &&
            r.getLocation()==loc && r.getDate()==date) { possibleDup = true; break; }
    }
    string wid = generateWasteId(reports, ewaste);
    WasteReport wr(wid, cat, qty, unit, sid, loc, date, desc, "Reported", false);
    reports.push_back(wr);
    fm.saveReports(reports);
    cout << "Waste report submitted. Waste ID: " << wid << " | Status: Reported\n";
    if (possibleDup) {
        cout << "WARNING: Possible duplicate report detected (same student/category/quantity/location/date). Flagged for administrator review.\n";
    }
}

static void studentMenu(Student& s, vector<Student>& students, vector<WasteReport>& reports,
                        vector<EWasteRecord>& ewaste, FileManager& fm) {
    while (true) {
        s.displayMenu();
        int choice;
        if (!readInt("Enter choice: ", choice)) { cout << "Invalid input.\n"; continue; }
        switch (choice) {
            case 1: studentProfile(s); break;
            case 2: submitWasteReport(students, reports, ewaste, fm); break;
            case 3: viewMyReports(reports, s.getUserId()); break;
            case 4: cout << "Total Reward Points: " << fixed << setprecision(2) << s.getRewardPoints() << "\n"; break;
            case 5: cout << "Logging out.\n"; return;
            default: cout << "Invalid choice.\n";
        }
    }
}

static void viewAllStudents(const vector<Student>& students) {
    cout << "\n--- All Students ---\n";
    if (students.empty()) { cout << "No students registered.\n"; return; }
    for (auto& s : students) {
        cout << s.getUserId() << " | " << s.getName() << " | " << s.getDepartment()
             << " | Sem " << s.getSemester() << " | Points: " << fixed << setprecision(2) << s.getRewardPoints() << "\n";
    }
}

static void viewAllReports(const vector<WasteReport>& reports) {
    cout << "\n--- All Waste Reports ---\n";
    if (reports.empty()) { cout << "No reports.\n"; return; }
    for (auto& r : reports) cout << r.getSummary() << "\nDescription: " << r.getDescription() << "\n";
}

static void searchReportById(vector<WasteReport>& reports) {
    string id; if (!readLineSafe("Enter Waste ID: ", id)) return;
    WasteReport* r = findReport(reports, id);
    if (!r) { cout << "Report not found.\n"; return; }
    cout << r->getSummary() << "\nDescription: " << r->getDescription() << "\n";
}

static void createCollectionReq(vector<WasteReport>& reports, CollectionManager& cm, FileManager& fm) {
    string wid; if (!readLineSafe("Enter Waste ID to create collection request: ", wid)) return;
    WasteReport* r = findReport(reports, wid);
    if (!r) { cout << "Waste report does not exist.\n"; return; }
    if (cm.findByWasteId(wid)) { cout << "Collection request already exists for this Waste ID.\n"; return; }
    string date; if (!readLineSafe("Enter assigned date (YYYY-MM-DD): ", date) || date.empty() || containsPipe(date)) { cout << "Invalid date.\n"; return; }
    // move report Reported -> Pending
    if (r->getStatus() == "Reported") {
        if (!r->setStatus("Pending")) { cout << "Cannot move report to Pending.\n"; return; }
    } else if (r->getStatus() != "Pending") {
        cout << "Request can be created when report is Reported/Pending. Current: " << r->getStatus() << "\n";
        return;
    }
    string reqId = cm.generateRequestId();
    cm.addRequest(CollectionRequest(reqId, wid, date, "Pending"));
    fm.saveCollections(cm.getRequests());
    fm.saveReports(reports);
    cout << "Collection request created: " << reqId << " for " << wid << " (Pending)\n";
}

static void viewPendingCollections(CollectionManager& cm) {
    cout << "\n--- Pending Collection Requests ---\n";
    auto pending = cm.getPending();
    if (pending.empty()) { cout << "None.\n"; return; }
    for (auto& r : pending) {
        cout << r.getRequestId() << " | Waste: " << r.getWasteId()
             << " | Date: " << r.getAssignedDate() << " | Status: " << r.getStatus() << "\n";
    }
}

static void updateStatusFlow(vector<WasteReport>& reports, CollectionManager& cm, FileManager& fm) {
    string wid; if (!readLineSafe("Enter Waste ID: ", wid)) return;
    WasteReport* r = findReport(reports, wid);
    if (!r) { cout << "Report not found.\n"; return; }
    CollectionRequest* req = cm.findByWasteId(wid);
    cout << "Current report status: " << r->getStatus() << "\n";
    if (req) cout << "Current collection request status: " << req->getStatus() << "\n";
    cout << "Allowed next statuses from current: ";
    const vector<string> seq = {"Reported","Pending","Assigned","Collected","Processed","Closed"};
    for (size_t i=0;i+1<seq.size();++i) if (seq[i]==r->getStatus()) cout << seq[i+1];
    cout << "\nEnter new status: ";
    string ns; if (!getline(cin, ns)) return; ns = trim(ns);
    if (!r->setStatus(ns)) { cout << "Invalid transition. Status cannot skip stages or move backwards.\n"; return; }
    if (req) {
        // keep collection status in sync if valid; if not, set to same as report when possible
        if (req->getStatus() != ns) {
            if (!req->setStatus(ns)) {
                // try force sync for cases where request started at Pending but report was moved in order
                req->forceStatus(ns);
            }
        }
    }
    fm.saveReports(reports);
    fm.saveCollections(cm.getRequests());
    cout << "Status updated to " << ns << ".\n";
}

static void verifyReport(vector<WasteReport>& reports, FileManager& fm) {
    string wid; if (!readLineSafe("Enter Waste ID to verify: ", wid)) return;
    WasteReport* r = findReport(reports, wid);
    if (!r) { cout << "Report not found.\n"; return; }
    r->setVerified(true);
    fm.saveReports(reports);
    cout << "Report " << wid << " marked as verified.\n";
}

static void recordRecycling(vector<WasteReport>& reports, vector<RecyclingRecord>& recycling,
                            vector<Student>& students, RewardManager& rm, FileManager& fm) {
    string wid; if (!readLineSafe("Enter Waste ID: ", wid)) return;
    WasteReport* r = findReport(reports, wid);
    if (!r) { cout << "Report not found.\n"; return; }
    double collected, recycled;
    if (!readDouble("Enter collected quantity: ", collected) || collected < 0.0) { cout << "Invalid collected quantity.\n"; return; }
    if (!readDouble("Enter actually recycled quantity: ", recycled) || recycled < 0.0) { cout << "Invalid recycled quantity.\n"; return; }
    if (recycled > collected) { cout << "Recycled quantity cannot exceed collected quantity.\n"; return; }
    string date; if (!readLineSafe("Enter date (YYYY-MM-DD): ", date) || date.empty() || containsPipe(date)) { cout << "Invalid date.\n"; return; }
    recycling.emplace_back(wid, r->getCategory(), collected, recycled, date);
    // update report status to Processed if appropriate (must follow sequence)
    if (r->getStatus() == "Collected") {
        r->setStatus("Processed");
    } else if (r->getStatus() == "Reported") {
        // try to advance stepwise to Processed
        r->setStatus("Pending"); r->setStatus("Assigned"); r->setStatus("Collected"); r->setStatus("Processed");
    } else if (r->getStatus() == "Pending") {
        r->setStatus("Assigned"); r->setStatus("Collected"); r->setStatus("Processed");
    } else if (r->getStatus() == "Assigned") {
        r->setStatus("Collected"); r->setStatus("Processed");
    }
    // award points only after verification and at processed/recycled stage, once
    if (r->isVerified() && r->getStatus() == "Processed") {
        if (!rm.isDuplicateReward(wid)) {
            double pts = rm.awardPoints(wid, categoryToString(r->getCategory()), collected);
            if (pts > 0.0) {
                Student* s = findStudent(students, r->getStudentId());
                if (s) s->addRewardPoints(pts);
                cout << "Reward points awarded: " << fixed << setprecision(2) << pts << "\n";
            } else {
                cout << "No reward points applicable for this category/quantity.\n";
            }
        } else {
            cout << "Reward already awarded for this report.\n";
        }
    } else {
        cout << "Note: Points awarded only after verification and when report reaches Processed.\n";
    }
    fm.saveRecycling(recycling);
    fm.saveReports(reports);
    fm.saveStudents(students);
    cout << "Recycling record saved. Remaining not recycled: " << fixed << setprecision(2) << (collected - recycled) << "\n";
}

static void manageEWaste(vector<EWasteRecord>& ewaste, vector<WasteReport>& reports, FileManager& fm) {
    cout << "\n1. Add/Update E-waste record\n2. View E-waste records\nChoice: ";
    string c; if (!getline(cin,c)) return; c=trim(c);
    if (c=="2") {
        cout << "\n--- E-waste Records ---\n";
        if (ewaste.empty()) { cout << "None.\n"; return; }
        for (auto& e : ewaste) cout << e.getSummary() << "\n";
        return;
    }
    string wid; if (!readLineSafe("Enter Waste ID (must exist as e-waste report ideally): ", wid)) return;
    WasteReport* rpt = findReport(reports, wid);
    if (rpt && rpt->getCategory() != WasteCategory::ElectronicEwaste) {
        cout << "Linked report is not Electronic / E-waste category.\n";
        return;
    }
    string type, status, handover; double qty;
    if (!readLineSafe("Enter item type (e.g., charger, cable): ", type) || type.empty() || containsPipe(type)) { cout << "Invalid item type.\n"; return; }
    if (!readDouble("Enter quantity: ", qty) || qty <= 0.0) { cout << "Invalid quantity.\n"; return; }
    if (!readLineSafe("Enter collection status: ", status) || status.empty() || containsPipe(status)) { cout << "Invalid status.\n"; return; }
    if (!readLineSafe("Enter handover details (or 'Pending'): ", handover) || handover.empty() || containsPipe(handover)) { cout << "Invalid handover details.\n"; return; }
    // update existing or add new
    bool found=false;
    for (auto& e : ewaste) {
        if (e.getWasteId()==wid) {
            e.setItemType(type); e.setQuantity(qty); e.setCollectionStatus(status); e.setHandoverDetails(handover);
            found=true; break;
        }
    }
    if (!found) ewaste.emplace_back(wid, qty, "unit(s)", type, status, handover);
    fm.saveEWaste(ewaste);
    cout << "E-waste record saved.\n";
}

static void viewRewardInfo(const vector<Student>& students, RewardManager& rm) {
    cout << "\n--- Reward Rates (points/kg) ---\n";
    for (auto& kv : rm.getRates()) cout << kv.first << ": " << kv.second << "\n";
    cout << "\n--- Student Reward Points ---\n";
    for (auto& s : students) cout << s.getUserId() << " | " << s.getName() << " | " << fixed << setprecision(2) << s.getRewardPoints() << "\n";
}

static void viewStats(ReportGenerator& rg, vector<WasteReport>& reports, vector<RecyclingRecord>& recycling) {
    cout << "\n--- Waste Statistics ---\n";
    double total = rg.totalWaste(reports);
    cout << fixed << setprecision(2);
    cout << "Total waste: " << total << " kg\n";
    auto cats = rg.categoryTotals(reports);
    for (auto& kv : cats) {
        double pct = total>0 ? (kv.second/total*100.0) : 0.0;
        cout << kv.first << ": " << kv.second << " kg (" << pct << "%)\n";
    }
    cout << "Recycling rate: " << rg.recyclingRate(reports, recycling) << "%\n";
    cout << "Collection efficiency: " << rg.collectionEfficiency(reports) << "%\n";
}

static void generateSummary(ReportGenerator& rg, vector<WasteReport>& reports, vector<RecyclingRecord>& recycling,
                            CollectionManager& cm, vector<EWasteRecord>& ewaste, vector<Student>& students) {
    cout << "\n1. Overall summary\n2. Category summary\nChoice: ";
    string c; if (!getline(cin,c)) return; c=trim(c);
    if (c=="2") {
        cout << "Enter category name exactly (Biodegradable/Recyclable/Non-recyclable/Hazardous/Electronic / E-waste): ";
        string cat; if (!getline(cin,cat)) return; cat=trim(cat);
        rg.summary(cat, reports, recycling);
    } else {
        rg.summary(reports, recycling, cm.getRequests(), ewaste, students);
        double prev,curr;
        cout << "\nOptional waste reduction: enter previous period total (or -1 to skip): ";
        if (readDouble("", prev) && prev >= 0.0) {
            if (readDouble("Enter current period total: ", curr) && curr >= 0.0) {
                cout << "Waste reduction percentage: " << fixed << setprecision(2) << rg.wasteReductionPercentage(prev, curr) << "%\n";
            }
        }
    }
}

static void adminMenu(Admin&, vector<Student>& students, vector<WasteReport>& reports, CollectionManager& cm,
                      vector<RecyclingRecord>& recycling, vector<EWasteRecord>& ewaste, RewardManager& rm,
                      ReportGenerator& rg, FileManager& fm) {
    while (true) {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. View all students\n2. View all waste reports\n3. Search waste report by Waste ID\n"
             << "4. View pending collections\n5. Create collection request\n6. Update report/collection status\n"
             << "7. Verify a waste report\n8. Record recycling details\n9. Manage e-waste records and handover\n"
             << "10. View reward information\n11. View waste statistics\n12. Generate summary report\n13. Save and exit\n";
        int choice;
        if (!readInt("Enter choice: ", choice)) { cout << "Invalid input.\n"; continue; }
        switch (choice) {
            case 1: viewAllStudents(students); break;
            case 2: viewAllReports(reports); break;
            case 3: searchReportById(reports); break;
            case 4: viewPendingCollections(cm); break;
            case 5: createCollectionReq(reports, cm, fm); break;
            case 6: updateStatusFlow(reports, cm, fm); break;
            case 7: verifyReport(reports, fm); break;
            case 8: recordRecycling(reports, recycling, students, rm, fm); break;
            case 9: manageEWaste(ewaste, reports, fm); break;
            case 10: viewRewardInfo(students, rm); break;
            case 11: viewStats(rg, reports, recycling); break;
            case 12: generateSummary(rg, reports, recycling, cm, ewaste, students); break;
            case 13: saveAll(fm, students, reports, cm, recycling, ewaste, rm); cout << "Saved. Exiting admin session.\n"; return;
            default: cout << "Invalid choice.\n";
        }
    }
}

static void registerStudent(vector<Student>& students, FileManager& fm) {
    string id,name,dept; int sem;
    if (!readLineSafe("Enter Student ID: ", id) || id.empty() || containsPipe(id)) { cout << "Invalid Student ID.\n"; return; }
    for (auto& s : students) if (s.getUserId()==id) { cout << "Duplicate student ID rejected.\n"; return; }
    if (!readLineSafe("Enter Name: ", name) || name.empty() || containsPipe(name)) { cout << "Invalid name.\n"; return; }
    if (!readLineSafe("Enter Department: ", dept) || dept.empty() || containsPipe(dept)) { cout << "Invalid department.\n"; return; }
    if (!readInt("Enter Semester (positive number): ", sem) || sem <= 0) { cout << "Invalid semester.\n"; return; }
    students.emplace_back(id, name, dept, sem, 0.0);
    fm.saveStudents(students);
    cout << "Registration successful for " << name << ".\n";
}

int main() {
    FileManager fm("data");
    fm.ensureFilesExist();

    vector<Student> students = fm.loadStudents();
    vector<WasteReport> reports = fm.loadReports();
    vector<CollectionRequest> colReqs = fm.loadCollections();
    vector<RecyclingRecord> recycling = fm.loadRecycling();
    vector<EWasteRecord> ewaste = fm.loadEWaste();
    RewardManager rewardManager;
    fm.loadRewards(rewardManager);
    CollectionManager cm;
    for (auto& r : colReqs) cm.addRequest(r);
    ReportGenerator reportGen;

    cout << "============================================\n";
    cout << " EcoTrack - Smart Campus Waste Management\n";
    cout << " (Academic prototype for MAIT, C++ OOP)\n";
    cout << " SDG 12 primary, SDG 11 secondary\n";
    cout << "============================================\n";
    cout << "Note: Console-based prototype using text files. ID-based login is basic, not secure production auth.\n";

    while (true) {
        cout << "\nMain Menu:\n1. Student Registration\n2. Student Login\n3. Admin Login\n4. Exit\n";
        int choice;
        if (!readInt("Enter choice: ", choice)) { cout << "Invalid input.\n"; continue; }
        if (choice == 1) {
            registerStudent(students, fm);
        } else if (choice == 2) {
            string sid; if (!readLineSafe("Enter Student ID: ", sid)) continue;
            Student* s = findStudent(students, sid);
            if (!s) { cout << "Student not found.\n"; continue; }
            cout << "Welcome, " << s->getName() << "!\n";
            // demonstrate polymorphism
            User* u = s;
            u->displayMenu(); // virtual dispatch shown once
            studentMenu(*s, students, reports, ewaste, fm);
            fm.saveStudents(students); fm.saveReports(reports);
        } else if (choice == 3) {
            string aid; if (!readLineSafe("Enter Admin ID (demo: admin): ", aid)) continue;
            if (aid != "admin") { cout << "Invalid admin ID.\n"; continue; }
            Admin admin("admin", "Administrator");
            User* u = &admin;
            u->displayMenu();
            adminMenu(admin, students, reports, cm, recycling, ewaste, rewardManager, reportGen, fm);
        } else if (choice == 4) {
            saveAll(fm, students, reports, cm, recycling, ewaste, rewardManager);
            cout << "All data saved. Goodbye.\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
