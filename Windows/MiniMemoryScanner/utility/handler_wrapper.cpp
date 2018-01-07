#include "handler_wrapper.h"


namespace {
    handle_deleter deleter;
}


std::unique_ptr<HANDLE, handle_deleter> make_unique_handle(HANDLE handle) {
    auto helper = std::make_unique<HANDLE>(handle);
    std::unique_ptr<HANDLE, handle_deleter> wrapper{ helper.get(), deleter };
    helper.release();
    return wrapper;
}


std::shared_ptr<HANDLE> make_shared_handle(HANDLE handle) {
    auto helper = std::make_unique<HANDLE>(handle);
    std::shared_ptr<HANDLE> wrapper{ helper.get(), deleter };
    helper.release();
    return wrapper;
}
