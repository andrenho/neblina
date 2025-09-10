#ifndef SERVICE_HH
#define SERVICE_HH

class Service {
public:
    explicit Service(Arguments const& args) : args_(args) {}
    virtual ~Service() = default;

    [[noreturn]] virtual void run() = 0;

protected:
    Arguments const& args_;
};

#endif //SERVICE_HH
