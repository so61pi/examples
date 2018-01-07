#include <exception>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <boost/format.hpp>

#include <Windows.h>

#include "parser/option_parser.h"
#include "process/memory/accessor.h"
#include "process/memory/finder.h"
#include "process/memory/utility.h"
#include "process/process.h"
#include "utility/handler_wrapper.h"


//
//
//
void list_processes(std::wostream& os) {
    auto pids = process::get_pids();

    // then, collect information
    std::vector<std::tuple<DWORD, std::wstring>> info;
    for (auto pid : pids) {
        auto process_handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
        if (!process_handle)
            continue;

        auto sh = make_unique_handle(process_handle);

        // get process' full file name
        wchar_t fileName[MAX_PATH]{};
        DWORD size = std::extent<decltype(fileName)>::value;
        QueryFullProcessImageName(*sh, 0, fileName, &size);

        info.emplace_back(pid, fileName);
    }

    // finally, print them out
    //
    //        ###         PID  Name
    //          1       12345  abcdef
    //
    boost::wformat format(L"%|10s|  %|10s|  %|-s|\n");
    os << format % L"###" % L"PID" % L"Name";
    for (decltype(info.size()) i = 0; i < info.size(); ++i) {
        auto const& e = info[i];
        os << format % i % std::get<0>(e) % std::get<1>(e);
    }
}


//
//
//
void find_data(std::ostream& os, DWORD pid, std::string const& pattern) {
    auto process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!process_handle)
        throw make_system_except(GetLastError(), "cannot open process");

    auto sh = make_shared_handle(process_handle);

    // find
    process::memory::finder finder{ sh };
    auto result = finder.find(
        pattern,
        [](MEMORY_BASIC_INFORMATION const& mbi) {
            return mbi.State == MEM_COMMIT && !(mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD));
        }
    );

    process::memory::accessor accessor{ sh };
    boost::format format("%|10s|  |  %|#016x|  |  %|-s|\n");
    boost::format hexFormat("%|02x|  %|02x|  %|02x| ...");

    // print out the results
    os << "Number of results : " << result.size() << "\n";
    os << format % "###" % "Address" % "Value";
    for (decltype(result.size()) i = 0; i < result.size(); ++i) {
        auto address = result[i];
        auto info = process::memory::query_info(*sh, reinterpret_cast<std::uintptr_t>(address));
        if (info.State == MEM_COMMIT && !(info.Protect & (PAGE_NOACCESS | PAGE_GUARD))) {
            if (info.Protect & (PAGE_READWRITE | PAGE_READONLY)) {
                hexFormat
                    % static_cast<int>(accessor.read(address))
                    % static_cast<int>(accessor.read(address + 1))
                    % static_cast<int>(accessor.read(address + 2));
                os << format % i % reinterpret_cast<std::uintptr_t>(address) % hexFormat;
            }
        }
    }
}


//
//
//
void read_data(std::ostream& os, DWORD pid, std::uintptr_t address, std::size_t length) {
    auto process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!process_handle)
        throw make_system_except(GetLastError(), "cannot open process");

    auto sh = make_shared_handle(process_handle);

    process::memory::accessor accessor{ sh };
    auto result = accessor.read(reinterpret_cast<std::uint8_t const *>(address), length);

    boost::format format("%|02x|  ");
    for (auto& e : result)
        os << format % static_cast<int>(e);
    os << "\n";
}


//
//
//
void write_data(DWORD pid, std::uintptr_t address, std::string const& pattern ) {
    auto process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!process_handle)
        throw make_system_except(GetLastError(), "cannot open process");

    auto sh = make_shared_handle(process_handle);

    process::memory::accessor accessor{ sh };
    accessor.write(reinterpret_cast<std::uint8_t *>(address), pattern);
}



int main(int argc, char *argv[]) {
    try {
        option_parser op;

        op.add_options()
            ("help", "print help message")
            ("list-processes", "list all running processes")
            ("find", "find data in remote process")
            ("read", "read data from remote process")
            ("write", "write data to remote process")
            ("pid", boost::program_options::value<DWORD>(), "specify process identifier")
            ("pattern", boost::program_options::value<std::string>(), "specify the pattern")
            ("address", boost::program_options::value<std::string>(), "address to read or write")
            ("length", boost::program_options::value<std::size_t>(), "length in bytes")
            ;

        op.parse(argc, argv);

        if (op.exist("help")) {
            op.print(std::cout);
            return 0;
        }
        else if (op.exist("list-processes")) {
            list_processes(std::wcout);
            return 0;
        }
        else if (op.exist("find")) {
            auto pid = op.get<DWORD>("pid");
            auto pattern = op.get<std::string>("pattern");
            if (pid && pattern) {
                find_data(std::cout, *pid, *pattern);
                return 0;
            }
            else {
                std::cout << "option --find needs --pid and --pattern\n";
                return -1;
            }
        }
        else if (op.exist("read")) {
            auto pid = op.get<DWORD>("pid");
            auto address = op.get<std::string>("address");
            auto length = op.get<std::size_t>("length");
            if (pid && address && length) {
                read_data(std::cout, *pid, std::stoull(*address, nullptr, 16), *length);
                return 0;
            }
            else {
                std::cout << "option --read needs --pid, --address and --length\n";
                return -1;
            }
        }
        else if (op.exist("write")) {
            auto pid = op.get<DWORD>("pid");
            auto address = op.get<std::string>("address");
            auto pattern = op.get<std::string>("pattern");
            if (pid && address && pattern) {
                write_data(*pid, std::stoull(*address, nullptr, 16), *pattern);
                return 0;
            }
            else {
                std::cout << "option --write needs --pid, --address and --pattern\n";
                return -1;
            }
        }
        else {
            op.print(std::cout);
            return 0;
        }
    }
    catch (std::exception const& e) {
        std::cerr << "exception : " << e.what();
    }
}
