#include <Windows.h>


int main() {
    SHELLEXECUTEINFO seinfo{};
    seinfo.cbSize = sizeof(seinfo);
    seinfo.lpVerb = L"runas";
    seinfo.lpFile = L"cmd.exe";
    seinfo.nShow = SW_NORMAL;
    ShellExecuteEx(&seinfo);
}
