#ifndef COLLECTIONMANAGER_H
#define COLLECTIONMANAGER_H

#include "CollectionRequest.h"
#include <vector>
#include <string>

class CollectionManager {
private:
    std::vector<CollectionRequest> requests;
public:
    void addRequest(const CollectionRequest& req);
    std::vector<CollectionRequest>& getRequests();
    const std::vector<CollectionRequest>& getRequests() const;
    std::vector<CollectionRequest> getPending() const;
    CollectionRequest* findByWasteId(const std::string& wasteId);
    bool updateRequestStatus(const std::string& wasteId, const std::string& newStatus);
    std::string generateRequestId() const;
};

#endif
