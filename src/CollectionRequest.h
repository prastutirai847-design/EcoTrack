#ifndef COLLECTIONREQUEST_H
#define COLLECTIONREQUEST_H

#include <string>

class CollectionRequest {
private:
    std::string requestId;
    std::string wasteId;
    std::string assignedDate;
    std::string status;

public:
    CollectionRequest();
    CollectionRequest(const std::string& reqId, const std::string& wId,
                      const std::string& date, const std::string& st);

    std::string getRequestId() const;
    std::string getWasteId() const;
    std::string getAssignedDate() const;
    std::string getStatus() const;

    bool setStatus(const std::string& newStatus);
    void forceStatus(const std::string& st);
};

#endif
