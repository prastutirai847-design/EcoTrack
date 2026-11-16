#ifndef EWASTERECORD_H
#define EWASTERECORD_H

#include "Waste.h"
#include <string>

class EWasteRecord : public Waste {
private:
    std::string itemType;
    std::string collectionStatus;
    std::string handoverDetails;

public:
    EWasteRecord();
    EWasteRecord(const std::string& id, double qty, const std::string& u,
                 const std::string& type, const std::string& status, const std::string& handover);

    std::string getItemType() const;
    std::string getCollectionStatus() const;
    std::string getHandoverDetails() const;

    bool setItemType(const std::string& type);
    bool setCollectionStatus(const std::string& status);
    bool setHandoverDetails(const std::string& details);

    std::string getSummary() const override;
};

#endif
