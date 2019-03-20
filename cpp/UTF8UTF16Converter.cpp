#include <codecvt>
#include <iostream>
#include <locale>
#include <string>


int main() {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;

    // display an UTF-8 string
    std::string utf8_string = "Hello world!";
    std::cout << utf8_string << "\n";

    // convert to UTF-16 string
    std::wstring utf16_string = converter.from_bytes(utf8_string);
    std::wcout << utf16_string << L"\n";

    // convert back to UTF-8 string
    utf8_string = converter.to_bytes(utf16_string);
    std::cout << utf8_string << "\n";
}
