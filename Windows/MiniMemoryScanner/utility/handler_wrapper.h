#ifndef HANDLE_WRAPPER_H
#define HANDLE_WRAPPER_H


#include <memory>

#include <windows.h>


struct handle_deleter {
    void operator()(HANDLE *handle) const {
        CloseHandle(*handle);
        delete handle;
    }
};


std::unique_ptr<HANDLE, handle_deleter> make_unique_handle(HANDLE handle);

std::shared_ptr<HANDLE> make_shared_handle(HANDLE handle);


#endif  // HANDLE_WRAPPER_H
