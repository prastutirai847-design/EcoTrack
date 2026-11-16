#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    std::string userId;
    std::string name;

public:
    User();
    User(const std::string& id, const std::string& userName);
    virtual ~User();

    std::string getUserId() const;
    std::string getName() const;
    void setUserId(const std::string& id);
    void setName(const std::string& userName);

    virtual void displayMenu() = 0;
    virtual std::string getRole() const = 0;
};

#endif
