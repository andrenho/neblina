#ifndef SERVICE_HH
#define SERVICE_HH

#include <string>

class Service {
public:
    virtual ~Service() = default;

    virtual void init() {}
    [[noreturn]] virtual void run() = 0;

protected:
};

#endif //SERVICE_HH
