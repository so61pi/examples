#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

/**
  <expression> ::= <term> + <expression> | <term> - <expression> | <term>
  <term> ::= <factor> * <term> | <factor> / <term> | <factor>
  <factor> ::= (<expression>) | <number> | <var>

  Expr = AddExpr | SubExpr | Term
  AddExpr = Term + Expr
  SubExpr = Term - Expr
  Term = MulTerm | DivTerm | Factor
  MulTerm = Factor * Term
  DivTerm = Factor / Term
  Factor = ParenExpr | Number | Var
  ParenExpr = (Expr)
*/


///
///
///
enum class TokType {
    EndOfSource,

    Number,
    OpAdd,
    OpSub,
    OpMul,
    OpDiv,
    LParen,
    RParen,
    Var
};

char const* GetName(TokType type) {
    switch (type) {
#define RETNAME(TYPE)                                                          \
    case TokType::TYPE:                                                        \
        return #TYPE
        RETNAME(EndOfSource);
        RETNAME(Number);
        RETNAME(OpAdd);
        RETNAME(OpSub);
        RETNAME(OpMul);
        RETNAME(OpDiv);
        RETNAME(LParen);
        RETNAME(RParen);
        RETNAME(Var);
#undef RETNAME

    default:
        return "Unknown";
    }
}


class Source {
public:
    explicit Source(char const* source) : mSource(source) { mFileName = "---"; }

    std::string const& FileName() const { return mFileName; }

    char const* begin() const { return mSource.c_str(); }
    char const* end() const { return mSource.c_str() + mSource.size(); }

    std::pair<unsigned, unsigned> GetLocation(char const* ptr) const {
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

private:
    std::string mFileName;
    std::string mSource;
};


///
///
///
class SourceLoc {
public:
    explicit SourceLoc(::Source& source, char const* begin, char const* end)
        : mSource(source), mBegin(begin), mEnd(end) {}

    std::pair<unsigned, unsigned> BeginLoc() const {
        return mSource.GetLocation(mBegin);
    }

    std::pair<unsigned, unsigned> EndLoc() const {
        return mSource.GetLocation(mEnd);
    }

    std::string String() const { return std::string(mBegin, mEnd); }

    ::Source& GetSource() const { return mSource; }

    char const* begin() const { return mBegin; }
    char const* end() const { return mEnd; }

private:
    ::Source&   mSource;
    char const* mBegin;
    char const* mEnd;
};

std::ostream& operator<<(std::ostream& os, SourceLoc const& srcLoc) {
    auto beginLoc = srcLoc.BeginLoc();
    os << srcLoc.GetSource().FileName() << ':' << beginLoc.first << ':'
       << beginLoc.second;
    return os;
}


///
///
///
class Token {
public:
    explicit Token(TokType type, SourceLoc loc) : mType(type), mLocation(loc) {}

    TokType   Type() const { return mType; }
    SourceLoc Location() const { return mLocation; }

    bool Is(TokType type) const { return type == mType; }
    bool IsNot(TokType type) const { return !Is(type); }

private:
    TokType   mType;
    SourceLoc mLocation;
};

std::ostream& operator<<(std::ostream& os, Token const& token) {
    auto beginLoc = token.Location().BeginLoc();
    os << '<' << GetName(token.Type()) << ':' << beginLoc.first << ':'
       << beginLoc.second << '>';
    return os;
}


///
///
///
using token_container_type = std::vector<Token>;
using token_const_iterator = token_container_type::const_iterator;


///
///
///
class Storage {
public:
    template <typename T, typename... Args>
    T* Allocate(Args&&... args) {
        mMemory.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
        return reinterpret_cast<T*>(mMemory.back().get());
    }

private:
    std::vector<std::shared_ptr<void>> mMemory;
};


///
///
///
namespace lexer {

    class Diagnostic {
    public:
        explicit Diagnostic(::SourceLoc loc, std::string const& message = "")
            : mSourceLoc(loc), mMessage(message) {}

