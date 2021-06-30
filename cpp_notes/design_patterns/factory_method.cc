#include <memory>
#include <string>
#include <unordered_map>
using namespace std;

class Session {
  public:
    Session();
    virtual ~Session();
    virtual void Run() = 0;
};

class DirectSession : public Session {
  public:
    DirectSession();
    virtual ~DirectSession();
    virtual void Run() override;
};

class GrpcSession : public Session {
  public:
    GrpcSession();
    virtual ~GrpcSession();
    virtual void Run() override;
};

class SessionFactory {
  public:
    virtual Session* NewSession() = 0;
};

class DirectSessionFactory : public SessionFactory {
  public:
    DirectSession* NewSession() override {
      return new DirectSession();
    }
};

class GrpcSessionFactory : public SessionFactory {
  public:
    GrpcSession* NewSession() override {
      return new GrpcSession();
    }
};