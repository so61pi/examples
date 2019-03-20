#include <iostream>
#include <memory>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include "rpc.grpc.pb.h"


// g++ -Wall -Wextra -pedantic -o client -std=c++11 client.cpp rpc.pb.cc rpc.grpc.pb.cc -lprotobuf -lgrpc++


int main() {
    auto channel = grpc::CreateChannel("localhost:12345", grpc::InsecureChannelCredentials());
    auto stub = MyServiceProvider::NewStub(channel);

    grpc::ClientContext context;
    MyRequest           request;
    MyResponse          response;
    request.set_question("???");
    grpc::Status status = stub->AskMe(&context, request, &response);
    if (status.ok()) {
        std::cout << "Answer from server = " << response.answer() << std::endl;
    } else {
        std::cerr << "AskMe rpc failed." << std::endl;
    }
}
