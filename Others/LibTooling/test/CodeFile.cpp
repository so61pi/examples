#include "MyError.h"


void calc(myspace::MyError& error) {
    error = myspace::MyError{};
}


int main() {
    myspace::MyError error;
    calc(error);
    {
        myspace::MyError error;
        if (error) {
            
        }
    }
}