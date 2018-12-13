#include <cassert>
#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/security/server_credentials.h>

#include "rpc.grpc.pb.h"


// protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` rpc.proto
// protoc --cpp_out=. rpc.proto
// g++ -Wall -Wextra -pedantic -o server -std=c++11 server.cpp rpc.pb.cc rpc.grpc.pb.cc -lprotobuf -lgrpc++


class MyServiceProviderImpl final : public MyServiceProvider::Service {
public:
    virtual ::grpc::Status AskMe(::grpc::ServerContext* context, const ::MyRequest* request, ::MyResponse* response) final override {
        assert(context);
        assert(request);
        assert(response);

        response->set_answer("No answer for question " + request->question());
        return ::grpc::Status::OK;
    }
};


void RunServer(std::string const& address) {
    MyServiceProviderImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << address << std::endl;
    server->Wait();
}


int main() {
    RunServer("0.0.0.0:12345");
    return 0;
}
