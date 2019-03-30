#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <boost/scope_exit.hpp>

#include <Windows.h>
#include <Wincrypt.h>
#include <Shlobj.h>
#include <Shlwapi.h>

#include "sqlite3.h"
#include "LoginData.h"

#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Shlwapi.lib")


/*
Documents:

https://chromium.googlesource.com/chromium/src.git/+/master/components/password_manager/core/browser/login_database.cc
LoginDatabase::AddLogin

https://chromium.googlesource.com/chromium/src.git/+/master/components/password_manager/core/browser/login_database_win.cc
LoginDatabase::EncryptedString

https://chromium.googlesource.com/chromium/src.git/+/master/components/os_crypt/os_crypt_win.cc
OSCrypt::EncryptString
*/


std::wstring GetLoginDatabaseFilePath() {
    PWSTR localAppData = nullptr;
    BOOST_SCOPE_EXIT_ALL(&) {
        if (localAppData)
            CoTaskMemFree(static_cast<LPVOID>(localAppData));
    };
    if (SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localAppData) != S_OK)
        throw std::runtime_error("Cannot get login database file path.");

    std::wstring dbFile = localAppData;
    dbFile += L"\\Google\\Chrome\\User Data\\Default\\";
    dbFile += L"Login Data";

    if (PathFileExists(dbFile.c_str()) == FALSE)
        throw std::runtime_error("Cannot get login database file path.");;

    return dbFile;
}


std::vector<LoginData> GetLoginData() {
    auto dbFile = GetLoginDatabaseFilePath();

    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    BOOST_SCOPE_EXIT_ALL(&) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    };

    if (sqlite3_open16(dbFile.c_str(), &db) != SQLITE_OK)
        throw std::runtime_error("Cannot open database.");
    
    const std::string query = "SELECT origin_url, username_value, password_value FROM logins;";
    if (sqlite3_prepare_v2(db, query.c_str(), query.size(), &stmt, NULL) != SQLITE_OK)
        throw std::runtime_error("Cannot prepare query.");

    std::vector<LoginData> loginData;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        LoginData log;

        log.Url = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        log.UserName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

        DATA_BLOB dataIn{};
        DATA_BLOB dataOut{};
        dataIn.cbData = sqlite3_column_bytes(stmt, 2);
        dataIn.pbData = const_cast<BYTE *>(reinterpret_cast<const BYTE *>(sqlite3_column_blob(stmt, 2)));
        if (CryptUnprotectData(&dataIn, NULL, NULL, NULL, NULL, 0, &dataOut) == FALSE)
            continue;
        log.Password = std::string(reinterpret_cast<const char *>(dataOut.pbData), dataOut.cbData);
        LocalFree(dataOut.pbData);

        loginData.push_back(std::move(log));
    }

    return loginData;
}


int main() {
    try {
        auto loginData = GetLoginData();
        for (auto& log : loginData) {
            std::cout
                << log.Url << "\n"
                << log.UserName << "\n"
                << log.Password << "\n\n";
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    }
}
