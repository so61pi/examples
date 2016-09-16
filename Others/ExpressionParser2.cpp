#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <sys/mman.h>
#include <unistd.h>


namespace std { // very hacky

    template <typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

} // namespace std


template <typename Pointer>
class nonull_ptr {
public:
    using pointer      = typename Pointer::pointer;
    using element_type = typename Pointer::element_type;

    static pointer pointer_to(element_type& r) { return std::pointer_traits<Pointer>::pointer_to(r); }

public:
    template <typename T>
    explicit nonull_ptr(T&& pointer) : mPointer(std::forward<T>(pointer)) {
        if (mPointer == nullptr) throw std::runtime_error("nonull_ptr doesn't accept a null pointer");
    }

    pointer get() const { return &(*mPointer); }
    element_type& operator*() const { return *mPointer; }
    pointer operator->() const { return get(); }

private:
    Pointer mPointer;
};

template <typename T>
using nonull_unique_ptr = nonull_ptr<std::unique_ptr<T>>;


/**
 * <expression> ::= <term> + <expression> | <term> - <expression> | <term>
 * <term> ::= <factor> * <term> | <factor> / <term> | <factor>
 * <factor> ::= (<expression>) | <number> | <var>
 *
 * Expr = AddExpr | SubExpr | Term
 * AddExpr = Term + Expr
 * SubExpr = Term - Expr
 * Term = MulTerm | DivTerm | Factor
 * MulTerm = Factor * Term
 * DivTerm = Factor / Term
 * Factor = ParenExpr | Number | Var
 * ParenExpr = (Expr)
 *
 * CompoundStmt = *Stmt
 * Stmt = PrintStmt;
 * | AssignStmt;
 * PrintStmt = print Var;
 * AssignStmt = LeftVar '=' Expr;
*/

#define TokenTypes                      \
        EnumString(EndOfSource),        \
        EnumString(Number),             \
        EnumString(OpAdd),              \
        EnumString(OpSub),              \
        EnumString(OpMul),              \
        EnumString(OpDiv),              \
        EnumString(LParen),             \
        EnumString(RParen),             \
        EnumString(Var),                \
        EnumString(Equal),              \
        EnumString(SemiColon),          \
        EnumString(KwPrint),            \
        /**/

enum class TokType {
#undef EnumString
#define EnumString(x) x
    TokenTypes
};

static char const* gTokTypeString[] = {
#undef EnumString
#define EnumString(x) #x
    TokenTypes
};

struct Keyword {
    char const* string;
    std::size_t length;
    TokType     type;
};

static Keyword const gKeywords[] = {
    {"print", sizeof("print") - 1, TokType::KwPrint}
};

bool GetKeyword(char const* p, Keyword& keyword) {
    for (auto const& kw : gKeywords) {
        bool mismatch = false;
        for (std::size_t i = 0; i < kw.length; ++i) {
            if (p[i] != kw.string[i]) {
                mismatch = true;
                break;
            }
        }
        if (mismatch || std::isalnum(p[kw.length])) continue;

        keyword = kw;
        return true;
    }
    return false;
}

char const* to_string(TokType type) { return gTokTypeString[static_cast<int>(type)]; }


using LineColumn = std::pair<unsigned, unsigned>;
std::string to_string(LineColumn const& loc) { return std::to_string(loc.first) + ':' + std::to_string(loc.second); }


class Source {
public:
    explicit Source(char const* source) : mSource(source) { mFileName = "---"; }

    std::string const& FileName() const { return mFileName; }

    char const* begin() const { return mSource.c_str(); }
    char const* end() const { return mSource.c_str() + mSource.size(); }

    LineColumn GetLineColumn(char const* ptr) const {
        unsigned line   = 1;
        unsigned column = 1;
        for (auto it = begin(); it != end(); ++it) {
            if (it == ptr) break;
            if (*it == '\n') {
                ++line;
                column = 1;
            } else {
                ++column;
            }
        }
        return std::make_pair(line, column);
    }

    std::string GetSourceLine(char const* ptr) const {
        auto        lineBegin = begin();
        char const* lineEnd   = nullptr;
        do {
            lineEnd = std::find(lineBegin, end(), '\n');
            if (lineBegin <= ptr && ptr <= lineEnd) {
                return std::string(lineBegin, lineEnd);
            }
            lineBegin = lineEnd + 1;
        } while (lineEnd != end());

        // should throw error instead
        return std::string();
    }

private:
    std::string mFileName;
    std::string mSource;
};

///
///
///
class SourceRange {
public:
    explicit SourceRange(Source& source, char const* begin, char const* end)
        : mSource(source), mBegin(begin), mEnd(end) {}

    LineColumn BeginLocation() const { return mSource.GetLineColumn(mBegin); }
    LineColumn EndLocation() const { return mSource.GetLineColumn(mEnd); }

    std::string GetSourceString() const { return std::string(mBegin, mEnd); }
    std::string GetSourceLine() const { return mSource.GetSourceLine(mBegin); }

    Source& GetSource() const { return mSource; }

    char const* begin() const { return mBegin; }
    char const* end() const { return mEnd; }

private:
    Source&     mSource;
    char const* mBegin;
    char const* mEnd;
};

std::string to_string(SourceRange const& range) {
    return range.GetSource().FileName() + ':' + to_string(range.BeginLocation());
}

std::ostream& operator<<(std::ostream& os, SourceRange const& range) {
    os << to_string(range);
    return os;
}


///
///
///
class Token {
public:
    explicit Token(TokType type, SourceRange range) : mType(type), mSourceRange(range) {}

    TokType     Type() const { return mType; }
    SourceRange GetSourceRange() const { return mSourceRange; }

    bool Is(TokType type) const { return type == mType; }
    bool IsNot(TokType type) const { return !Is(type); }

private:
    TokType     mType;
    SourceRange mSourceRange;
};

std::string to_string(Token const& token) {
    return std::string("<") + to_string(token.Type()) + ':' + to_string(token.GetSourceRange()) + '>';
}

std::ostream& operator<<(std::ostream& os, Token const& token) {
    os << to_string(token);
    return os;
}


///
///
///
using token_container_type = std::vector<Token>;
using token_const_iterator = token_container_type::const_iterator;

std::ostream& Dump(std::ostream& os, token_container_type const& tokens) {
    for (auto const& token : tokens) {
        std::cout << token;
    }
    return os;
}


