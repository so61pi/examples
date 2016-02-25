#include <iostream>

#include <clang-c/Index.h>


enum CXChildVisitResult VarPrinterVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    if (clang_getCursorKind(cursor) == CXCursor_VarDecl) {
        auto cxMangled  = clang_Cursor_getMangling(cursor);
        auto strMangled = clang_getCString(cxMangled);

        auto cxDisplay  = clang_getCursorDisplayName(cursor);
        auto strDisplay = clang_getCString(cxDisplay);

        auto cxSpell  = clang_getTypeSpelling(clang_getCursorType(cursor));
        auto strSpell = clang_getCString(cxSpell);

        auto location = clang_getCursorLocation(cursor);
        CXFile cxFile;
        unsigned line   = -1;
        unsigned column = -1;
        clang_getSpellingLocation(location, &cxFile, &line, &column, nullptr);
        auto cxFileName = clang_getFileName(cxFile);
        auto strName    = clang_getCString(cxFileName);

        std::cout << "Variable: "     << (strMangled ? strMangled : "nullptr") << "\n"
                  << "    Display:  " << (strDisplay ? strDisplay : "nullptr") << "\n"
                  << "    Spelling: " << (strSpell ? strSpell : "nullptr")     << "\n"
                  << "    At:       " << (strName ? strName : "nullptr")
                                      << ":" << line << ":" << column << "\n";

        clang_disposeString(cxMangled);
        clang_disposeString(cxSpell);
        clang_disposeString(cxDisplay);
        clang_disposeString(cxFileName);
    }
    return CXChildVisit_Recurse;
}


int main() {
    int excludeDeclsFromPCH = 1;
    int displayDiagnostics  = 1;
    auto Idx = clang_createIndex(excludeDeclsFromPCH, displayDiagnostics);

    char* args[] = {"-std=c++14"};
    auto TU = clang_createTranslationUnitFromSourceFile(Idx, "IndexTest.cpp",
                                                        sizeof(args) / sizeof(*args), args,
                                                        0, 0);
    clang_visitChildren(clang_getTranslationUnitCursor(TU),
                        VarPrinterVisitor, 0);
    clang_disposeTranslationUnit(TU);
}
