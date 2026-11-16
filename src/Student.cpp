#include "Student.h"
#include <iostream>
#include <iomanip>

Student::Student() : User(), department(""), semester(1), rewardPoints(0.0) {}

Student::Student(const std::string& id, const std::string& userName,
                 const std::string& dept, int sem, double points)
    : User(id, userName), department(dept), semester(sem), rewardPoints(points) {}

std::string Student::getDepartment() const { return department; }
int Student::getSemester() const { return semester; }
double Student::getRewardPoints() const { return rewardPoints; }

bool Student::setDepartment(const std::string& dept) {
    if (dept.empty()) return false;
    department = dept;
    return true;
}

bool Student::setSemester(int sem) {
    if (sem <= 0) return false;
    semester = sem;
    return true;
}

void Student::setRewardPoints(double points) {
    if (points < 0) rewardPoints = 0;
    else rewardPoints = points;
}

void Student::addRewardPoints(double points) {
    if (points > 0) rewardPoints += points;
}

void Student::displayMenu() {
    std::cout << "\n--- Student Menu (" << name << ") ---\n";
    std::cout << "1. View Profile\n";
    std::cout << "2. Submit Waste Report\n";
    std::cout << "3. View My Reports\n";
    std::cout << "4. View Reward Points\n";
    std::cout << "5. Logout\n";
}

std::string Student::getRole() const { return "Student"; }