///
///
///
class Storage {
public:
    template <typename T, typename... Args>
    T const* Allocate(Args&&... args) {
#if 0
        auto pagesize = sysconf(_SC_PAGE_SIZE);
        if (pagesize == -1) {
            perror("sysconf");
            exit(EXIT_FAILURE);
        }
        auto size = (sizeof(T) / pagesize + 1) * pagesize;
        auto buffer = aligned_alloc(pagesize, size);
        new (buffer) T(std::forward<Args>(args)...);
        if (mprotect(buffer, size, PROT_READ) == -1) {
            perror("mprotect");
            exit(EXIT_FAILURE);
        }
        return reinterpret_cast<T const*>(buffer);
#else
        mMemory.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
        return reinterpret_cast<T const*>(mMemory.back().get());
#endif
    }

private:
    std::vector<std::shared_ptr<void>> mMemory;
};


///
///
///
enum class DiagnosticKind { Warning, Error };

char const* to_string(DiagnosticKind kind) {
    switch (kind) {
    case DiagnosticKind::Warning:
        return "warning";

    case DiagnosticKind::Error:
        return "error";
    }
    return "unknown";
}


///
/// \brief The Diagnostic class
///
class Diagnostic {
public:
    explicit Diagnostic(DiagnosticKind kind, SourceRange range, std::string message)
        : mKind(kind), mSourceRange(range), mMessage(message) {}

    virtual ~Diagnostic() = default;

    DiagnosticKind      Kind() const { return mKind; }
    SourceRange const&  GetSourceRange() const { return mSourceRange; }
    virtual std::string GetMessage() const { return Message(); }

protected:
    std::string const& Message() const { return mMessage; }

private:
    DiagnosticKind mKind;
    SourceRange    mSourceRange;
    std::string    mMessage;
};


std::string to_string(Diagnostic const& diag) {
    return to_string(diag.GetSourceRange()) + ": " + to_string(diag.Kind()) + ": " + diag.GetMessage();
}


///
///
///
template <typename... Args>
std::unique_ptr<Diagnostic> make_diag(Args&&... args) {
    return std::make_unique<Diagnostic>(std::forward<Args>(args)...);
}

template <typename... Args>
std::unique_ptr<Diagnostic> make_warning_diag(Args&&... args) {
    return make_diag(DiagnosticKind::Warning, std::forward<Args>(args)...);
}

template <typename... Args>
std::unique_ptr<Diagnostic> make_error_diag(Args&&... args) {
    return make_diag(DiagnosticKind::Error, std::forward<Args>(args)...);
}


///
/// \brief The DiagnosticContainer class
///
class DiagnosticContainer {
public:
    using container_type = std::vector<nonull_ptr<std::unique_ptr<Diagnostic>>>;

public:
    bool HaveDiag() const { return !mDiags.empty(); }
    explicit operator bool() const { return HaveDiag(); }

    Diagnostic& Add(std::unique_ptr<Diagnostic> diag) {
        mDiags.emplace_back(std::move(diag));
        return *mDiags.back();
    }

    Diagnostic& Add(nonull_ptr<std::unique_ptr<Diagnostic>> diag) {
        mDiags.emplace_back(std::move(diag));
        return *mDiags.back();
    }

    std::ostream& Print(std::ostream& os) const {
        for (auto const& diag : mDiags) {
            std::size_t spaces = diag->GetSourceRange().BeginLocation().second;
            if (spaces) --spaces;
            os << to_string(*diag) << '\n'
               << diag->GetSourceRange().GetSourceLine() << '\n'
               << std::string(spaces, ' ') << '^' << '\n';
        }
        return os;
    }

private:
    container_type mDiags;
};

DiagnosticContainer& operator<<(DiagnosticContainer& diagContainer, std::unique_ptr<Diagnostic> diag) {
    diagContainer.Add(std::move(diag));
    return diagContainer;
}

std::ostream& operator<<(std::ostream& os, DiagnosticContainer const& diagContainer) { return diagContainer.Print(os); }


namespace lexer {

    class Lexer {
    public:
        explicit Lexer(DiagnosticContainer& diagContainer, Source& source)
            : mDiagContainer(diagContainer), mSource(source) {
            mCurrent = mSource.begin();
        }

        bool Lex(token_container_type& tokens) {
            bool result;
            while ((result = LexToken(tokens))) {
                if (tokens.back().Is(TokType::EndOfSource)) return true;
            }
            return result;
        }

