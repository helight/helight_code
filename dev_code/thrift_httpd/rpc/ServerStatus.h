/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ServerStatus_H
#define ServerStatus_H

#include <thrift/TDispatchProcessor.h>
#include "server_status_types.h"

namespace thrift_httpd {

class ServerStatusIf {
 public:
  virtual ~ServerStatusIf() {}
  virtual void ping() = 0;
  virtual int32_t add(const int32_t num1, const int32_t num2) = 0;
  virtual int32_t calculate(const int32_t logid, const Work& w) = 0;
  virtual void getStruct(SharedStruct& _return, const int32_t key) = 0;
};

class ServerStatusIfFactory {
 public:
  typedef ServerStatusIf Handler;

  virtual ~ServerStatusIfFactory() {}

  virtual ServerStatusIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(ServerStatusIf* /* handler */) = 0;
};

class ServerStatusIfSingletonFactory : virtual public ServerStatusIfFactory {
 public:
  ServerStatusIfSingletonFactory(const boost::shared_ptr<ServerStatusIf>& iface) : iface_(iface) {}
  virtual ~ServerStatusIfSingletonFactory() {}

  virtual ServerStatusIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(ServerStatusIf* /* handler */) {}

 protected:
  boost::shared_ptr<ServerStatusIf> iface_;
};

class ServerStatusNull : virtual public ServerStatusIf {
 public:
  virtual ~ServerStatusNull() {}
  void ping() {
    return;
  }
  int32_t add(const int32_t /* num1 */, const int32_t /* num2 */) {
    int32_t _return = 0;
    return _return;
  }
  int32_t calculate(const int32_t /* logid */, const Work& /* w */) {
    int32_t _return = 0;
    return _return;
  }
  void getStruct(SharedStruct& /* _return */, const int32_t /* key */) {
    return;
  }
};


class ServerStatus_ping_args {
 public:

  ServerStatus_ping_args() {
  }

  virtual ~ServerStatus_ping_args() throw() {}


