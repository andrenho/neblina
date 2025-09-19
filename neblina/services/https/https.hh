#ifndef HTTPS_HH
#define HTTPS_HH

#include "service/communication/communication_service.hh"

class Https : public CommunicationService {
public:
    SERVICE_NAME = "https";

    Https();

    void init() override;
};

#endif //HTTPS_HH