    private:
        bool LexToken(token_container_type& tokens) {
            SkipWhitespace();
            switch (*mCurrent) {
            case '\0':
                tokens.push_back(Token(TokType::EndOfSource, SourceRange(mSource, mCurrent, mCurrent)));
                break;

            case '(':
                tokens.push_back(Token(TokType::LParen, SourceRange(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case ')':
                tokens.push_back(Token(TokType::RParen, SourceRange(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case '+':
                tokens.push_back(Token(TokType::OpAdd, SourceRange(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case '-':
                tokens.push_back(Token(TokType::OpSub, SourceRange(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case '*':
                tokens.push_back(Token(TokType::OpMul, SourceRange(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case '/':
                tokens.push_back(Token(TokType::OpDiv, SourceRange(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case '=':
                tokens.push_back(Token(TokType::Equal, SourceRange(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case ';':
                tokens.push_back(Token(TokType::SemiColon, SourceRange(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return LexNumber(tokens);

            default: {
                if (std::isalpha(*mCurrent)) {
                    Keyword kw;
                    if (GetKeyword(mCurrent, kw)) {
                        tokens.push_back(Token(kw.type, SourceRange(mSource, mCurrent, mCurrent + 5)));
                        mCurrent += kw.length;
                        break;
                    } else {
                        return LexVar(tokens);
                    }
                } else {
                    mDiagContainer << make_error_diag(SourceRange(mSource, mCurrent, mCurrent + 1),
                                                      "unknown character");
                    return false;
                }
            }
            }
            return true;
        }

        bool LexNumber(token_container_type& tokens) {
            auto begin = mCurrent;
            auto end   = mCurrent;
            while (std::isdigit(*end))
                ++end;
            mCurrent = end;
            tokens.push_back(Token(TokType::Number, SourceRange(mSource, begin, end)));
            return true;
        }

        bool LexVar(token_container_type& tokens) {
            auto begin = mCurrent;
            auto end   = mCurrent;
            while (std::isalpha(*end))
                ++end;
            mCurrent = end;
            tokens.push_back(Token(TokType::Var, SourceRange(mSource, begin, end)));
            return true;
        }

        void SkipWhitespace() {
            while (*mCurrent == ' ' || *mCurrent == '\t' || *mCurrent == '\r' || *mCurrent == '\n')
                ++mCurrent;
        }

    private:
        DiagnosticContainer& mDiagContainer;
        Source&              mSource;
        char const*          mCurrent;
    };

} // namespace lexer


namespace ast {

    class Visitor;
    class ASTNode;

    class Expr;
    class AddExpr;
    class SubExpr;
    class Term;
    class MulTerm;
    class DivTerm;
    class Factor;
    class Number;
    class Var;
    class ParenExpr;
    class Stmt;
    class CompoundStmt;
    class PrintStmt;
    class AssignStmt;
    class LeftVar;

    ///
    ///
    ///
    enum class VisitorTraverseOrder { PreOrder, PostOrder };

    class Visitor {
    public:
        virtual ~Visitor();

        virtual VisitorTraverseOrder TraverseOrder() const { return VisitorTraverseOrder::PreOrder; }

        virtual bool Visit(Expr const&) { return true; }
        virtual bool Visit(AddExpr const&) { return true; }
        virtual bool Visit(SubExpr const&) { return true; }
        virtual bool Visit(Term const&) { return true; }
        virtual bool Visit(MulTerm const&) { return true; }
        virtual bool Visit(DivTerm const&) { return true; }
        virtual bool Visit(Factor const&) { return true; }
        virtual bool Visit(Number const&) { return true; }
        virtual bool Visit(Var const&) { return true; }
        virtual bool Visit(ParenExpr const&) { return true; }
        virtual bool Visit(Stmt const&) { return true; }
        virtual bool Visit(CompoundStmt const&) { return true; }
        virtual bool Visit(PrintStmt const&) { return true; }
        virtual bool Visit(AssignStmt const&) { return true; }
        virtual bool Visit(LeftVar const&) { return true; }

        virtual bool WalkUpFrom(Expr const&);
        virtual bool WalkUpFrom(AddExpr const&);
        virtual bool WalkUpFrom(SubExpr const&);
        virtual bool WalkUpFrom(Term const&);
        virtual bool WalkUpFrom(MulTerm const&);
        virtual bool WalkUpFrom(DivTerm const&);
        virtual bool WalkUpFrom(Factor const&);
        virtual bool WalkUpFrom(Number const&);
        virtual bool WalkUpFrom(Var const&);
        virtual bool WalkUpFrom(ParenExpr const&);
        virtual bool WalkUpFrom(Stmt const&);
        virtual bool WalkUpFrom(CompoundStmt const&);
        virtual bool WalkUpFrom(PrintStmt const&);
        virtual bool WalkUpFrom(AssignStmt const&);
        virtual bool WalkUpFrom(LeftVar const&);

        virtual bool Traverse(Expr const&);
        virtual bool Traverse(AddExpr const&);
        virtual bool Traverse(SubExpr const&);
        virtual bool Traverse(Term const&);
        virtual bool Traverse(MulTerm const&);
        virtual bool Traverse(DivTerm const&);
        virtual bool Traverse(Factor const&);
        virtual bool Traverse(Number const&);
        virtual bool Traverse(Var const&);
        virtual bool Traverse(ParenExpr const&);
        virtual bool Traverse(Stmt const&);
        virtual bool Traverse(CompoundStmt const&);
        virtual bool Traverse(PrintStmt const&);
        virtual bool Traverse(AssignStmt const&);
        virtual bool Traverse(LeftVar const&);
    };

    Visitor::~Visitor() = default;

    ///
    /// \brief The ASTNode class
    /// [begin, end]
    ///
    class ASTNode {
    public:
        explicit ASTNode(token_const_iterator begin, token_const_iterator end) : mBegin(begin), mEnd(end) {}

        virtual ~ASTNode();

        Token const& TokenBegin() const { return *mBegin; }
        Token const& TokenEnd() const { return *mEnd; }
        std::string  String() const {
            return std::string(mBegin->GetSourceRange().begin(), mEnd->GetSourceRange().end());
        }
        virtual char const* NodeName() const = 0;

    private:
        token_const_iterator mBegin;
        token_const_iterator mEnd;
    };

    ASTNode::~ASTNode() = default;

    ///
    /// \brief The ExprKind enum
    ///
    enum class ExprKind { AddExpr, SubExpr, Term };

    class Expr : public ast::ASTNode {
    public:
        explicit Expr(token_const_iterator begin, token_const_iterator end, ExprKind kind)
            : ast::ASTNode(begin, end), mKind(kind) {}

        ExprKind Kind() const { return mKind; }

    private:
        ExprKind mKind;
    };

    class AddExpr : public ast::Expr {
    public:
        explicit AddExpr(token_const_iterator begin, token_const_iterator end, ast::Term const& lTerm,
                         ast::Expr const& rExpr)
            : ast::Expr(begin, end, ExprKind::AddExpr), mLTerm(lTerm), mRExpr(rExpr) {}

        ast::Term const&    Left() const { return Term(); }
        ast::Expr const&    Right() const { return Expr(); }
        ast::Term const&    Term() const { return mLTerm; }
        ast::Expr const&    Expr() const { return mRExpr; }
        virtual const char* NodeName() const { return "AddExpr"; }

    private:
        ast::Term const& mLTerm;
        ast::Expr const& mRExpr;
    };

    class SubExpr : public ast::Expr {
    public:
        explicit SubExpr(token_const_iterator begin, token_const_iterator end, ast::Term const& lTerm,
                         ast::Expr const& rExpr)
            : ast::Expr(begin, end, ExprKind::SubExpr), mLTerm(lTerm), mRExpr(rExpr) {}

        ast::Term const&    Left() const { return Term(); }
        ast::Expr const&    Right() const { return Expr(); }
        ast::Term const&    Term() const { return mLTerm; }
        ast::Expr const&    Expr() const { return mRExpr; }
        virtual const char* NodeName() const { return "SubExpr"; }

    private:
        ast::Term const& mLTerm;
        ast::Expr const& mRExpr;
    };

    ///
    ///
    ///
    enum class TermKind { MulTerm, DivTerm, Factor };

    class Term : public ast::Expr {
    public:
        explicit Term(token_const_iterator begin, token_const_iterator end, TermKind kind)
            : ast::Expr(begin, end, ExprKind::Term), mKind(kind) {}

        TermKind Kind() const { return mKind; }

    private:
        TermKind mKind;
    };

    class MulTerm : public ast::Term {
    public:
        explicit MulTerm(token_const_iterator begin, token_const_iterator end, ast::Factor const& lFactor,
                         ast::Term const& rTerm)
            : ast::Term(begin, end, TermKind::MulTerm), mLFactor(lFactor), mRTerm(rTerm) {}

        ast::Factor const&  Left() const { return Factor(); }
        ast::Term const&    Right() const { return Term(); }
        ast::Factor const&  Factor() const { return mLFactor; }
        ast::Term const&    Term() const { return mRTerm; }
        virtual const char* NodeName() const { return "MulTerm"; }

    private:
        ast::Factor const& mLFactor;
        ast::Term const&   mRTerm;
    };

    class DivTerm : public ast::Term {
    public:
        explicit DivTerm(token_const_iterator begin, token_const_iterator end, ast::Factor const& lFactor,
                         ast::Term const& rTerm)
            : ast::Term(begin, end, TermKind::DivTerm), mLFactor(lFactor), mRTerm(rTerm) {}

        ast::Factor const&  Left() const { return Factor(); }
        ast::Term const&    Right() const { return Term(); }
        ast::Factor const&  Factor() const { return mLFactor; }
        ast::Term const&    Term() const { return mRTerm; }
        virtual const char* NodeName() const { return "DivTerm"; }

    private:
        ast::Factor const& mLFactor;
        ast::Term const&   mRTerm;
    };

    ///
    ///
    ///
    enum class FactorKind { ParenExpr, Number, Var };

    class Factor : public ast::Term {
    public:
        explicit Factor(token_const_iterator begin, token_const_iterator end, FactorKind kind)
            : ast::Term(begin, end, TermKind::Factor), mKind(kind) {}

        FactorKind Kind() const { return mKind; }

    private:
        FactorKind mKind;
    };

    class Number : public ast::Factor {
    public:
        explicit Number(token_const_iterator begin, token_const_iterator end, int value)
            : ast::Factor(begin, end, FactorKind::Number), mValue(value) {}

        int                 Value() const { return mValue; }
        virtual const char* NodeName() const { return "Number"; }

    private:
        int mValue;
    };

    class Var : public ast::Factor {
    public:
        explicit Var(token_const_iterator begin, token_const_iterator end, std::string const& name)
            : ast::Factor(begin, end, FactorKind::Var), mName(name) {}

        std::string const&  Name() const { return mName; }
        virtual const char* NodeName() const { return "Var"; }

    private:
        std::string mName;
    };

    class ParenExpr : public ast::Factor {
    public:
        explicit ParenExpr(token_const_iterator begin, token_const_iterator end, ast::Expr const& expr)
            : ast::Factor(begin, end, FactorKind::ParenExpr), mExpr(expr) {}

        ast::Expr const&    Expr() const { return mExpr; }
        virtual const char* NodeName() const { return "ParenExpr"; }

    private:
        ast::Expr const& mExpr;
    };

    ///
    ///
    ///
    enum class StmtKind { Print, Assign };

    class Stmt : public ast::ASTNode {
    public:
        explicit Stmt(token_const_iterator begin, token_const_iterator end, StmtKind kind)
            : ASTNode(begin, end), mKind(kind) {}

        StmtKind Kind() const { return mKind; }

    private:
        StmtKind mKind;
    };

    class CompoundStmt : public ast::ASTNode {
    public:
        using container_type = std::vector<std::reference_wrapper<Stmt const>>;

        explicit CompoundStmt(token_const_iterator begin, token_const_iterator end) : ast::ASTNode(begin, end) {}
        explicit CompoundStmt(token_const_iterator begin, token_const_iterator end, container_type stmts)
            : ast::ASTNode(begin, end), mStmts(std::move(stmts)) {}

        void                  AddStmt(Stmt& stmt) { mStmts.emplace_back(stmt); }
        container_type const& Stmts() const { return mStmts; }
        virtual const char*   NodeName() const { return "CompoundStmt"; }

    private:
        container_type mStmts;
    };

    class PrintStmt : public ast::Stmt {
    public:
        explicit PrintStmt(token_const_iterator begin, token_const_iterator end, ast::Var const& var)
            : ast::Stmt(begin, end, StmtKind::Print), mVar(var) {}

        ast::Var const&     Var() const { return mVar; }
        virtual const char* NodeName() const { return "PrintStmt"; }

    private:
        ast::Var const& mVar;
    };

    class AssignStmt : public ast::Stmt {
    public:
        explicit AssignStmt(token_const_iterator begin, token_const_iterator end, ast::LeftVar const& leftVar,
                            ast::Expr const& expr)
            : ast::Stmt(begin, end, StmtKind::Assign), mLeftVar(leftVar), mExpr(expr) {}

        ast::LeftVar const& LeftVar() const { return mLeftVar; }
        ast::Expr const&    Expr() const { return mExpr; }
        virtual const char* NodeName() const { return "PrintStmt"; }

    private:
        ast::LeftVar const& mLeftVar;
        ast::Expr const&    mExpr;
    };

    class LeftVar : public ast::ASTNode {
    public:
        explicit LeftVar(token_const_iterator begin, token_const_iterator end, std::string const& name)
            : ast::ASTNode(begin, end), mName(name) {}

        std::string const&  Name() const { return mName; }
        virtual const char* NodeName() const { return "LeftVar"; }

    private:
        std::string mName;
    };

    ///
    ///
    ///
    bool Visitor::WalkUpFrom(Expr const& expr) { return Visit(expr); }
    bool Visitor::WalkUpFrom(AddExpr const& addExpr) {
        return WalkUpFrom(static_cast<Expr const&>(addExpr)) && Visit(addExpr);
    }
    bool Visitor::WalkUpFrom(SubExpr const& subExpr) {
        return WalkUpFrom(static_cast<Expr const&>(subExpr)) && Visit(subExpr);
    }
    bool Visitor::WalkUpFrom(Term const& term) { return WalkUpFrom(static_cast<Expr const&>(term)) && Visit(term); }
    bool Visitor::WalkUpFrom(MulTerm const& mulTerm) {
        return WalkUpFrom(static_cast<Term const&>(mulTerm)) && Visit(mulTerm);
    }
    bool Visitor::WalkUpFrom(DivTerm const& divTerm) {
        return WalkUpFrom(static_cast<Term const&>(divTerm)) && Visit(divTerm);
    }
    bool Visitor::WalkUpFrom(Factor const& factor) {
        return WalkUpFrom(static_cast<Term const&>(factor)) && Visit(factor);
    }
    bool Visitor::WalkUpFrom(Number const& number) {
        return WalkUpFrom(static_cast<Factor const&>(number)) && Visit(number);
    }
    bool Visitor::WalkUpFrom(Var const& var) { return WalkUpFrom(static_cast<Factor const&>(var)) && Visit(var); }
    bool Visitor::WalkUpFrom(ParenExpr const& parenExpr) {
        return WalkUpFrom(static_cast<Factor const&>(parenExpr)) && Visit(parenExpr);
    }
    bool Visitor::WalkUpFrom(Stmt const& stmt) { return Visit(stmt); }
    bool Visitor::WalkUpFrom(CompoundStmt const& compoundStmt) { return Visit(compoundStmt); }
    bool Visitor::WalkUpFrom(PrintStmt const& printStmt) {
        return WalkUpFrom(static_cast<Stmt const&>(printStmt)) && Visit(printStmt);
    }
    bool Visitor::WalkUpFrom(AssignStmt const& assignStmt) {
        return WalkUpFrom(static_cast<Stmt const&>(assignStmt)) && Visit(assignStmt);
    }
    bool Visitor::WalkUpFrom(LeftVar const& leftVar) { return Visit(leftVar); }

    ///
    ///
    ///
    bool Visitor::Traverse(Expr const& expr) {
        // if (!WalkUpFrom(expr)) return false;
        switch (expr.Kind()) {
        case ExprKind::AddExpr:
            return Traverse(dynamic_cast<AddExpr const&>(expr));

        case ExprKind::SubExpr:
            return Traverse(dynamic_cast<SubExpr const&>(expr));

        case ExprKind::Term:
            return Traverse(dynamic_cast<Term const&>(expr));
        }
        return false;
    }
    bool Visitor::Traverse(AddExpr const& addExpr) {
        switch (TraverseOrder()) {
        case VisitorTraverseOrder::PreOrder:
            return WalkUpFrom(addExpr) && Traverse(addExpr.Left()) && Traverse(addExpr.Right());

        case VisitorTraverseOrder::PostOrder:
            return Traverse(addExpr.Left()) && Traverse(addExpr.Right()) && WalkUpFrom(addExpr);
        }
        return true;
    }
    bool Visitor::Traverse(SubExpr const& subExpr) {
        switch (TraverseOrder()) {
        case VisitorTraverseOrder::PreOrder:
            return WalkUpFrom(subExpr) && Traverse(subExpr.Left()) && Traverse(subExpr.Right());

        case VisitorTraverseOrder::PostOrder:
            return Traverse(subExpr.Left()) && Traverse(subExpr.Right()) && WalkUpFrom(subExpr);
        }
        return true;
    }
    bool Visitor::Traverse(Term const& term) {
        // if (!WalkUpFrom(term)) return false;
        switch (term.Kind()) {
        case TermKind::DivTerm:
            return Traverse(dynamic_cast<DivTerm const&>(term));

        case TermKind::Factor:
            return Traverse(dynamic_cast<Factor const&>(term));

        case TermKind::MulTerm:
            return Traverse(dynamic_cast<MulTerm const&>(term));
        }
        return false;
    }
    bool Visitor::Traverse(MulTerm const& mulTerm) {
        switch (TraverseOrder()) {
        case VisitorTraverseOrder::PreOrder:
            return WalkUpFrom(mulTerm) && Traverse(mulTerm.Left()) && Traverse(mulTerm.Right());

        case VisitorTraverseOrder::PostOrder:
            return Traverse(mulTerm.Left()) && Traverse(mulTerm.Right()) && WalkUpFrom(mulTerm);
        }
        return true;
    }
    bool Visitor::Traverse(DivTerm const& divTerm) {
        switch (TraverseOrder()) {
        case VisitorTraverseOrder::PreOrder:
            return WalkUpFrom(divTerm) && Traverse(divTerm.Left()) && Traverse(divTerm.Right());

        case VisitorTraverseOrder::PostOrder:
            return Traverse(divTerm.Left()) && Traverse(divTerm.Right()) && WalkUpFrom(divTerm);
        }
        return true;
    }
    bool Visitor::Traverse(Factor const& factor) {
        switch (factor.Kind()) {
        case FactorKind::Number:
            return Traverse(dynamic_cast<Number const&>(factor));

        case FactorKind::ParenExpr:
            return Traverse(dynamic_cast<ParenExpr const&>(factor));

        case FactorKind::Var:
            return Traverse(dynamic_cast<Var const&>(factor));
        }
        return false;
    }
    bool Visitor::Traverse(Number const& number) { return WalkUpFrom(number); }
    bool Visitor::Traverse(Var const& var) { return WalkUpFrom(var); }
    bool Visitor::Traverse(ParenExpr const& parenExpr) {
        switch (TraverseOrder()) {
        case VisitorTraverseOrder::PreOrder:
            return WalkUpFrom(parenExpr) && Traverse(parenExpr.Expr());

        case VisitorTraverseOrder::PostOrder:
            return Traverse(parenExpr.Expr()) && WalkUpFrom(parenExpr);
        }
        return true;
    }

    bool Visitor::Traverse(Stmt const& stmt) {
        switch (stmt.Kind()) {
        case StmtKind::Assign:
            return Traverse(dynamic_cast<AssignStmt const&>(stmt));

        case StmtKind::Print:
            return Traverse(dynamic_cast<PrintStmt const&>(stmt));
        }
        return false;
    }
    bool Visitor::Traverse(CompoundStmt const& compoundStmt) {
        for (auto const stmt : compoundStmt.Stmts()) {
            if (Traverse(stmt.get()) == false) {
                return false;
            }
        }
        return true;
    }
    bool Visitor::Traverse(PrintStmt const& printStmt) {
        switch (TraverseOrder()) {
        case VisitorTraverseOrder::PreOrder:
            return WalkUpFrom(printStmt) && Traverse(printStmt.Var());

        case VisitorTraverseOrder::PostOrder:
            return Traverse(printStmt.Var()) && WalkUpFrom(printStmt);
        }
        return false;
    }
    bool Visitor::Traverse(AssignStmt const& assignStmt) {
        switch (TraverseOrder()) {
        case VisitorTraverseOrder::PreOrder:
            return WalkUpFrom(assignStmt) && Traverse(assignStmt.Expr()) && Traverse(assignStmt.LeftVar());

        case VisitorTraverseOrder::PostOrder:
            return Traverse(assignStmt.Expr()) && Traverse(assignStmt.LeftVar()) && WalkUpFrom(assignStmt);
        }
        return false;
    }
    bool Visitor::Traverse(LeftVar const& leftVar) { return WalkUpFrom(leftVar); }

} // namespace ast


namespace parser {

    template <typename AST>
    class ParseResult {
    public:
        using ast_node_type = AST;

    public:
        ParseResult() : mPointer(nullptr) {}

        template <typename U>
        explicit ParseResult(U* u) : mPointer(u) {}

        template <typename U>
        ParseResult& operator=(ParseResult<U> const& rhs) {
            mPointer = rhs.ptr();
            return *this;
        }

        bool IsOK() const { return mPointer != nullptr; }
        explicit operator bool() const { return IsOK(); }

        operator ast_node_type const&() const { return ref(); }
        ast_node_type const& ref() const { return *mPointer; }
        ast_node_type const* ptr() const { return mPointer; }

    private:
        ast_node_type const* mPointer;
    };

    class Parser {
    public:
        explicit Parser(DiagnosticContainer& diagContainer, token_const_iterator tok)
            : mDiagContainer(diagContainer), mTok(tok), mSuccess(true) {}

        ParseResult<ast::CompoundStmt> Parse() { return ParseCompoundStmt(mTok); }
        bool                           Success() const { return mSuccess; }

    private:
        ParseResult<ast::Expr> ParseExpr(token_const_iterator& stok);
        ParseResult<ast::Expr> ParseAddExpr(token_const_iterator& stok);
        ParseResult<ast::Expr> ParseSubExpr(token_const_iterator& stok);
        ParseResult<ast::Term> ParseTerm(token_const_iterator& stok);
        ParseResult<ast::Term> ParseMulTerm(token_const_iterator& stok);
        ParseResult<ast::Term> ParseDivTerm(token_const_iterator& stok);
        ParseResult<ast::Factor> ParseFactor(token_const_iterator& stok);
        ParseResult<ast::Number> ParseNumber(token_const_iterator& stok);
        ParseResult<ast::Var> ParseVar(token_const_iterator& stok);
        ParseResult<ast::ParenExpr> ParseParenExpr(token_const_iterator& stok);
        ParseResult<ast::CompoundStmt> ParseCompoundStmt(token_const_iterator& stok);
        ParseResult<ast::Stmt> ParseStmt(token_const_iterator& stok);
        ParseResult<ast::Stmt> ParsePrintStmt(token_const_iterator& stok);
        ParseResult<ast::Stmt> ParseAssignStmt(token_const_iterator& stok);
        ParseResult<ast::LeftVar> ParseLeftVar(token_const_iterator& stok);

    private:
        DiagnosticContainer& mDiagContainer;
        Storage              mStorage;
        token_const_iterator mTok;
        bool                 mSuccess;
    };

    ParseResult<ast::Expr> Parser::ParseExpr(token_const_iterator& stok) {
        auto r = ParseAddExpr(stok);
        if (r) return r;

        r = ParseSubExpr(stok);
        if (r) return r;

        r = ParseTerm(stok);
        if (r) return r;

        return {};
    }

    ParseResult<ast::Expr> Parser::ParseAddExpr(token_const_iterator& stok) {
        auto btok = stok;

        auto itok = btok;
        auto term = ParseTerm(itok);
        if (!term) return {};

        if (itok->IsNot(TokType::OpAdd)) {
            return {};
        }
        ++itok;

        auto expr = ParseExpr(itok);
        if (!expr) return {};

        stok      = itok;
        auto etok = stok - 1;

        return ParseResult<ast::Expr>(mStorage.Allocate<ast::AddExpr>(btok, etok, term, expr));
    }

    ParseResult<ast::Expr> Parser::ParseSubExpr(token_const_iterator& stok) {
        auto btok = stok;

        auto itok = btok;
        auto term = ParseTerm(itok);
        if (!term) return {};

        if (itok->IsNot(TokType::OpSub)) {
            return {};
        }
        ++itok;

        auto expr = ParseExpr(itok);
        if (!expr) {
            mDiagContainer << make_error_diag((itok - 1)->GetSourceRange(), "need an expression after '-'");
            return {};
        }

        stok      = itok;
        auto etok = stok - 1;

        return ParseResult<ast::Expr>(mStorage.Allocate<ast::SubExpr>(btok, etok, term, expr));
    }

    ParseResult<ast::Term> Parser::ParseTerm(token_const_iterator& stok) {
        ParseResult<ast::Term> r;
        r = ParseMulTerm(stok);
        if (r) return r;

        r = ParseDivTerm(stok);
        if (r) return r;

        r = ParseFactor(stok);
        if (r) return r;

        return {};
    }

    ParseResult<ast::Term> Parser::ParseMulTerm(token_const_iterator& stok) {
        auto btok = stok;

        auto itok   = btok;
        auto factor = ParseFactor(itok);
        if (!factor) return {};

        if (itok->IsNot(TokType::OpMul)) {
            return {};
        }
        ++itok;

        auto term = ParseTerm(itok);
        if (!term) return {};

        stok      = itok;
        auto etok = stok - 1;

        return ParseResult<ast::Term>(mStorage.Allocate<ast::MulTerm>(btok, etok, factor, term));
    }

    ParseResult<ast::Term> Parser::ParseDivTerm(token_const_iterator& stok) {
        auto btok = stok;

        auto itok   = btok;
        auto factor = ParseFactor(itok);
        if (!factor) return {};

        if (itok->IsNot(TokType::OpDiv)) {
            return {};
        }
        ++itok;

        auto term = ParseTerm(itok);
        if (!term) return {};

        stok      = itok;
        auto etok = stok - 1;

        return ParseResult<ast::Term>(mStorage.Allocate<ast::DivTerm>(btok, etok, factor, term));
    }

    ParseResult<ast::Factor> Parser::ParseFactor(token_const_iterator& stok) {
        ParseResult<ast::Factor> r;
        r = ParseParenExpr(stok);
        if (r) return r;

        r = ParseNumber(stok);
        if (r) return r;

        r = ParseVar(stok);
        if (r) return r;

        return {};
    }

    ParseResult<ast::Number> Parser::ParseNumber(token_const_iterator& stok) {
        auto btok = stok;
        if (stok->IsNot(TokType::Number)) {
            return {};
        }

        auto num = std::stoi(stok->GetSourceRange().GetSourceString());
        ++stok;

        return ParseResult<ast::Number>(mStorage.Allocate<ast::Number>(btok, stok - 1, num));
    }

    ParseResult<ast::Var> Parser::ParseVar(token_const_iterator& stok) {
        auto btok = stok;
        if (stok->IsNot(TokType::Var)) {
            return {};
        }
        ++stok;
        return ParseResult<ast::Var>(
            mStorage.Allocate<ast::Var>(btok, stok - 1, btok->GetSourceRange().GetSourceString()));
    }

    ParseResult<ast::ParenExpr> Parser::ParseParenExpr(token_const_iterator& stok) {
        auto btok = stok;

        auto itok  = btok;
        auto saved = itok;
        if (itok->IsNot(TokType::LParen)) {
            return {};
        }
        ++itok;

        saved     = itok;
        auto expr = ParseExpr(itok);
        if (!expr) return {};

        saved = itok;
        if (itok->IsNot(TokType::RParen)) {
            mDiagContainer << make_error_diag(itok->GetSourceRange(), "missing ')' for ParenExpr");
            return {};
        }
        ++itok;

        stok      = itok;
        auto etok = stok - 1;

        return ParseResult<ast::ParenExpr>(mStorage.Allocate<ast::ParenExpr>(btok, etok, expr));
    }

    ParseResult<ast::CompoundStmt> Parser::ParseCompoundStmt(token_const_iterator& stok) {
        auto btok = stok;

        ast::CompoundStmt::container_type stmts;
        auto                              itok = btok;
        while (itok->IsNot(TokType::EndOfSource)) {
            auto r = ParseStmt(itok);
            if (!r) return {};
            stmts.emplace_back(r.ref());
        }
        stok = itok;

        auto compoundStmt = mStorage.Allocate<ast::CompoundStmt>(btok, itok - 1, std::move(stmts));
        //compoundStmt->Replace(std::move(stmts));
        return ParseResult<ast::CompoundStmt>(compoundStmt);
    }

    ParseResult<ast::Stmt> Parser::ParseStmt(token_const_iterator& stok) {
        auto btok = stok;

        auto itok = btok;
        if (itok->Is(TokType::KwPrint)) {
            auto r = ParsePrintStmt(itok);
            stok   = itok;
            return r;
        } else if (itok->Is(TokType::Var)) {
            auto r = ParseAssignStmt(itok);
            stok   = itok;
            return r;
        } else {
            // error
            mDiagContainer << make_error_diag(itok->GetSourceRange(), "unknown token");
            return {};
        }
    }

    ParseResult<ast::Stmt> Parser::ParsePrintStmt(token_const_iterator& stok) {
        auto btok = stok;

        auto itok = btok;
        assert(itok->Is(TokType::KwPrint));
        ++itok;

        auto r = ParseVar(itok);
        if (!r) return {};

        if (itok->IsNot(TokType::SemiColon)) {
            mDiagContainer << make_error_diag((itok - 1)->GetSourceRange(), "need a semicolon after this");
            mSuccess = false;

            // ignore ';'
            stok = itok;
            return ParseResult<ast::Stmt>(mStorage.Allocate<ast::PrintStmt>(btok, itok - 1, r));
        }
        auto etok = itok++;
        stok      = itok;

        return ParseResult<ast::Stmt>(mStorage.Allocate<ast::PrintStmt>(btok, etok, r));
    }

    ParseResult<ast::Stmt> Parser::ParseAssignStmt(token_const_iterator& stok) {
        auto btok = stok;

        auto itok = btok;
        assert(itok->Is(TokType::Var));

        auto rLeftVar = ParseLeftVar(itok);
        if (!rLeftVar) return {};

        if (itok->IsNot(TokType::Equal)) {
            mDiagContainer << make_error_diag(itok->GetSourceRange(), "need '=' after variable for assignment");
            mSuccess = false;
            --itok;
        }
        ++itok;

        auto rExpr = ParseExpr(itok);
        if (!rExpr) return {};

        if (itok->IsNot(TokType::SemiColon)) {
            mDiagContainer << make_error_diag((itok - 1)->GetSourceRange(), "need a semicolon after this");
            mSuccess = false;

            // ignore ';'
            stok = itok;
            return ParseResult<ast::Stmt>(mStorage.Allocate<ast::AssignStmt>(btok, itok - 1, rLeftVar, rExpr));
        }
        auto etok = itok++;
        stok      = itok;

        return ParseResult<ast::Stmt>(mStorage.Allocate<ast::AssignStmt>(btok, etok, rLeftVar, rExpr));
    }

    ParseResult<ast::LeftVar> Parser::ParseLeftVar(token_const_iterator& stok) {
        auto btok = stok;
        if (stok->IsNot(TokType::Var)) {
            mDiagContainer << make_error_diag(stok->GetSourceRange(), "need a variable here");
            return {};
        }
        ++stok;
        return ParseResult<ast::LeftVar>(mStorage.Allocate<ast::LeftVar>(btok, stok - 1, btok->GetSourceRange().GetSourceString()));
    }

} // namespace parser


namespace vm {

///
/// SP : stack pointer (like ESP, RSP...)
/// FP : frame pointer (like EBP, RBP...)
/// IP : instruction pointer (like EIP, RIP...)
///

#define OpCodes                                                                                                        \
    /* opcode args */                                                                                                  \
    EnumString(OpPush), EnumString(OpPop), EnumString(OpLoad), /* frame pointer offset */                              \
        EnumString(OpStore),                                   /* frame pointer offset */                              \
        EnumString(OpAdd), EnumString(OpSub), EnumString(OpMul), EnumString(OpDiv), EnumString(OpAdjustSP),            \
        EnumString(OpPrint), /**/

    enum OpCode {
#undef EnumString
#define EnumString(x) x
        OpCodes
    };

    static char const* OpCodeString[] = {
#undef EnumString
#define EnumString(x) #x
        OpCodes};

    char const* to_string(OpCode op) { return OpCodeString[op]; }


    class Program {
    public:
        std::size_t AddOp(OpCode op) {
            mInstructions.push_back(op);
            return mInstructions.size() - 1;
        }

        std::size_t AddOp(OpCode op, int data) {
            mInstructions.push_back(op);
            mInstructions.push_back(data);
            return mInstructions.size() - 2;
        }

        void Change(std::size_t index, int whatever) { mInstructions[index] = whatever; }

        std::vector<int> const& Instructions() const { return mInstructions; }

    private:
        std::vector<int> mInstructions;
    };

    std::ostream& Dump(std::ostream& os, Program const& program, std::size_t indent = 4) {
        for (auto it = program.Instructions().begin(); it != program.Instructions().end(); ++it) {
            std::cout << std::string(indent, ' ') << to_string(static_cast<OpCode>(*it));
            switch (*it) {
            case OpPush:
            case OpLoad:
            case OpStore:
            case OpAdjustSP:
                std::cout << "\t\t" << *(it + 1);
                ++it;
                break;

            default:
                break;
            }
            std::cout << '\n';
        }
        return os;
    }


    class VMachine {
    public:
        explicit VMachine(Program const& program, std::size_t stackSize = 4096)
            : mProgram(program), mStack(stackSize), mSP(0), mFP(0) {}
        void Execute() {
            auto const& instructions = mProgram.Instructions();
            for (auto IP = 0UL; IP < instructions.size(); ++IP) {
                auto op = instructions[IP];
                switch (op) {
                case OpPush:
                    mStack[++mSP] = instructions[++IP];
                    break;

                case OpPop:
                    --mSP;
                    break;

                case OpLoad: {
                    auto val      = mStack[mFP + instructions[++IP]];
                    mStack[++mSP] = val;
                    break;
                }

                case OpStore: {
                    auto val                         = mStack[mSP--];
                    mStack[mFP + instructions[++IP]] = val;
                    break;
                }

                case OpAdd: {
                    auto b      = mStack[mSP--];
                    auto a      = mStack[mSP];
                    mStack[mSP] = a + b;
                    break;
                }

                case OpSub: {
                    auto b      = mStack[mSP--];
                    auto a      = mStack[mSP];
                    mStack[mSP] = a - b;
                    break;
                }

                case OpMul: {
                    auto b      = mStack[mSP--];
                    auto a      = mStack[mSP];
                    mStack[mSP] = a * b;
                    break;
                }

                case OpDiv: {
                    auto b      = mStack[mSP--];
                    auto a      = mStack[mSP];
                    mStack[mSP] = a / b;
                    break;
                }

                case OpAdjustSP: {
                    mSP += instructions[++IP];
                    break;
                }

                case OpPrint: {
                    std::cout << mStack[mSP--] << "\n";
                    break;
                }

                default:
                    std::cerr << "Invalid instruction, stopping now.\n";
                    return;
                }
            }
        }

    private:
        Program const&   mProgram;
        std::vector<int> mStack;
        std::size_t      mSP;
        std::size_t      mFP;
    };

} // namespace vm


class CompilerVisitor : public ast::Visitor {
public:
    explicit CompilerVisitor(DiagnosticContainer& diagContainer, vm::Program& program)
        : mDiagContainer(diagContainer), mProgram(program), mSuccess(true) {}

    virtual ast::VisitorTraverseOrder TraverseOrder() const { return ast::VisitorTraverseOrder::PostOrder; }

    virtual bool Visit(ast::AddExpr const& /*addExpr*/) {
        mProgram.AddOp(vm::OpAdd);
        return true;
    }

    virtual bool Visit(ast::SubExpr const& /*subExpr*/) {
        mProgram.AddOp(vm::OpSub);
        return true;
    }

    virtual bool Visit(ast::MulTerm const& /*mulTerm*/) {
        mProgram.AddOp(vm::OpMul);
        return true;
    }

    virtual bool Visit(ast::DivTerm const& /*divTerm*/) {
        mProgram.AddOp(vm::OpDiv);
        return true;
    }

    virtual bool Visit(ast::Number const& number) {
        mProgram.AddOp(vm::OpPush, number.Value());
        return true;
    }

    virtual bool Visit(ast::Var const& var) {
        auto it = std::find(mVars.begin(), mVars.end(), var.Name());
        if (it == mVars.end()) {
            mDiagContainer << make_error_diag(var.TokenBegin().GetSourceRange(),
                                              "use uninitialized variable '" + var.Name() + "'");
            mSuccess = false;
            return true;
        }

        mProgram.AddOp(vm::OpLoad, std::distance(mVars.begin(), it));
        return true;
    }

    virtual bool Visit(ast::PrintStmt const& printStmt) {
        auto it = std::find(mVars.begin(), mVars.end(), printStmt.Var().Name());
        if (it == mVars.end()) {
//            mDiagContainer << make_error_diag(printStmt.Var().TokenBegin().GetSourceRange(),
//                                              "use uninitialized variable '" + printStmt.Var().Name() + "'");
            mSuccess = false;
            return true;
        }

        mProgram.AddOp(vm::OpLoad, std::distance(mVars.begin(), it));
        mProgram.AddOp(vm::OpPrint);
        return true;
    }

    virtual bool Visit(ast::AssignStmt const& assignStmt) {
        auto it = std::find(mVars.begin(), mVars.end(), assignStmt.LeftVar().Name());
        mProgram.AddOp(vm::OpStore, std::distance(mVars.begin(), it));
        return true;
    }

    virtual bool Visit(ast::LeftVar const& leftVar) {
        auto it = std::find(mVars.begin(), mVars.end(), leftVar.Name());
        if (it == mVars.end()) {
            mVars.push_back(leftVar.Name());
        }
        return true;
    }

    virtual bool Traverse(ast::CompoundStmt const& compoundStmt) {
        auto index = mProgram.AddOp(vm::OpAdjustSP, 0);
        if (ast::Visitor::Traverse(compoundStmt) == false) {
            return false;
        }
        mProgram.Change(index + 1, mVars.size());
        return true;
    }

    bool Success() const { return mSuccess; }

private:
    DiagnosticContainer&     mDiagContainer;
    vm::Program&             mProgram;
    std::vector<std::string> mVars;
    bool                     mSuccess;
};


int main() {
    char const* const expression =
        "   a = (1*(2*3)+4*(5 * 6 + 1));\n"
        "   print a;\n"
        "   b = a - 6;\n"
        "   printc = a;\n"
        "   print printc;"
        "   print b;";

    DiagnosticContainer  diagContainer;
    Source               source(expression);
    lexer::Lexer         l(diagContainer, source);
    token_container_type tokens;
    auto                 lexResult = l.Lex(tokens);
    if (!lexResult) {
        std::cerr << "Lex FAILED.\n";
        std::cerr << diagContainer << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "Lex OK, tokens:\n";
    Dump(std::cout, tokens) << "\n\n";
    parser::Parser p(diagContainer, tokens.begin());
    auto           parseResult = p.Parse();
    if (!parseResult || !p.Success()) {
        std::cerr << "Parse FAILED.\n";
        std::cerr << diagContainer << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "Parse OK.\n";
    vm::Program     program;
    CompilerVisitor compiler(diagContainer, program);
    if (compiler.Traverse(parseResult) == false) {
        std::cerr << "Traverse FAILED.\n";
        std::cerr << diagContainer << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "Traverse OK.\n";
    if (!compiler.Success()) {
        std::cerr << "Compilation FAILED.\n";
        std::cerr << diagContainer << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "Compilation OK, assembly:\n";
    Dump(std::cout, program) << "\n";

    std::cout << "Running result:\n";
    vm::VMachine vmachine(program);
    vmachine.Execute();
}
