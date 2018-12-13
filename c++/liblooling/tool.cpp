#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/Tooling.h"

#include "llvm/Support/CommandLine.h"


struct ErrorDeclInfo {
    std::string file;
    std::string name;
    unsigned int line       = -1;
    unsigned int column     = -1;
    unsigned int blockBegin = -1;
    unsigned int blockEnd   = -1;
    bool found              = false;
};


class FindErrorVarVisitor final : public clang::RecursiveASTVisitor<FindErrorVarVisitor> {
public:
    explicit FindErrorVarVisitor(clang::ASTContext* context, std::vector<ErrorDeclInfo>& errorDeclInfos)
        : m_context(context), m_errorDeclInfos{errorDeclInfos} {}

    auto VisitCompoundStmt(clang::CompoundStmt* comStmt) -> bool {
        m_currentBlock = comStmt;
        return true;
    }

    auto VisitCXXMemberCallExpr(clang::CXXMemberCallExpr* cxxMemCallExpr) -> bool {
        auto cxxRecord = cxxMemCallExpr->getRecordDecl();
        if (cxxRecord->getQualifiedNameAsString() == "myspace::MyError") {
            // get location
            auto fullLoc = m_context->getFullLoc(cxxMemCallExpr->getLocStart());
            auto line    = fullLoc.getSpellingLineNumber();

            // get object name
            auto obj = cxxMemCallExpr->getImplicitObjectArgument();
            std::string objName =
                clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(obj->getSourceRange()),
                                            m_context->getSourceManager(), m_context->getLangOpts(), 0);

            // get function name
            auto cxxMethod = cxxMemCallExpr->getMethodDecl();
            auto funcName  = cxxMethod->getNameInfo().getAsString();

            // find location of object's declaration
            // if it exists, erase it
            auto it = std::find_if(rbegin(m_errorDeclInfos), rend(m_errorDeclInfos), [&](ErrorDeclInfo const& info) {
                return objName == info.name && line >= info.line && (info.blockBegin <= line && line <= info.blockEnd);
            });
            if (it != rend(m_errorDeclInfos)) it->found = true;
        }
        return true;
    }

    auto VisitVarDecl(clang::VarDecl* varDecl) -> bool {
        auto quadType = varDecl->getType();
        if (quadType.getAsString() == "myspace::MyError") {
            auto fullLoc       = m_context->getFullLoc(varDecl->getLocation());
            auto blockBeginLoc = m_context->getFullLoc(m_currentBlock->getLocStart());
            auto blockEndLoc   = m_context->getFullLoc(m_currentBlock->getLocEnd());
            auto fileExtry     = m_context->getSourceManager().getFileEntryForID(fullLoc.getFileID());

            // save variable's declaration location
            ErrorDeclInfo info;
            info.file       = (fileExtry ? fileExtry->getName() : "nullptr");
            info.name       = varDecl->getName().str();
            info.line       = fullLoc.getSpellingLineNumber();
            info.column     = fullLoc.getSpellingColumnNumber();
            info.blockBegin = blockBeginLoc.getSpellingLineNumber();
            info.blockEnd   = blockEndLoc.getSpellingLineNumber();
            m_errorDeclInfos.emplace_back(std::move(info));
        }
        return true;
    }

private:
    clang::ASTContext* m_context;
    clang::CompoundStmt* m_currentBlock;
    std::vector<ErrorDeclInfo>& m_errorDeclInfos;
};


class FindErrorVarCheckConsumer final : public clang::ASTConsumer {
public:
    explicit FindErrorVarCheckConsumer(clang::ASTContext* context) : m_visitor{context, m_errorDeclInfos} {}

    virtual void HandleTranslationUnit(clang::ASTContext& context) {
        m_visitor.TraverseDecl(context.getTranslationUnitDecl());
        llvm::outs().changeColor(llvm::raw_ostream::Colors::RED, true);
        for (auto const& info : m_errorDeclInfos) {
            if (!info.found) {
                llvm::outs() << "MyError instance (" << info.name << ") is not checked, at "
                             << info.file << ":" << info.line << ":" << info.column << "\n";
            }
        }
        llvm::outs().resetColor();
    }

private:
    std::vector<ErrorDeclInfo> m_errorDeclInfos;
    FindErrorVarVisitor m_visitor;
};


class FindNamedClassAction final : public clang::ASTFrontendAction {
public:
    virtual auto CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile)
        -> std::unique_ptr<clang::ASTConsumer> override {
        return std::make_unique<FindErrorVarCheckConsumer>(&compiler.getASTContext());
    }
};


static llvm::cl::OptionCategory MyToolCategory("tool options");
static llvm::cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp MoreHelp(
    "\nAdd compiling command after -- symbol, for example\n"
    "\ttool input.cpp -- clang++ -std=c++14\n");

int main(int argc, char const* argv[]) {
    using namespace std::literals;

    clang::tooling::CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    clang::tooling::ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    // run tool on code with specific compiling command
    return Tool.run(clang::tooling::newFrontendActionFactory<FindNamedClassAction>().get());
}


/*
void usage(std::ostream& os) {
    os << "Usage:\n"
       << "\tfile <file>\n"
       << "\tsrc <src>\n";
}


int main(int argc, char const* argv[]) {
    using namespace std::literals;

    if (argc < 3) {
        usage(std::cout);
        return 0;
    }

    try {
        std::string fileName = "input.cc";
        std::string src;
        if (argv[1] == "file"s) {
            fileName = argv[2];
            std::stringstream ss;
            std::ifstream ifs{fileName};
            ss << ifs.rdbuf();
            src = ss.str();
        } else if (argv[1] == "src"s) {
            src = argv[2];
        } else {
            usage(std::cout);
            return 0;
        }

        // run tool on code with -fsyntax-only flag
        clang::tooling::runToolOnCode(new FindNamedClassAction, src, fileName);
    } catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }
    return 0;
}
*/
