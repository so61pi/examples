#include "MyError.h"


void test(myspace::MyError& error) {
    error = myspace::MyError{};
}


int main() {
    myspace::MyError e;
    test(e);
}