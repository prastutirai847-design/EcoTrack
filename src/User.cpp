#include "User.h"

User::User() : userId(""), name("") {}

User::User(const std::string& id, const std::string& userName)
    : userId(id), name(userName) {}

User::~User() {}

std::string User::getUserId() const { return userId; }
std::string User::getName() const { return name; }

void User::setUserId(const std::string& id) { userId = id; }
void User::setName(const std::string& userName) { name = userName; }
