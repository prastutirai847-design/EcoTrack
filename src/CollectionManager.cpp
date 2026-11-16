#include "CollectionManager.h"
#include <sstream>

void CollectionManager::addRequest(const CollectionRequest& req) {
    requests.push_back(req);
}

std::vector<CollectionRequest>& CollectionManager::getRequests() { return requests; }
const std::vector<CollectionRequest>& CollectionManager::getRequests() const { return requests; }

std::vector<CollectionRequest> CollectionManager::getPending() const {
    std::vector<CollectionRequest> pending;
    for (const auto& r : requests) {
        if (r.getStatus() != "Closed" && r.getStatus() != "Processed") {
            pending.push_back(r);
        }
    }
    return pending;
}

CollectionRequest* CollectionManager::findByWasteId(const std::string& wasteId) {
    for (auto& r : requests) {
        if (r.getWasteId() == wasteId) return &r;
    }
    return nullptr;
}

bool CollectionManager::updateRequestStatus(const std::string& wasteId, const std::string& newStatus) {
    CollectionRequest* r = findByWasteId(wasteId);
    if (!r) return false;
    return r->setStatus(newStatus);
}

std::string CollectionManager::generateRequestId() const {
    std::ostringstream oss;
    oss << "REQ" << (requests.size() + 1);
    return oss.str();
}
