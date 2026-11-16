#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User {
public:
    Admin();
    Admin(const std::string& id, const std::string& userName);

    void displayMenu() override;
    std::string getRole() const override;
};

#endif