        ::SourceLoc const& GetSourceLoc() const { return mSourceLoc; }
        std::string const& Message() const { return mMessage; }

    private:
        ::SourceLoc mSourceLoc;
        std::string mMessage;
    };


    class LexResult {
    public:
        LexResult() : mpDiag(nullptr) {}

        explicit LexResult(std::unique_ptr<Diagnostic> pDiag)
            : mpDiag(std::move(pDiag)) {}

        explicit operator bool() const { return mpDiag == nullptr; }
        Diagnostic* Diag() const { return mpDiag.get(); }

    private:
        std::unique_ptr<Diagnostic> mpDiag;
    };


    class Lexer {
    public:
        explicit Lexer(Source& source) : mSource(source) {
            mCurrent = mSource.begin();
        }

        LexResult Lex(token_container_type& tokens) {
            LexResult result;
            while ((result = LexToken(tokens))) {
                if (tokens.back().Is(TokType::EndOfSource)) return LexResult();
            }
            return result;
        }

    private:
        LexResult LexToken(token_container_type& tokens) {
            SkipWhitespace();
            switch (*mCurrent) {
            case '\0':
                tokens.push_back(Token(TokType::EndOfSource,
                                       SourceLoc(mSource, mCurrent, mCurrent)));
                break;

            case '(':
                tokens.push_back(
                    Token(TokType::LParen,
                          SourceLoc(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case ')':
                tokens.push_back(
                    Token(TokType::RParen,
                          SourceLoc(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case '+':
                tokens.push_back(
                    Token(TokType::OpAdd,
                          SourceLoc(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case '-':
                tokens.push_back(
                    Token(TokType::OpSub,
                          SourceLoc(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case '*':
                tokens.push_back(
                    Token(TokType::OpMul,
                          SourceLoc(mSource, mCurrent, mCurrent + 1)));
                ++mCurrent;
                break;

            case '/':
                tokens.push_back(
                    Token(TokType::OpDiv,
                          SourceLoc(mSource, mCurrent, mCurrent + 1)));
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

            default:
                if ('a' <= *mCurrent && *mCurrent <= 'z')
                    return LexVar(tokens);
                else
                    return LexResult(std::unique_ptr<Diagnostic>(
                        new Diagnostic(SourceLoc(mSource, mCurrent, mCurrent),
                                       "unknown character")));
            }
            return LexResult();
        }

        LexResult LexNumber(token_container_type& tokens) {
            auto begin = mCurrent;
            auto end   = mCurrent;
            while ('0' <= *end && *end <= '9')
                ++end;
            mCurrent = end;
            tokens.push_back(
                Token(TokType::Number, SourceLoc(mSource, begin, end)));
            return LexResult();
        }

        LexResult LexVar(token_container_type& tokens) {
            auto begin = mCurrent;
            auto end   = mCurrent;
            while ('a' <= *end && *end <= 'a')
                ++end;
            mCurrent = end;
            tokens.push_back(
                Token(TokType::Var, SourceLoc(mSource, begin, end)));
            return LexResult();
        }


        void SkipWhitespace() {
            while (*mCurrent == ' ' || *mCurrent == '\t')
                ++mCurrent;
        }

    private:
        Source&     mSource;
        char const* mCurrent;
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

    ///
    ///
    ///
    enum class VisitorTraverseOrder { PreOrder, PostOrder };

    class Visitor {
    public:
        virtual ~Visitor();

        virtual VisitorTraverseOrder TraverseOrder() const {
            return VisitorTraverseOrder::PreOrder;
        }

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
    };

    Visitor::~Visitor() = default;


    ///
    /// \brief The ASTNode class
    /// [begin, end]
    ///
    class ASTNode {
    public:
        explicit ASTNode(token_const_iterator begin, token_const_iterator end)
            : mBegin(begin), mEnd(end) {}

        virtual ~ASTNode();

        Token const& TokenBegin() const { return *mBegin; }
        Token const& TokenEnd() const { return *mEnd; }
        std::string  String() const {
            return std::string(mBegin->Location().begin(),
                               mEnd->Location().end());
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

    class Expr : public ASTNode {
    public:
        explicit Expr(token_const_iterator begin, token_const_iterator end,
                      ExprKind kind);
        ExprKind Kind() const;

    private:
        ExprKind mKind;
    };


    class AddExpr : public Expr {
        using base_type = Expr;

    public:
        explicit AddExpr(token_const_iterator begin, token_const_iterator end,
                         ast::Term* lTerm, ast::Expr* rExpr);

        ast::Term*          Left() const;
        ast::Expr*          Right() const;
        ast::Term*          Term() const;
        ast::Expr*          Expr() const;
        virtual const char* NodeName() const { return "AddExpr"; }

    private:
        ast::Term* mLTerm;
        ast::Expr* mRExpr;
    };


    class SubExpr : public Expr {
        using base_type = Expr;

    public:
        explicit SubExpr(token_const_iterator begin, token_const_iterator end,
                         ast::Term* lTerm, ast::Expr* rExpr);

        ast::Term*          Left() const;
        ast::Expr*          Right() const;
        ast::Term*          Term() const;
        ast::Expr*          Expr() const;
        virtual const char* NodeName() const { return "SubExpr"; }

    private:
        ast::Term* mLTerm;
        ast::Expr* mRExpr;
    };


    ///
    ///
    ///
    enum class TermKind { MulTerm, DivTerm, Factor };

    class Term : public Expr {
        using base_type = Expr;

    public:
        explicit Term(token_const_iterator begin, token_const_iterator end,
                      TermKind kind);
        TermKind Kind() const;

    private:
        TermKind mKind;
    };

    class MulTerm : public Term {
        using base_type = Term;

    public:
        explicit MulTerm(token_const_iterator begin, token_const_iterator end,
                         Factor* lFactor, Term* rTerm);

        ast::Factor*        Left() const;
        ast::Term*          Right() const;
        ast::Factor*        Factor() const;
        ast::Term*          Term() const;
        virtual const char* NodeName() const { return "MulTerm"; }

    private:
        ast::Factor* mLFactor;
        ast::Term*   mRTerm;
    };

    class DivTerm : public Term {
        using base_type = Term;

    public:
        explicit DivTerm(token_const_iterator begin, token_const_iterator end,
                         ast::Factor* lFactor, ast::Term* rTerm);

        ast::Factor*        Left() const;
        ast::Term*          Right() const;
        ast::Factor*        Factor() const;
        ast::Term*          Term() const;
        virtual const char* NodeName() const { return "DivTerm"; }

    private:
        ast::Factor* mLFactor;
        ast::Term*   mRTerm;
    };

    ///
    ///
    ///
    enum class FactorKind { ParenExpr, Number, Var };

    class Factor : public Term {
        using base_type = Term;

    public:
        explicit Factor(token_const_iterator begin, token_const_iterator end,
                        FactorKind kind);
        FactorKind Kind() const;

    private:
        FactorKind mKind;
    };

    class Number : public Factor {
        using base_type = Factor;

    public:
        explicit Number(token_const_iterator begin, token_const_iterator end,
                        int value);
        int                 Value() const;
        virtual const char* NodeName() const { return "Number"; }

    private:
        int mValue;
    };

    class Var : public Factor {
        using base_type = Factor;

    public:
        explicit Var(token_const_iterator begin, token_const_iterator end,
                     std::string const& name);
        std::string const&  Name() const;
        virtual const char* NodeName() const { return "Var"; }

    private:
        std::string mName;
    };

    class ParenExpr : public Factor {
        using base_type = Factor;

    public:
        explicit ParenExpr(token_const_iterator begin, token_const_iterator end,
                           ast::Expr* expr);
        ast::Expr*          Expr() const;
        virtual const char* NodeName() const { return "ParenExpr"; }

    private:
        ast::Expr* mExpr;
    };


    ///
    /// Expr
    ///
    Expr::Expr(token_const_iterator begin, token_const_iterator end,
               ExprKind kind)
        : ASTNode(begin, end), mKind(kind) {}
    ExprKind Expr::Kind() const { return mKind; }


    ///
    /// AddExpr
    ///
    AddExpr::AddExpr(token_const_iterator begin, token_const_iterator end,
                     ast::Term* lTerm, ast::Expr* rExpr)
        : ast::Expr(begin, end, ExprKind::AddExpr),
          mLTerm(lTerm),
          mRExpr(rExpr) {}

    ast::Term* AddExpr::Left() const { return Term(); }
    ast::Expr* AddExpr::Right() const { return Expr(); }
    ast::Term* AddExpr::Term() const { return mLTerm; }
    ast::Expr* AddExpr::Expr() const { return mRExpr; }


    ///
    ///
    ///
    SubExpr::SubExpr(token_const_iterator begin, token_const_iterator end,
                     ast::Term* lTerm, ast::Expr* rExpr)
        : ast::Expr(begin, end, ExprKind::SubExpr),
          mLTerm(lTerm),
          mRExpr(rExpr) {}

    ast::Term* SubExpr::Left() const { return Term(); }
    ast::Expr* SubExpr::Right() const { return Expr(); }
    ast::Term* SubExpr::Term() const { return mLTerm; }
    ast::Expr* SubExpr::Expr() const { return mRExpr; }


    ///
    ///
    ///
    Term::Term(token_const_iterator begin, token_const_iterator end,
               TermKind kind)
        : Expr(begin, end, ExprKind::Term), mKind(kind) {}
    TermKind Term::Kind() const { return mKind; }


    ///
    ///
    ///
    MulTerm::MulTerm(token_const_iterator begin, token_const_iterator end,
                     ast::Factor* lFactor, ast::Term* rTerm)
        : ast::Term(begin, end, TermKind::MulTerm),
          mLFactor(lFactor),
          mRTerm(rTerm) {}

    ast::Factor* MulTerm::Left() const { return Factor(); }
    ast::Term*   MulTerm::Right() const { return Term(); }
    ast::Factor* MulTerm::Factor() const { return mLFactor; }
    ast::Term*   MulTerm::Term() const { return mRTerm; }


    ///
    ///
    ///
    DivTerm::DivTerm(token_const_iterator begin, token_const_iterator end,
                     ast::Factor* lFactor, ast::Term* rTerm)
        : ast::Term(begin, end, TermKind::DivTerm),
          mLFactor(lFactor),
          mRTerm(rTerm) {}

    ast::Factor* DivTerm::Left() const { return Factor(); }
    ast::Term*   DivTerm::Right() const { return Term(); }
    ast::Factor* DivTerm::Factor() const { return mLFactor; }
    ast::Term*   DivTerm::Term() const { return mRTerm; }


    ///
    ///
    ///
    Factor::Factor(token_const_iterator begin, token_const_iterator end,
                   FactorKind kind)
        : Term(begin, end, TermKind::Factor), mKind(kind) {}
    FactorKind Factor::Kind() const { return mKind; }


    ///
    ///
    ///
    Number::Number(token_const_iterator begin, token_const_iterator end,
                   int value)
        : Factor(begin, end, FactorKind::Number), mValue(value) {}
    int Number::Value() const { return mValue; }


    ///
    ///
    ///
    Var::Var(token_const_iterator begin, token_const_iterator end,
             std::string const& name)
        : Factor(begin, end, FactorKind::Var), mName(name) {}
    std::string const& Var::Name() const { return mName; }


    ///
    ///
    ///
    ParenExpr::ParenExpr(token_const_iterator begin, token_const_iterator end,
                         ast::Expr* expr)
        : Factor(begin, end, FactorKind::ParenExpr), mExpr(expr) {}

    ast::Expr* ParenExpr::Expr() const { return mExpr; }


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
    bool Visitor::WalkUpFrom(Term const& term) {
        return WalkUpFrom(static_cast<Expr const&>(term)) && Visit(term);
    }
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
    bool Visitor::WalkUpFrom(Var const& var) {
        return WalkUpFrom(static_cast<Factor const&>(var)) && Visit(var);
    }
    bool Visitor::WalkUpFrom(ParenExpr const& parenExpr) {
        return WalkUpFrom(static_cast<Factor const&>(parenExpr))
               && Visit(parenExpr);
    }

    ///
    ///
    ///
    bool Visitor::Traverse(Expr const& expr) {
        // if (!WalkUpFrom(expr)) return false;
        switch (expr.Kind()) {
        case ExprKind::AddExpr:
            return Traverse(*static_cast<AddExpr const*>(&expr));

        case ExprKind::SubExpr:
            return Traverse(*static_cast<SubExpr const*>(&expr));

        case ExprKind::Term:
            return Traverse(*static_cast<Term const*>(&expr));
        }
        return false;
    }
    bool Visitor::Traverse(AddExpr const& addExpr) {
        switch (TraverseOrder()) {
        case VisitorTraverseOrder::PreOrder:
            return WalkUpFrom(addExpr) && Traverse(*addExpr.Left())
                   && Traverse(*addExpr.Right());

        case VisitorTraverseOrder::PostOrder:
            return Traverse(*addExpr.Left()) && Traverse(*addExpr.Right())
                   && WalkUpFrom(addExpr);
        }
        return true;
    }
    bool Visitor::Traverse(SubExpr const& subExpr) {
        switch (TraverseOrder()) {
        case VisitorTraverseOrder::PreOrder:
            return WalkUpFrom(subExpr) && Traverse(*subExpr.Left())
                   && Traverse(*subExpr.Right());

        case VisitorTraverseOrder::PostOrder:
            return Traverse(*subExpr.Left()) && Traverse(*subExpr.Right())
                   && WalkUpFrom(subExpr);
        }
        return true;
    }
    bool Visitor::Traverse(Term const& term) {
        // if (!WalkUpFrom(term)) return false;
        switch (term.Kind()) {
        case TermKind::DivTerm:
            return Traverse(*static_cast<DivTerm const*>(&term));

        case TermKind::Factor:
            return Traverse(*static_cast<Factor const*>(&term));

        case TermKind::MulTerm:
            return Traverse(*static_cast<MulTerm const*>(&term));
        }
        return false;
    }
    bool Visitor::Traverse(MulTerm const& mulTerm) {
        switch (TraverseOrder()) {
        case VisitorTraverseOrder::PreOrder:
            return WalkUpFrom(mulTerm) && Traverse(*mulTerm.Left())
                   && Traverse(*mulTerm.Right());

        case VisitorTraverseOrder::PostOrder:
            return Traverse(*mulTerm.Left()) && Traverse(*mulTerm.Right())
                   && WalkUpFrom(mulTerm);
        }
        return true;
    }
    bool Visitor::Traverse(DivTerm const& divTerm) {
        switch (TraverseOrder()) {
        case VisitorTraverseOrder::PreOrder:
            return WalkUpFrom(divTerm) && Traverse(*divTerm.Left())
                   && Traverse(*divTerm.Right());

        case VisitorTraverseOrder::PostOrder:
            return Traverse(*divTerm.Left()) && Traverse(*divTerm.Right())
                   && WalkUpFrom(divTerm);
        }
        return true;
    }
    bool Visitor::Traverse(Factor const& factor) {
        // if (!WalkUpFrom(factor)) return false;
        switch (factor.Kind()) {
        case FactorKind::Number:
            return Traverse(*static_cast<Number const*>(&factor));

        case FactorKind::ParenExpr:
            return Traverse(*static_cast<ParenExpr const*>(&factor));

        case FactorKind::Var:
            return Traverse(*static_cast<Var const*>(&factor));
        }
        return false;
    }
    bool Visitor::Traverse(Number const& number) { return WalkUpFrom(number); }
    bool Visitor::Traverse(Var const& var) { return WalkUpFrom(var); }
    bool Visitor::Traverse(ParenExpr const& parenExpr) {
        switch (TraverseOrder()) {
        case VisitorTraverseOrder::PreOrder:
            return WalkUpFrom(parenExpr) && Traverse(*parenExpr.Expr());

        case VisitorTraverseOrder::PostOrder:
            return Traverse(*parenExpr.Expr()) && WalkUpFrom(parenExpr);
        }
        return true;
    }

} // namespace ast


namespace parser {

    class Diagnostic {
    public:
        explicit Diagnostic(Token token, std::string const& message)
            : mToken(token), mMessage(message) {}

        ::Token const&     GetToken() const { return mToken; }
        std::string const& Message() const { return mMessage; }

    private:
        ::Token     mToken;
        std::string mMessage;
    };


    template <typename T>
    class ParseResult {
    public:
        ParseResult() : mPtr(nullptr) {}

        template <typename U>
        explicit ParseResult(U* ptr) : mPtr(ptr) {}

        template <typename U>
        ParseResult& operator=(ParseResult<U> const& rhs) {
            mPtr = rhs.get();
            return *this;
        }

        explicit operator bool() const { return mPtr != nullptr; }
        T* get() const { return mPtr; }

    private:
        T* mPtr;
    };


    class Parser {
    public:
        explicit Parser(token_const_iterator tok) : mTok(tok) {}

        ParseResult<ast::Expr> Parse() { return ParseExpr(mTok); }

        Diagnostic* Diag() const { return mpDiag.get(); }

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

    private:
        Storage                     mStorage;
        token_const_iterator        mTok;
        std::unique_ptr<Diagnostic> mpDiag;
    };


    ParseResult<ast::Expr> Parser::ParseExpr(token_const_iterator& stok) {
        ParseResult<ast::Expr> r;
        r = ParseAddExpr(stok);
        if (r) return r;

        r = ParseSubExpr(stok);
        if (r) return r;

        r = ParseTerm(stok);
        if (r) return r;

        mpDiag = std::unique_ptr<Diagnostic>(
            new Diagnostic(*stok, "cannot parse Expr"));
        return {};
    }


    ParseResult<ast::Expr> Parser::ParseAddExpr(token_const_iterator& stok) {
        auto btok = stok;

        auto                   itok = btok;
        ParseResult<ast::Term> term = ParseTerm(itok);
        if (!term) return {};

        if (itok->IsNot(TokType::OpAdd)) {
            mpDiag = std::unique_ptr<Diagnostic>(
                new Diagnostic(*itok, "AddExpr needs '+'"));
            return {};
        }
        ++itok;

        auto expr = ParseExpr(itok);
        if (!expr) return {};

        stok      = itok;
        auto etok = stok - 1;

        return ParseResult<ast::Expr>(
            mStorage.Allocate<ast::AddExpr>(btok, etok, term.get(),
                                            expr.get()));
    }


    ParseResult<ast::Expr> Parser::ParseSubExpr(token_const_iterator& stok) {
        auto btok = stok;

        auto itok = btok;
        auto term = ParseTerm(itok);
        if (!term) return {};

        if (itok->IsNot(TokType::OpSub)) {
            mpDiag = std::unique_ptr<Diagnostic>(
                new Diagnostic(*itok, "AddExpr needs '-'"));
            return {};
        }
        ++itok;

        auto expr = ParseExpr(itok);
        if (!expr) return {};

        stok      = itok;
        auto etok = stok - 1;

        return ParseResult<ast::Expr>(
            mStorage.Allocate<ast::SubExpr>(btok, etok, term.get(),
                                            expr.get()));
    }


    ParseResult<ast::Term> Parser::ParseTerm(token_const_iterator& stok) {
        ParseResult<ast::Term> r;
        r = ParseMulTerm(stok);
        if (r) return r;

        r = ParseDivTerm(stok);
        if (r) return r;

        r = ParseFactor(stok);
        if (r) return r;

        mpDiag = std::unique_ptr<Diagnostic>(
            new Diagnostic(*stok, "cannot parse Term"));
        return {};
    }


    ParseResult<ast::Term> Parser::ParseMulTerm(token_const_iterator& stok) {
        auto btok = stok;

        auto itok   = btok;
        auto factor = ParseFactor(itok);
        if (!factor) return {};

        if (itok->IsNot(TokType::OpMul)) {
            mpDiag = std::unique_ptr<Diagnostic>(
                new Diagnostic(*itok, "MulTerm needs '*'"));
            return {};
        }
        ++itok;

        auto term = ParseTerm(itok);
        if (!term) return {};

        stok      = itok;
        auto etok = stok - 1;

        return ParseResult<ast::Term>(
            mStorage.Allocate<ast::MulTerm>(btok, etok, factor.get(),
                                            term.get()));
    }


    ParseResult<ast::Term> Parser::ParseDivTerm(token_const_iterator& stok) {
        auto btok = stok;

        auto itok   = btok;
        auto factor = ParseFactor(itok);
        if (!factor) return {};

        if (itok->IsNot(TokType::OpDiv)) {
            mpDiag = std::unique_ptr<Diagnostic>(
                new Diagnostic(*itok, "DivTerm needs '*'"));
            return {};
        }
        ++itok;

        auto term = ParseTerm(itok);
        if (!term) return {};

        stok      = itok;
        auto etok = stok - 1;

        return ParseResult<ast::Term>(
            mStorage.Allocate<ast::DivTerm>(btok, etok, factor.get(),
                                            term.get()));
    }


    ParseResult<ast::Factor> Parser::ParseFactor(token_const_iterator& stok) {
        ParseResult<ast::Factor> r;
        r = ParseParenExpr(stok);
        if (r) return r;

        r = ParseNumber(stok);
        if (r) return r;

        r = ParseVar(stok);
        if (r) return r;

        mpDiag = std::unique_ptr<Diagnostic>(
            new Diagnostic(*stok, "cannot parse Factor"));
        return {};
    }


    ParseResult<ast::Number> Parser::ParseNumber(token_const_iterator& stok) {
        auto btok = stok;
        if (stok->IsNot(TokType::Number)) {
            mpDiag = std::unique_ptr<Diagnostic>(
                new Diagnostic(*stok, "cannot parse Number"));
            return {};
        }

        auto num = std::stoi(stok->Location().String());
        ++stok;

        return ParseResult<ast::Number>(
            mStorage.Allocate<ast::Number>(btok, stok - 1, num));
    }


    ParseResult<ast::Var> Parser::ParseVar(token_const_iterator& stok) {
        auto btok = stok;
        if (stok->IsNot(TokType::Var)) {
            mpDiag = std::unique_ptr<Diagnostic>(
                new Diagnostic(*stok, "cannot parse Var"));
            return {};
        }
        ++stok;
        return ParseResult<ast::Var>(
            mStorage.Allocate<ast::Var>(btok, stok - 1,
                                        btok->Location().String()));
    }


    ParseResult<ast::ParenExpr> Parser::ParseParenExpr(
        token_const_iterator& stok) {
        auto btok = stok;

        auto itok  = btok;
        auto saved = itok;
        if (itok->IsNot(TokType::LParen)) {
            mpDiag = std::unique_ptr<Diagnostic>(
                new Diagnostic(*saved, "missing '(' for ParenExpr"));
            return {};
        }
        ++itok;

        saved     = itok;
        auto expr = ParseExpr(itok);
        if (!expr) return {};

        saved = itok;
        if (itok->IsNot(TokType::RParen)) {
            mpDiag = std::unique_ptr<Diagnostic>(
                new Diagnostic(*saved, "missing ')' for ParenExpr"));
            return {};
        }
        ++itok;

        stok      = itok;
        auto etok = stok - 1;

        return ParseResult<ast::ParenExpr>(
            mStorage.Allocate<ast::ParenExpr>(btok, etok, expr.get()));
    }

} // namespace parser


class MyVisitor : public ast::Visitor {
public:
    virtual ast::VisitorTraverseOrder TraverseOrder() const {
        return ast::VisitorTraverseOrder::PostOrder;
    }

    void PrintInfo(char const* func, ast::ASTNode const& node) {
        std::cout << func << "\n"
                  << "--- stack size : " << mStack.size() << "\n"
                  << "--- expression : " << node.String() << "\n\n";
    }

    virtual bool Visit(ast::AddExpr const& addExpr) {
        PrintInfo(__PRETTY_FUNCTION__, addExpr);

        if (mStack.size() < 2) return false;
        auto eit = mStack.end();
        auto r   = *(eit - 2) + *(eit - 1);
        mStack.pop_back();
        mStack.pop_back();
        mStack.push_back(r);
        return true;
    }

    virtual bool Visit(ast::SubExpr const& subExpr) {
        PrintInfo(__PRETTY_FUNCTION__, subExpr);

        if (mStack.size() < 2) return false;
        auto eit = mStack.end();
        auto r   = *(eit - 2) - *(eit - 1);
        mStack.pop_back();
        mStack.pop_back();
        mStack.push_back(r);
        return true;
    }

    virtual bool Visit(ast::MulTerm const& mulTerm) {
        PrintInfo(__PRETTY_FUNCTION__, mulTerm);

        if (mStack.size() < 2) return false;
        auto eit = mStack.end();
        auto r   = *(eit - 2) * *(eit - 1);
        mStack.pop_back();
        mStack.pop_back();
        mStack.push_back(r);
        return true;
    }

    virtual bool Visit(ast::DivTerm const& divTerm) {
        PrintInfo(__PRETTY_FUNCTION__, divTerm);

        if (mStack.size() < 2) return false;
        auto eit = mStack.end();
        auto r   = *(eit - 2) / *(eit - 1);
        mStack.pop_back();
        mStack.pop_back();
        mStack.push_back(r);
        return true;
    }

    virtual bool Visit(ast::Number const& number) {
        PrintInfo(__PRETTY_FUNCTION__, number);

        mStack.push_back(number.Value());
        return true;
    }

    int Result() const {
        if (mStack.size() != 1) {
            std::cout << "Eh!\n";
            return -1;
        }
        return mStack.back();
    }

private:
    std::vector<int> mStack;
};


int main() {
    char const* const expression = "(1*(2*3)-4*(5 * 6))";

    Source               source(expression);
    lexer::Lexer         l(source);
    token_container_type tokens;
    auto                 lexResult = l.Lex(tokens);
    if (!lexResult) {
        std::cout << lexResult.Diag()->GetSourceLoc() << ' '
                  << lexResult.Diag()->Message() << std::endl;
        return EXIT_FAILURE;
    }

    parser::Parser p(tokens.begin());
    auto           parseResult = p.Parse();
    if (!parseResult) {
        std::cout << p.Diag()->GetToken().Location() << ' '
                  << p.Diag()->Message() << std::endl;
        return EXIT_FAILURE;
    } else {
        std::cout << "Parse OK.\n";
        MyVisitor myVisitor;
        myVisitor.Traverse(*parseResult.get());
        std::cout << myVisitor.Result() << "\n";
    }
}