  bool operator == (const ServerStatus_ping_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const ServerStatus_ping_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ServerStatus_ping_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ServerStatus_ping_pargs {
 public:


  virtual ~ServerStatus_ping_pargs() throw() {}


  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ServerStatus_ping_result {
 public:

  ServerStatus_ping_result() {
  }

  virtual ~ServerStatus_ping_result() throw() {}


  bool operator == (const ServerStatus_ping_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const ServerStatus_ping_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ServerStatus_ping_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ServerStatus_ping_presult {
 public:


  virtual ~ServerStatus_ping_presult() throw() {}


  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ServerStatus_add_args__isset {
  _ServerStatus_add_args__isset() : num1(false), num2(false) {}
  bool num1;
  bool num2;
} _ServerStatus_add_args__isset;

class ServerStatus_add_args {
 public:

  ServerStatus_add_args() : num1(0), num2(0) {
  }

  virtual ~ServerStatus_add_args() throw() {}

  int32_t num1;
  int32_t num2;

  _ServerStatus_add_args__isset __isset;

  void __set_num1(const int32_t val) {
    num1 = val;
  }

  void __set_num2(const int32_t val) {
    num2 = val;
  }

  bool operator == (const ServerStatus_add_args & rhs) const
  {
    if (!(num1 == rhs.num1))
      return false;
    if (!(num2 == rhs.num2))
      return false;
    return true;
  }
  bool operator != (const ServerStatus_add_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ServerStatus_add_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ServerStatus_add_pargs {
 public:


  virtual ~ServerStatus_add_pargs() throw() {}

  const int32_t* num1;
  const int32_t* num2;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ServerStatus_add_result__isset {
  _ServerStatus_add_result__isset() : success(false) {}
  bool success;
} _ServerStatus_add_result__isset;

class ServerStatus_add_result {
 public:

  ServerStatus_add_result() : success(0) {
  }

  virtual ~ServerStatus_add_result() throw() {}

  int32_t success;

  _ServerStatus_add_result__isset __isset;

  void __set_success(const int32_t val) {
    success = val;
  }

  bool operator == (const ServerStatus_add_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const ServerStatus_add_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ServerStatus_add_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ServerStatus_add_presult__isset {
  _ServerStatus_add_presult__isset() : success(false) {}
  bool success;
} _ServerStatus_add_presult__isset;

class ServerStatus_add_presult {
 public:


  virtual ~ServerStatus_add_presult() throw() {}

  int32_t* success;

  _ServerStatus_add_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ServerStatus_calculate_args__isset {
  _ServerStatus_calculate_args__isset() : logid(false), w(false) {}
  bool logid;
  bool w;
} _ServerStatus_calculate_args__isset;

class ServerStatus_calculate_args {
 public:

  ServerStatus_calculate_args() : logid(0) {
  }

  virtual ~ServerStatus_calculate_args() throw() {}

  int32_t logid;
  Work w;

  _ServerStatus_calculate_args__isset __isset;

  void __set_logid(const int32_t val) {
    logid = val;
  }

  void __set_w(const Work& val) {
    w = val;
  }

  bool operator == (const ServerStatus_calculate_args & rhs) const
  {
    if (!(logid == rhs.logid))
      return false;
    if (!(w == rhs.w))
      return false;
    return true;
  }
  bool operator != (const ServerStatus_calculate_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ServerStatus_calculate_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ServerStatus_calculate_pargs {
 public:


  virtual ~ServerStatus_calculate_pargs() throw() {}

  const int32_t* logid;
  const Work* w;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ServerStatus_calculate_result__isset {
  _ServerStatus_calculate_result__isset() : success(false), ouch(false) {}
  bool success;
  bool ouch;
} _ServerStatus_calculate_result__isset;

class ServerStatus_calculate_result {
 public:

  ServerStatus_calculate_result() : success(0) {
  }

  virtual ~ServerStatus_calculate_result() throw() {}

  int32_t success;
  InvalidOperation ouch;

  _ServerStatus_calculate_result__isset __isset;

  void __set_success(const int32_t val) {
    success = val;
  }

  void __set_ouch(const InvalidOperation& val) {
    ouch = val;
  }

  bool operator == (const ServerStatus_calculate_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(ouch == rhs.ouch))
      return false;
    return true;
  }
  bool operator != (const ServerStatus_calculate_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ServerStatus_calculate_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ServerStatus_calculate_presult__isset {
  _ServerStatus_calculate_presult__isset() : success(false), ouch(false) {}
  bool success;
  bool ouch;
} _ServerStatus_calculate_presult__isset;

class ServerStatus_calculate_presult {
 public:


  virtual ~ServerStatus_calculate_presult() throw() {}

  int32_t* success;
  InvalidOperation ouch;

  _ServerStatus_calculate_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ServerStatus_getStruct_args__isset {
  _ServerStatus_getStruct_args__isset() : key(false) {}
  bool key;
} _ServerStatus_getStruct_args__isset;

class ServerStatus_getStruct_args {
 public:

  ServerStatus_getStruct_args() : key(0) {
  }

  virtual ~ServerStatus_getStruct_args() throw() {}

  int32_t key;

  _ServerStatus_getStruct_args__isset __isset;

  void __set_key(const int32_t val) {
    key = val;
  }

  bool operator == (const ServerStatus_getStruct_args & rhs) const
  {
    if (!(key == rhs.key))
      return false;
    return true;
  }
  bool operator != (const ServerStatus_getStruct_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ServerStatus_getStruct_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ServerStatus_getStruct_pargs {
 public:


  virtual ~ServerStatus_getStruct_pargs() throw() {}

  const int32_t* key;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ServerStatus_getStruct_result__isset {
  _ServerStatus_getStruct_result__isset() : success(false) {}
  bool success;
} _ServerStatus_getStruct_result__isset;

class ServerStatus_getStruct_result {
 public:

  ServerStatus_getStruct_result() {
  }

  virtual ~ServerStatus_getStruct_result() throw() {}

  SharedStruct success;

  _ServerStatus_getStruct_result__isset __isset;

  void __set_success(const SharedStruct& val) {
    success = val;
  }

  bool operator == (const ServerStatus_getStruct_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const ServerStatus_getStruct_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ServerStatus_getStruct_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ServerStatus_getStruct_presult__isset {
  _ServerStatus_getStruct_presult__isset() : success(false) {}
  bool success;
} _ServerStatus_getStruct_presult__isset;

class ServerStatus_getStruct_presult {
 public:


  virtual ~ServerStatus_getStruct_presult() throw() {}

  SharedStruct* success;

  _ServerStatus_getStruct_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class ServerStatusClient : virtual public ServerStatusIf {
 public:
  ServerStatusClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  ServerStatusClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void ping();
  void send_ping();
  void recv_ping();
  int32_t add(const int32_t num1, const int32_t num2);
  void send_add(const int32_t num1, const int32_t num2);
  int32_t recv_add();
  int32_t calculate(const int32_t logid, const Work& w);
  void send_calculate(const int32_t logid, const Work& w);
  int32_t recv_calculate();
  void getStruct(SharedStruct& _return, const int32_t key);
  void send_getStruct(const int32_t key);
  void recv_getStruct(SharedStruct& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class ServerStatusProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<ServerStatusIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (ServerStatusProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_add(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_calculate(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_getStruct(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  ServerStatusProcessor(boost::shared_ptr<ServerStatusIf> iface) :
    iface_(iface) {
    processMap_["ping"] = &ServerStatusProcessor::process_ping;
    processMap_["add"] = &ServerStatusProcessor::process_add;
    processMap_["calculate"] = &ServerStatusProcessor::process_calculate;
    processMap_["getStruct"] = &ServerStatusProcessor::process_getStruct;
  }

  virtual ~ServerStatusProcessor() {}
};

class ServerStatusProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  ServerStatusProcessorFactory(const ::boost::shared_ptr< ServerStatusIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< ServerStatusIfFactory > handlerFactory_;
};

class ServerStatusMultiface : virtual public ServerStatusIf {
 public:
  ServerStatusMultiface(std::vector<boost::shared_ptr<ServerStatusIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~ServerStatusMultiface() {}
 protected:
  std::vector<boost::shared_ptr<ServerStatusIf> > ifaces_;
  ServerStatusMultiface() {}
  void add(boost::shared_ptr<ServerStatusIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void ping() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ping();
    }
    ifaces_[i]->ping();
  }

  int32_t add(const int32_t num1, const int32_t num2) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->add(num1, num2);
    }
    return ifaces_[i]->add(num1, num2);
  }

  int32_t calculate(const int32_t logid, const Work& w) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->calculate(logid, w);
    }
    return ifaces_[i]->calculate(logid, w);
  }

  void getStruct(SharedStruct& _return, const int32_t key) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->getStruct(_return, key);
    }
    ifaces_[i]->getStruct(_return, key);
    return;
  }

};

} // namespace

#endif
