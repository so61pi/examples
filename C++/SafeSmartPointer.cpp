#include <memory>
#include <windows.h>


int main() {
    auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
    if (hProcess) {
        // cannot use
        //     std::shared_ptr<HANDLE> pHandle{ new HANDLE{ hProcess }, Deleter };
        // because it's not exception-safe

        // allocate HANDLE by using std::make_unique
        auto helper = std::make_unique<HANDLE>(hProcess);

        // then transfer the newly allocated object to std::shared_ptr
        // if an exception is thrown during constructing std::shared_ptr,
        // the allocated object will be freed by std::unique_ptr above
        std::shared_ptr<HANDLE> pHandle{ helper.get(), [](HANDLE *handle) { CloseHandle(*handle); delete handle; } };

        // until now, the allocated object is controlled by both std::unique_ptr and std::shared_ptr
        // so std::unique_ptr has to release its control
        helper.release(); // std::unique_ptr<T>::release is noexcept, so it's safe
    }
}
