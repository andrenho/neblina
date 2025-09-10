#ifndef SERVICE_HH
#define SERVICE_HH

class Service {
public:
    virtual ~Service() = default;
    [[noreturn]] virtual void run() = 0;
};

#endif //SERVICE_HH
