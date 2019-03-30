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
#include "clang/Rewrite/Core/Rewriter.h"

#include "llvm/Support/CommandLine.h"


class VariableTrackerVisitor final : public clang::RecursiveASTVisitor<VariableTrackerVisitor> {
public:
    explicit VariableTrackerVisitor(clang::ASTContext* context, std::unique_ptr<clang::Rewriter> rewriter)
        : mContext(context), mRewriter(std::move(rewriter)) {}

    bool VisitFunctionDecl(clang::FunctionDecl *f) {
        // Only function definitions (with bodies), not declarations.
        if (f->hasBody()) {
            clang::Stmt *FuncBody = f->getBody();

            // Type name as string
            clang::QualType QT = f->getCallResultType();
            std::string TypeStr = QT.getAsString();

            // Function name
            clang::DeclarationName DeclName = f->getNameInfo().getName();
            std::string FuncName = DeclName.getAsString();

            // Add comment before
            std::stringstream SSBefore;
            SSBefore << "// Begin function " << FuncName << " returning "
                     << TypeStr << "\n";
            clang::SourceLocation ST = f->getSourceRange().getBegin();
            mRewriter->InsertText(ST, SSBefore.str(), true, true);

            // And after
            std::stringstream SSAfter;
            SSAfter << "\n// End function " << FuncName << "\n";
            ST = FuncBody->getLocEnd().getLocWithOffset(1);
            mRewriter->InsertText(ST, SSAfter.str(), true, true);
        }

        return true;
    }


    bool VisitDeclRefExpr(clang::DeclRefExpr *E) {
        /* #define PRINTVAL(VAR) (printf(":%d " #VAR " = %lu\n", __LINE__, (int64_t)(VAR)), (VAR))
         */
        if (auto varDecl = llvm::dyn_cast<clang::VarDecl>(E->getDecl())) {
            mRewriter->ReplaceText(clang::SourceRange(E->getLocStart(), E->getLocEnd()), "PRINTVAL(" + varDecl->getNameAsString() + ")");
        }

        return true;
    }


    bool VisitStmt(clang::Stmt *s) {
        if (llvm::isa<clang::IfStmt>(s)) {
            clang::IfStmt *IfStatement = llvm::cast<clang::IfStmt>(s);
            clang::Stmt *Then = IfStatement->getThen();

            mRewriter->InsertText(Then->getLocStart(),
                                   "// the 'if' part\n",
                                   true, true);

            clang::Stmt *Else = IfStatement->getElse();
            if (Else)
                mRewriter->InsertText(Else->getLocStart(),
                                       "// the 'else' part\n",
                                       true, true);
        }

//        if (llvm::isa<clang::Expr>(s)) {
////            clang::Expr *E = llvm::cast<clang::Expr>(s);
//            mRewriter->InsertTextBefore(s->getLocStart(), R"({ printf("abc"); })");
//        }

        return true;
    }


    ~VariableTrackerVisitor() {
        // TODO: This place is stupid!
        clang::RewriteBuffer const* RewriteBuf = mRewriter->getRewriteBufferFor(mContext->getSourceManager().getMainFileID());
        llvm::outs() << "==================\n"
                     << std::string(RewriteBuf->begin(), RewriteBuf->end());
    }

private:
    clang::ASTContext* mContext;
    std::unique_ptr<clang::Rewriter> mRewriter;
};


class VariableTrackerConsumer final : public clang::ASTConsumer {
public:
    explicit VariableTrackerConsumer(clang::ASTContext* context, std::unique_ptr<clang::Rewriter> rewriter)
        : mVisitor(context, std::move(rewriter)) {}

    virtual void HandleTranslationUnit(clang::ASTContext& context) {
        mVisitor.TraverseDecl(context.getTranslationUnitDecl());
        llvm::outs().changeColor(llvm::raw_ostream::Colors::RED, true);
        llvm::outs().resetColor();
    }

private:
    VariableTrackerVisitor mVisitor;
};


class VariableTrackerAction final : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile) override {
        auto rewriter = std::make_unique<clang::Rewriter>();
        rewriter->setSourceMgr(compiler.getSourceManager(), compiler.getLangOpts());
        return std::make_unique<VariableTrackerConsumer>(&compiler.getASTContext(), std::move(rewriter));
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
    return Tool.run(clang::tooling::newFrontendActionFactory<VariableTrackerAction>().get());
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
