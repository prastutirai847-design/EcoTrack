#include "Admin.h"
#include <iostream>

Admin::Admin() : User() {}
Admin::Admin(const std::string& id, const std::string& userName) : User(id, userName) {}

void Admin::displayMenu() {
    std::cout << "\n--- Admin Menu (" << name << ") ---\n";
    std::cout << "1. View all students\n";
    std::cout << "2. View all waste reports\n";
    std::cout << "3. Search waste report by Waste ID\n";
    std::cout << "4. View pending collections\n";
    std::cout << "5. Create collection request\n";
    std::cout << "6. Update report/collection status\n";
    std::cout << "7. Verify a waste report\n";
    std::cout << "8. Record recycling details\n";
    std::cout << "9. Manage e-waste records and handover\n";
    std::cout << "10. View reward information\n";
    std::cout << "11. View waste statistics\n";
    std::cout << "12. Generate summary report\n";
    std::cout << "13. Save and exit\n";
}

std::string Admin::getRole() const { return "Admin"; }
