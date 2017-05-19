#include <fstream>
#include <iostream>
#include <string>
#include "message.pb.h"

// protoc --cpp_out=. message.proto
// g++ -std=c++11 message.pb.cc main.cpp -pthread -L/usr/local/lib -lprotobuf -lpthread


int main(int argc, char* argv[]) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <FILE>" << std::endl;
    return -1;
  }

  std::string const file(argv[1]);

  {
    Query query;
    query.set_name("so61pi");
    query.set_age(-1);

    std::fstream output(file, std::ios::out | std::ios::trunc | std::ios::binary);
    if (query.SerializeToOstream(&output) == false) {
      std::cerr << "Failed to write Query." << std::endl;
      return -1;
    }
  }

  {
    std::fstream input(file, std::ios::in | std::ios::binary);

    Query query;
    if (query.ParseFromIstream(&input) == false) {
      std::cerr << "Failed to read Query." << std::endl;
      return -1;
    }
    std::cout << query.name() << std::endl
              << query.age() << std::endl;
  }

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
