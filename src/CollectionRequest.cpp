#include "CollectionRequest.h"
#include "WasteReport.h"

CollectionRequest::CollectionRequest() : requestId(""), wasteId(""), assignedDate(""), status("Pending") {}

CollectionRequest::CollectionRequest(const std::string& reqId, const std::string& wId,
                                     const std::string& date, const std::string& st)
    : requestId(reqId), wasteId(wId), assignedDate(date), status(st) {}

std::string CollectionRequest::getRequestId() const { return requestId; }
std::string CollectionRequest::getWasteId() const { return wasteId; }
std::string CollectionRequest::getAssignedDate() const { return assignedDate; }
std::string CollectionRequest::getStatus() const { return status; }

bool CollectionRequest::setStatus(const std::string& newStatus) {
    if (WasteReport::isValidStatusTransition(status, newStatus)) {
        status = newStatus;
        return true;
    }
    return false;
}

void CollectionRequest::forceStatus(const std::string& st) { status = st; }
