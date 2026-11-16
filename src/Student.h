#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <string>

class Student : public User {
private:
    std::string department;
    int semester;
    double rewardPoints;

public:
    Student();
    Student(const std::string& id, const std::string& userName,
            const std::string& dept, int sem, double points = 0.0);

    std::string getDepartment() const;
    int getSemester() const;
    double getRewardPoints() const;

    bool setDepartment(const std::string& dept);
    bool setSemester(int sem);
    void setRewardPoints(double points);
    void addRewardPoints(double points);

    void displayMenu() override;
    std::string getRole() const override;
};

#endif
