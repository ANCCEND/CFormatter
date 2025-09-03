#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>
#include "ASTNodes.hpp"

using namespace std;

enum class ASTNodeType
{
    Program,      // 程序
    ExtVarDecl,   // 外部变量定义
    TypeSpec,     // 类型说明符
    FunctionDef,  // 函数定义
    VarDeclList,  // 变量声明列表
    VarDecl,      // 变量声明
    CompoundStmt, // 语句块 { ... }
    IfStmt,       // if 语句
    THenStmt,     // then 语句
    ElseStmt,     // else 语句
    WhileStmt,    // while 语句
    ReturnStmt,   // return 语句
    SwitchStmt,   // switch 语句
    SwitchCase,   // case 语句
    DefaultCase,  // default 语句
    ForStmt,      // for 语句
    BreakStmt,    // break 语句
    ContinueStmt, // continue 语句
    StuctStmt,    // struct 语句
    Statement,    // 语句
    BinaryExpr,   // 二元表达式
    UnaryExpr,    // 一元表达式
    Literal,      // 常量
    Identifier,   // 标识符
    CallExpr      // 函数调用
};

class ASTNode
{
public:
    ASTNodeType type;
    ASTNode(ASTNodeType t) : type(t) {}
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
};

class Preprocessor : public ASTNode
{
public:
    string directive;
    Preprocessor(const string &dir) : ASTNode(ASTNodeType::Preprocessor), directive(dir) {}
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "Preprocessor: " << directive << "\n";
    }
};

class programNode : public ASTNode
{
public:
    vector<ASTNode *> extdeflists;
    programNode() : ASTNode(ASTNodeType::Program) {}
    ~programNode()
    {
        for (auto def : extdeflists)
        {
            if (def)
            {
                delete def;
            }
        }
        // extdeflists.clear();
    }
    void addExtdef(ASTNode *def)
    {
        if (def)
        {
            extdeflists.push_back(def);
        }
        else
        {
            cout << "Warning: Attempted to add null ExtDef to programNode\n";
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "Program\n";
        for (auto def : extdeflists)
        {
            if (def)
            {
                def->print(indent + 2);
            }
            else
            {
                cout << string(indent + 2, ' ') << "Error: Null ExtDef\n";
            }
        }
    }
};

class TypeSpec : public ASTNode
{
public:
    vector<string> typeName; // 类型名称，例如 "int", "float"
    TypeSpec(const vector<string> &names)
        : ASTNode(ASTNodeType::TypeSpec), typeName(names) {}
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "TypeSpec: ";
        for (const auto &name : typeName)
        {
            cout << name << " ";
        }
        cout << "\n";
    }
};

class extVarDecl : public ASTNode
{
public:
    TypeSpec *typeName;      // 变量类型，例如 "int"
    vector<string> varNames; // 变量名列表
    extVarDecl(TypeSpec *type, const vector<string> &names)
        : ASTNode(ASTNodeType::ExtVarDecl), typeName(type), varNames(names) {}
    ~extVarDecl()
    {
        if (typeName)
        {
            delete typeName;
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "ExtVarDecl:\n";
        if (typeName)
        {
            typeName->print(indent + 2);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null TypeSpec\n";
        }
        for (const auto &name : varNames)
        {
            if (!name.empty())
            {
                cout << string(indent + 2, ' ') << "Var: " << name << "\n";
            }
            else
            {
                cout << string(indent + 2, ' ') << "Error: Empty Variable Name\n";
            }
        }
    }
};

class functionDef : public ASTNode
{
public:
    TypeSpec *returnType;                        // 返回类型
    string functionName;                         // 函数名
    vector<pair<TypeSpec *, string>> parameters; // 参数列表，包含类型和名称
    ASTNode *body;                               // 函数体

    functionDef(TypeSpec *retType, const string &funcName,
                const vector<pair<TypeSpec *, string>> &params, ASTNode *bdy)
        : ASTNode(ASTNodeType::FunctionDef), returnType(retType), functionName(funcName), parameters(params), body(bdy) {}
    ~functionDef()
    {
        if (returnType)
        {
            delete returnType;
        }
        for (auto &param : parameters)
        {
            if (param.first)
            {
                delete param.first;
            }
        }
        if (body)
        {
            delete body;
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "FunctionDef: " << functionName << "\n";
        if (returnType)
        {
            cout << string(indent + 2, ' ') << "Return Type:\n";
            returnType->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Return Type\n";
        }
        cout << string(indent + 2, ' ') << "Parameters:\n";
        for (const auto &param : parameters)
        {
            if (param.first)
            {
                param.first->print(indent + 4);
                cout << string(indent + 4, ' ') << "Param Name: " << param.second << "\n";
            }
            else
            {
                cout << string(indent + 4, ' ') << "Error: Null Parameter Type\n";
            }
        }
        if (body)
        {
            cout << string(indent + 2, ' ') << "Body:\n";
            body->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Body\n";
        }
    }
};

class VarDecl : public ASTNode
{
public:
    TypeSpec *typeName; // 变量类型
    string varName;     // 变量名
    VarDecl(TypeSpec *type, const string &name)
        : ASTNode(ASTNodeType::VarDecl), typeName(type), varName(name) {}
    ~VarDecl()
    {
        if (typeName)
        {
            delete typeName;
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "VarDecl: " << varName << "\n";
        if (typeName)
        {
            typeName->print(indent + 2);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null TypeSpec\n";
        }
    }
};

class VarDeclList : public ASTNode
{
public:
    VarDecl *varDecl = nullptr;
    VarDeclList(VarDecl *decl)
        : ASTNode(ASTNodeType::VarDeclList), varDecl(decl) {}
    ~VarDeclList()
    {
        if (varDecl)
        {
            delete varDecl;
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "VarDeclList:\n";
        if (varDecl)
        {
            varDecl->print(indent + 2);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null VarDecl\n";
        }
    }
};

class CompoundStmt : public ASTNode
{
public:
    vector<ASTNode *> statements; // 语句列表
    CompoundStmt() : ASTNode(ASTNodeType::CompoundStmt) {}
    ~CompoundStmt()
    {
        for (auto stmt : statements)
        {
            if (stmt)
            {
                delete stmt;
            }
        }
    }
    void addStatement(ASTNode *stmt)
    {
        if (stmt)
        {
            statements.push_back(stmt);
        }
        else
        {
            cout << "Warning: Attempted to add null statement to CompoundStmt\n";
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "CompoundStmt:\n";
        for (auto stmt : statements)
        {
            if (stmt)
            {
                stmt->print(indent + 2);
            }
            else
            {
                cout << string(indent + 2, ' ') << "Error: Null Statement\n";
            }
        }
    }
};

class IfStmt : public ASTNode
{
public:
    ASTNode *condition;
    ASTNode *thenBranch;
    ASTNode *elseBranch;
    IfStmt(ASTNode *cond, ASTNode *thenB, ASTNode *elseB) : ASTNode(ASTNodeType::IfStmt), thenBranch(thenB), elseBranch(elseB) {}
    ~IfStmt()
    {
        if (condition)
            delete condition;
        if (thenBranch)
            delete thenBranch;
        if (elseBranch)
            delete elseBranch;
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "IfStmt:\n";
        if (condition)
        {
            cout << string(indent + 2, ' ') << "Condition:\n";
            condition->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Condition\n";
        }
        if (thenBranch)
        {
            cout << string(indent + 2, ' ') << "Then Branch:\n";
            thenBranch->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Then Branch\n";
        }
        if (elseBranch)
        {
            cout << string(indent + 2, ' ') << "Else Branch:\n";
            elseBranch->print(indent + 4);
        }
    }
};

class ThenStmt : public ASTNode
{
public:
    ASTNode *ThenBranch;
    ThenStmt(ASTNode *thenB) : ASTNode(ASTNodeType::THenStmt), ThenBranch(thenB) {}
    ~ThenStmt()
    {
        if (ThenBranch)
            delete ThenBranch;
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "ThenStmt:\n";
        if (ThenBranch)
        {
            ThenBranch->print(indent + 2);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Then Branch\n";
        }
    }
};

class ElseStmt : public ASTNode
{
public:
    ASTNode *ElseBranch;
    ElseStmt(ASTNode *elseB) : ASTNode(ASTNodeType::ElseStmt), ElseBranch(elseB) {}
    ~ElseStmt()
    {
        if (ElseBranch)
            delete ElseBranch;
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "ElseStmt:\n";
        if (ElseBranch)
        {
            ElseBranch->print(indent + 2);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Else Branch\n";
        }
    }
};

class WhileStmt : public ASTNode
{
public:
    ASTNode *condition;
    ASTNode *body;
    WhileStmt(ASTNode *cond, ASTNode *bdy) : ASTNode(ASTNodeType::WhileStmt), condition(cond), body(bdy) {}
    ~WhileStmt()
    {
        if (condition)
            delete condition;
        if (body)
            delete body;
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "WhileStmt:\n";
        if (condition)
        {
            cout << string(indent + 2, ' ') << "Condition:\n";
            condition->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Condition\n";
        }
        if (body)
        {
            cout << string(indent + 2, ' ') << "Body:\n";
            body->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Body\n";
        }
    }
};

class ReturnStmt : public ASTNode
{
public:
    ASTNode *expression;
    ReturnStmt(ASTNode *expr) : ASTNode(ASTNodeType::ReturnStmt), expression(expr) {}
    ~ReturnStmt()
    {
        if (expression)
            delete expression;
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "ReturnStmt:\n";
        if (expression)
        {
            cout << string(indent + 2, ' ') << "Expression:\n";
            expression->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Expression\n";
        }
    }
};

class SwitchStmt : public ASTNode
{
public:
    ASTNode *expression;
    vector<ASTNode *> cases;
    ASTNode *defaultCase;
    SwitchStmt(ASTNode *expr, const vector<ASTNode *> &caseList, ASTNode *defCase)
        : ASTNode(ASTNodeType::SwitchStmt), expression(expr), cases(caseList), defaultCase(defCase) {}
    ~SwitchStmt()
    {
        if (expression)
            delete expression;
        for (auto caseStmt : cases)
        {
            if (caseStmt)
                delete caseStmt;
        }
        if (defaultCase)
            delete defaultCase;
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "SwitchStmt:\n";
        if (expression)
        {
            cout << string(indent + 2, ' ') << "Expression:\n";
            expression->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Expression\n";
        }
        cout << string(indent + 2, ' ') << "Cases:\n";
        for (const auto &caseStmt : cases)
        {
            if (caseStmt)
            {
                caseStmt->print(indent + 4);
            }
            else
            {
                cout << string(indent + 4, ' ') << "Error: Null Case Statement\n";
            }
        }
        if (defaultCase)
        {
            cout << string(indent + 2, ' ') << "Default Case:\n";
            defaultCase->print(indent + 4);
        }
    }
};

class SwitchCase : public ASTNode
{
public:
    ASTNode *caseValue;
    vector<ASTNode *> statements;
    SwitchCase(ASTNode *value, const vector<ASTNode *> &stmts)
        : ASTNode(ASTNodeType::SwitchCase), caseValue(value), statements(stmts) {}
    ~SwitchCase()
    {
        if (caseValue)
            delete caseValue;
        for (auto stmt : statements)
        {
            if (stmt)
                delete stmt;
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "SwitchCase:\n";
        if (caseValue)
        {
            cout << string(indent + 2, ' ') << "Case Value:\n";
            caseValue->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Case Value\n";
        }
        cout << string(indent + 2, ' ') << "Statements:\n";
        for (const auto &stmt : statements)
        {
            if (stmt)
            {
                stmt->print(indent + 4);
            }
            else
            {
                cout << string(indent + 4, ' ') << "Error: Null Statement\n";
            }
        }
    }
};

class DefaultCase : public ASTNode
{
public:
    vector<ASTNode *> statements;
    DefaultCase(const vector<ASTNode *> &stmts)
        : ASTNode(ASTNodeType::DefaultCase), statements(stmts) {}
    ~DefaultCase()
    {
        for (auto stmt : statements)
        {
            if (stmt)
                delete stmt;
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "DefaultCase:\n";
        cout << string(indent + 2, ' ') << "Statements:\n";
        for (const auto &stmt : statements)
        {
            if (stmt)
            {
                stmt->print(indent + 4);
            }
            else
            {
                cout << string(indent + 4, ' ') << "Error: Null Statement\n";
            }
        }
    }
};

class ForStmt : public ASTNode
{
public:
    ASTNode *init;
    ASTNode *condition;
    ASTNode *increment;
    ASTNode *body;
    ForStmt(ASTNode *ini, ASTNode *cond, ASTNode *inc, ASTNode *bdy) : ASTNode(ASTNodeType::ForStmt), init(ini), condition(cond), increment(inc), body(bdy) {}
    ~ForStmt()
    {
        if (init)
            delete init;
        if (condition)
            delete condition;
        if (increment)
            delete increment;
        if (body)
            delete body;
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "ForStmt:\n";
        if (init)
        {
            cout << string(indent + 2, ' ') << "Initialization:\n";
            init->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Initialization\n";
        }
        if (condition)
        {
            cout << string(indent + 2, ' ') << "Condition:\n";
            condition->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Condition\n";
        }
        if (increment)
        {
            cout << string(indent + 2, ' ') << "Increment:\n";
            increment->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Increment\n";
        }
        if (body)
        {
            cout << string(indent + 2, ' ') << "Body:\n";
            body->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Body\n";
        }
    }
};

class BreakStmt : public ASTNode
{
public:
    BreakStmt() : ASTNode(ASTNodeType::BreakStmt) {}
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "BreakStmt\n";
    }
};

class ContinueStmt : public ASTNode
{
public:
    ContinueStmt() : ASTNode(ASTNodeType::ContinueStmt) {}
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "ContinueStmt\n";
    }
};

class Statement : public ASTNode
{
public:
    ASTNode *stmt;
    Statement(ASTNode *s) : ASTNode(ASTNodeType::Statement), stmt(s) {}
    ~Statement()
    {
        if (stmt)
            delete stmt;
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "Statement:\n";
        if (stmt)
        {
            stmt->print(indent + 2);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Statement\n";
        }
    }
};

class BinaryExpr : public ASTNode
{
public:
    ASTNode *left;
    ASTNode *right;
    string op;
    BinaryExpr(ASTNode *l, ASTNode *r, const string &o)
        : ASTNode(ASTNodeType::BinaryExpr), left(l), right(r), op(o) {}
    ~BinaryExpr()
    {
        if (left)
            delete left;
        if (right)
            delete right;
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "BinaryExpr: " << op << "\n";
        if (left)
        {
            cout << string(indent + 2, ' ') << "Left:\n";
            left->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Left Operand\n";
        }
        if (right)
        {
            cout << string(indent + 2, ' ') << "Right:\n";
            right->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Right Operand\n";
        }
    }
};

class UnaryExpr : public ASTNode
{
public:
    ASTNode *operand;
    string op;
    UnaryExpr(ASTNode *opr, const string &o)
        : ASTNode(ASTNodeType::UnaryExpr), operand(opr), op(o) {}
    ~UnaryExpr()
    {
        if (operand)
            delete operand;
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "UnaryExpr: " << op << "\n";
        if (operand)
        {
            cout << string(indent + 2, ' ') << "Operand:\n";
            operand->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Operand\n";
        }
    }
};

class Literal : public ASTNode
{
public:
    string value;
    Literal(const string &val)
        : ASTNode(ASTNodeType::Literal), value(val) {}
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "Literal: " << value << "\n";
    }
};

class Identifier : public ASTNode
{
public:
    string name;
    Identifier(const string &n)
        : ASTNode(ASTNodeType::Identifier), name(n) {}
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "Identifier: " << name << "\n";
    }
};

class CallExpr : public ASTNode
{
public:
    string functionName;
    vector<ASTNode *> arguments;
    CallExpr(const string &fname, const vector<ASTNode *> &args)
        : ASTNode(ASTNodeType::CallExpr), functionName(fname), arguments(args) {}
    ~CallExpr()
    {
        for (auto arg : arguments)
        {
            if (arg)
                delete arg;
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "CallExpr: " << functionName << "\n";
        cout << string(indent + 2, ' ') << "Arguments:\n";
        for (const auto &arg : arguments)
        {
            if (arg)
            {
                arg->print(indent + 4);
            }
            else
            {
                cout << string(indent + 4, ' ') << "Error: Null Argument\n";
            }
        }
    }
};

enum class TokenType
{
    ERROR,
    IDENTIFIER,

    VOID,
    CHAR,
    CHAR_CONST,
    SHORT,
    UNSIGNED,
    SIGNED,
    INT,
    INT_CONST,
    LONG,
    FLOAT,
    FLOAT_CONST,
    DOUBLE,

    IF,
    ELSE,
    SWITCH,
    WHILE,
    DO,
    FOR,
    BREAK,
    CONTINUE,
    RETURN,
    SIZEOF,
    TYPEDEF,
    DEFAULT,
    CASE,
    STATIC,
    EXTERN,
    REGISTER,
    CONST,
    STRUCT,
    UNION,
    ENUM,

    INCLUDE,
    DEFINE,
    UNDEF,

    ADD,
    DIV,
    MOD,
    MUL,
    ASSIGN,
    SUB,
    ADD_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN,
    MUL_ASSIGN,
    SUB_ASSIGN,
    LEFT_SHIFT,
    RIGHT_SHIFT, // <<, >>
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR, // &, |, ^
    BITWISE_NOT, // ~
    INCREMENT,
    DECREMENT, // ++, --
    ARROW,     // ->
    AND,
    OR,
    NOT, // &&, ||, !
    LEFT_SHIFT_ASSIGN,
    RIGHT_SHIFT_ASSIGN,
    AND_ASSIGN,
    OR_ASSIGN,
    NOT_ASSIGN,
    BITWISE_AND_ASSIGN,
    BITWISE_OR_ASSIGN,
    BITWISE_XOR_ASSIGN, // <<=, >>=, &=, |=, ^=
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    LESS_EQUAL,
    GREATER_EQUAL, // ==, !=, <, >, <=, >=

    STRING,
    LPAREN,   // (
    RPAREN,   // )
    LBRACE,   // {
    RBRACE,   // }
    LBRACKET, // [
    RBRACKET, // ]
    DOT,
    COMMA,
    SEMI,      // ;
    HASHTAG,   // #
    COLON,     // :
    QUESTMARK, // ?

    SIGNAL_COMMENT, // //
    BLOCK_COMMENT,  // /* */
    END_OF_FILE,
};

struct Token
{
    TokenType type;
    string lexeme;
    int line;
    int column;

    Token(TokenType t, const std::string &l, int ln, int col)
        : type(t), lexeme(l), line(ln), column(col) {}
};

static const unordered_map<string, TokenType> keywordMap = {
    {"int", TokenType::INT},
    {"long", TokenType::LONG},
    {"char", TokenType::CHAR},
    {"short", TokenType::SHORT},
    {"float", TokenType::FLOAT},
    {"double", TokenType::DOUBLE},
    {"void", TokenType::VOID},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"switch", TokenType::SWITCH},
    {"case", TokenType::CASE},
    {"default", TokenType::DEFAULT},
    {"while", TokenType::WHILE},
    {"do", TokenType::DO},
    {"for", TokenType::FOR},
    {"break", TokenType::BREAK},
    {"continue", TokenType::CONTINUE},
    {"return", TokenType::RETURN},
    {"sizeof", TokenType::SIZEOF},
    {"typedef", TokenType::TYPEDEF},
    {"static", TokenType::STATIC},
    {"extern", TokenType::EXTERN},
    {"register", TokenType::REGISTER},
    {"const", TokenType::CONST},
    {"struct", TokenType::STRUCT},
    {"union", TokenType::UNION},
    {"enum", TokenType::ENUM},
    {"unsigned", TokenType::UNSIGNED},
    {"signed", TokenType::SIGNED},
    {"include", TokenType::INCLUDE},
    {"define", TokenType::DEFINE},
    {"undef", TokenType::UNDEF},
};

const vector<string> operators = {
    "+",
    "-",
    "*",
    "/",
    "%",
    "++",
    "--",
    "=",
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
    "<",
    ">",
    "=",
    "<=",
    ">=",
    "==",
    "!=",
    "&&",
    "||",
    "!",
    "&",
    "|",
    "~",
    "<<",
    ">>",
    "->",
};

inline std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::MUL:
        return "MUL";
    case TokenType::DIV:
        return "DIV";
    case TokenType::LPAREN:
        return "LPAREN";
    case TokenType::RPAREN:
        return "RPAREN";
    case TokenType::SEMI:
        return "SEMI";
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::INT:
        return "INT";
    case TokenType::FLOAT:
        return "FLOAT";
    case TokenType::STRING:
        return "STRING";
    case TokenType::END_OF_FILE:
        return "END_OF_FILE";
    case TokenType::ERROR:
        return "ERROR";
    case TokenType::HASHTAG:
        return "HASHTAG";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::COLON:
        return "COLON";
    case TokenType::LBRACE:
        return "LBRACE";
    case TokenType::RBRACE:
        return "RBRACE";
    case TokenType::LBRACKET:
        return "LBRACKET";
    case TokenType::RBRACKET:
        return "RBRACKET";
    case TokenType::CHAR:
        return "CHAR";
    case TokenType::CHAR_CONST:
        return "CHAR_CONST";
    case TokenType::SHORT:
        return "SHORT";
    case TokenType::UNSIGNED:
        return "UNSIGNED";
    case TokenType::SIGNED:
        return "SIGNED";
    case TokenType::LONG:
        return "LONG";
    case TokenType::DOUBLE:
        return "DOUBLE";
    case TokenType::VOID:
        return "VOID";
    case TokenType::INT_CONST:
        return "INT_CONST";
    case TokenType::FLOAT_CONST:
        return "FLOAT_CONST";
    case TokenType::IF:
        return "IF";
    case TokenType::ELSE:
        return "ELSE";
    case TokenType::SWITCH:
        return "SWITCH";
    case TokenType::WHILE:
        return "WHILE";
    case TokenType::DO:
        return "DO";
    case TokenType::FOR:
        return "FOR";
    case TokenType::BREAK:
        return "BREAK";
    case TokenType::CONTINUE:
        return "CONTINUE";
    case TokenType::RETURN:
        return "RETURN";
    case TokenType::SIZEOF:
        return "SIZEOF";
    case TokenType::TYPEDEF:
        return "TYPEDEF";
    case TokenType::DEFAULT:
        return "DEFAULT";
    case TokenType::CASE:
        return "CASE";
    case TokenType::STATIC:
        return "STATIC";
    case TokenType::EXTERN:
        return "EXTERN";
    case TokenType::REGISTER:
        return "REGISTER";
    case TokenType::CONST:
        return "CONST";
    case TokenType::STRUCT:
        return "STRUCT";
    case TokenType::UNION:
        return "UNION";
    case TokenType::ENUM:
        return "ENUM";
    case TokenType::INCLUDE:
        return "INCLUDE";
    case TokenType::DEFINE:
        return "DEFINE";
    case TokenType::UNDEF:
        return "UNDEF";
    case TokenType::ADD:
        return "ADD";
    case TokenType::SUB:
        return "SUB";
    case TokenType::ASSIGN:
        return "ASSIGN";
    case TokenType::ADD_ASSIGN:
        return "ADD_ASSIGN";
    case TokenType::SUB_ASSIGN:
        return "SUB_ASSIGN";
    case TokenType::MUL_ASSIGN:
        return "MUL_ASSIGN";
    case TokenType::DIV_ASSIGN:
        return "DIV_ASSIGN";
    case TokenType::MOD_ASSIGN:
        return "MOD_ASSIGN";
    case TokenType::LEFT_SHIFT:
        return "LEFT_SHIFT";
    case TokenType::RIGHT_SHIFT:
        return "RIGHT_SHIFT";
    case TokenType::BITWISE_AND:
        return "BITWISE_AND";
    case TokenType::BITWISE_OR:
        return "BITWISE_OR";
    default:
        return "UNKNOWN";
    }
}

class Lexer
{
private:
    istream &input;
    int line = 1;
    int column = 0;
    char ch = ' ';

public:
    Lexer(istream &in) : input(in)
    {
        if (!input.eof())
        {
            next();
        }
        else
        {
            ch = EOF;
        }
    }
    void next()
    {
        ch = input.get();
        if (ch == '\n')
        {
            line++;
            column = 0;
        }
        else
        {
            column++;
        }
    }

    void skipSpace()
    {
        while (isspace(ch))
            next();
    }

    Token gettoken()
    {
        skipSpace();
        if (ch == EOF)
            return Token(TokenType::END_OF_FILE, "", line, column);
        string lexeme;
        int tokenLine = line, tokenColumn = column;

        if (isalpha(ch) || ch == '_')
        {
            while (isalnum(ch) || ch == '_')
            {
                lexeme += ch;
                next();
            }
            auto it = keywordMap.find(lexeme);
            if (it != keywordMap.end())
            {
                return Token(it->second, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::IDENTIFIER, lexeme, tokenLine, tokenColumn);
            }
        }
        if (isdigit(ch))
        {
            if (ch == '0')
            {
                lexeme += ch;
                next();
                if (ch == 'x' || ch == 'X')
                { // 十六进制
                    lexeme += ch;
                    next();
                    while (isxdigit(ch))
                    {
                        lexeme += ch;
                        next();
                    }
                    return Token(TokenType::INT_CONST, lexeme, tokenLine, tokenColumn);
                }
                else if (isdigit(ch))
                { // 八进制
                    while (ch >= '0' && ch <= '7')
                    {
                        lexeme += ch;
                        next();
                    }
                    return Token(TokenType::INT_CONST, lexeme, tokenLine, tokenColumn);
                }
                // 否则继续处理十进制或浮点
            }
            bool dot = false;
            while (isdigit(ch) || (!dot && ch == '.'))
            {
                if (ch == '.')
                    dot = true;
                lexeme += ch;
                next();
            }
            // 检查指数
            if (ch == 'e' || ch == 'E')
            {
                lexeme += ch;
                next();
                if (ch == '+' || ch == '-')
                {
                    lexeme += ch;
                    next();
                }
                while (isdigit(ch))
                {
                    lexeme += ch;
                    next();
                }
                dot = true; // 科学计数法也是浮点
            }

            if (dot)
                return Token(TokenType::FLOAT_CONST, lexeme, tokenLine, tokenColumn);
            else
                return Token(TokenType::INT_CONST, lexeme, tokenLine, tokenColumn);
        }
        if (ch == '.')
        {
            lexeme += ch;
            next();
            if (isdigit(ch))
            {
                bool dot = true;
                while (isdigit(ch))
                {
                    lexeme += ch;
                    next();
                }
                return Token(TokenType::FLOAT_CONST, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::DOT, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == ',')
        {
            next();
            return Token(TokenType::COMMA, ",", tokenLine, tokenColumn);
        }
        if (ch == '"')
        {
            next();

            while (ch != '"')
            {
                if (ch == EOF)
                {
                    lexeme += ch;
                    return Token(TokenType::ERROR, lexeme, tokenLine, tokenColumn);
                }
                if (ch == '\\')
                {
                    next();
                    if (ch == EOF)
                    {
                        return Token(TokenType::ERROR, "", tokenLine, tokenColumn);
                    }
                    switch (ch)
                    {
                    case 'n':
                        ch = '\n';
                        break;
                    case 't':
                        ch = '\t';
                        break;
                    case '\\':
                        ch = '\\';
                        break;
                    case '"':
                        ch = '"';
                        break;
                    }
                }
                lexeme += ch;
                next();
            }
            if (ch == '"')
                next();
            return Token(TokenType::STRING, lexeme, tokenLine, tokenColumn);
        }
        if (ch == ';')
        {
            next();
            return Token(TokenType::SEMI, ";", tokenLine, tokenColumn);
        }
        if (ch == '#')
        {
            next();
            return Token(TokenType::HASHTAG, "#", tokenLine, tokenColumn);
        }
        if (ch == ':')
        {
            next();
            return Token(TokenType::COLON, ":", tokenLine, tokenColumn);
        }
        if (ch == '(')
        {
            next();
            return Token(TokenType::LPAREN, "(", tokenLine, tokenColumn);
        }
        if (ch == ')')
        {
            next();
            return Token(TokenType::RPAREN, ")", tokenLine, tokenColumn);
        }
        if (ch == '{')
        {
            next();
            return Token(TokenType::LBRACE, "{", tokenLine, tokenColumn);
        }
        if (ch == '}')
        {
            next();
            return Token(TokenType::RBRACE, "}", tokenLine, tokenColumn);
        }
        if (ch == '[')
        {
            next();
            return Token(TokenType::LBRACKET, "[", tokenLine, tokenColumn);
        }
        if (ch == ']')
        {
            next();
            return Token(TokenType::RBRACKET, "]", tokenLine, tokenColumn);
        }
        if (ch == '+')
        {
            lexeme += ch;
            next();
            if (ch == '+')
            {
                lexeme += ch;
                next();
                return Token(TokenType::INCREMENT, lexeme, tokenLine, tokenColumn);
            }
            else if (ch == '=')
            {
                lexeme += ch;
                next();
                return Token(TokenType::ADD_ASSIGN, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::ADD, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == '-')
        {
            lexeme += ch;
            next();
            if (ch == '-')
            {
                lexeme += ch;
                next();
                return Token(TokenType::DECREMENT, lexeme, tokenLine, tokenColumn);
            }
            else if (ch == '=')
            {
                lexeme += ch;
                next();
                return Token(TokenType::SUB_ASSIGN, lexeme, tokenLine, tokenColumn);
            }
            else if (ch == '>')
            {
                lexeme += ch;
                next();
                return Token(TokenType::ARROW, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::SUB, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == '*')
        {
            lexeme += ch;
            next();
            if (ch == '=')
            {
                lexeme += ch;
                next();
                return Token(TokenType::MUL_ASSIGN, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::MUL, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == '/')
        {
            lexeme += ch;
            next();
            if (ch == '=')
            {
                lexeme += ch;
                next();
                return Token(TokenType::DIV_ASSIGN, lexeme, tokenLine, tokenColumn);
            }
            else if (ch == '/')
            {
                while (ch != '\n' && ch != EOF)
                    next();
                return Token(TokenType::SIGNAL_COMMENT, lexeme, tokenLine, tokenColumn);
            }
            else if (ch == '*')
            {
                next();
                while (true)
                {
                    if (ch == EOF)
                        return Token(TokenType::ERROR, lexeme, tokenLine, tokenColumn);
                    if (ch == '*')
                    {
                        next();
                        if (ch == '/')
                        {
                            next();
                            break;
                        }
                    }
                    else
                    {
                        next();
                    }
                }
                return Token(TokenType::BLOCK_COMMENT, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::DIV, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == '%')
        {
            lexeme += ch;
            next();
            if (ch == '=')
            {
                lexeme += ch;
                next();
                return Token(TokenType::MOD_ASSIGN, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::MOD, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == '=')
        {
            lexeme += ch;
            next();
            if (ch == '=')
            {
                lexeme += ch;
                next();
                return Token(TokenType::EQUAL, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::ASSIGN, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == '<')
        {
            lexeme += ch;
            next();
            if (ch == '<')
            {
                lexeme += ch;
                next();
                if (ch == '=')
                {
                    lexeme += ch;
                    next();
                    return Token(TokenType::LEFT_SHIFT_ASSIGN, lexeme, tokenLine, tokenColumn);
                }
                else
                {
                    return Token(TokenType::LEFT_SHIFT, lexeme, tokenLine, tokenColumn);
                }
            }
            else if (ch == '=')
            {
                lexeme += ch;
                next();
                return Token(TokenType::LESS_EQUAL, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::LESS_THAN, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == '>')
        {
            lexeme += ch;
            next();
            if (ch == '>')
            {
                lexeme += ch;
                next();
                if (ch == '=')
                {
                    lexeme += ch;
                    next();
                    return Token(TokenType::RIGHT_SHIFT_ASSIGN, lexeme, tokenLine, tokenColumn);
                }
                else
                {
                    return Token(TokenType::RIGHT_SHIFT, lexeme, tokenLine, tokenColumn);
                }
            }
            else if (ch == '=')
            {
                lexeme += ch;
                next();
                return Token(TokenType::GREATER_EQUAL, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::GREATER_THAN, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == '!')
        {
            lexeme += ch;
            next();
            if (ch == '=')
            {
                lexeme += ch;
                next();
                return Token(TokenType::NOT_EQUAL, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::NOT, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == '&')
        {
            lexeme += ch;
            next();
            if (ch == '&')
            {
                lexeme += ch;
                next();
                return Token(TokenType::AND, lexeme, tokenLine, tokenColumn);
            }
            else if (ch == '=')
            {
                lexeme += ch;
                next();
                return Token(TokenType::AND_ASSIGN, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::BITWISE_AND, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == '|')
        {
            lexeme += ch;
            next();
            if (ch == '|')
            {
                lexeme += ch;
                next();
                return Token(TokenType::OR, lexeme, tokenLine, tokenColumn);
            }
            else if (ch == '=')
            {
                lexeme += ch;
                next();
                return Token(TokenType::OR_ASSIGN, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::BITWISE_OR, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == '^')
        {
            lexeme += ch;
            next();
            if (ch == '=')
            {
                lexeme += ch;
                next();
                return Token(TokenType::BITWISE_XOR_ASSIGN, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::BITWISE_XOR, lexeme, tokenLine, tokenColumn);
            }
        }
        if (ch == '~')
        {
            lexeme += ch;
            next();
            return Token(TokenType::BITWISE_NOT, lexeme, tokenLine, tokenColumn);
        }
        if (ch == '\'')
        {
            lexeme += ch;
            next();
            if (ch == '\\')
            {
                lexeme += ch;
                next();
                if (ch == EOF)
                {
                    return Token(TokenType::ERROR, lexeme, tokenLine, tokenColumn);
                }
                switch (ch)
                {
                case 'n':
                    ch = '\n';
                    break;
                case 't':
                    ch = '\t';
                    break;
                case '\\':
                    ch = '\\';
                    break;
                case '\'':
                    ch = '\'';
                    break;
                }
                lexeme += ch;
                next();
                if (ch == '\'')
                {
                    lexeme += ch;
                    next();
                    return Token(TokenType::CHAR_CONST, lexeme, tokenLine, tokenColumn);
                }
                else
                {
                    return Token(TokenType::ERROR, lexeme, tokenLine, tokenColumn);
                }
            }
            else if (ch == EOF)
            {
                return Token(TokenType::ERROR, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                lexeme += ch;
                next();
                if (ch == '\'')
                {
                    lexeme += ch;
                    next();
                    return Token(TokenType::CHAR_CONST, lexeme, tokenLine, tokenColumn);
                }
                else
                {
                    return Token(TokenType::ERROR, lexeme, tokenLine, tokenColumn);
                }
            }
        }
        if (ch == '?')
        {
            lexeme += ch;
            next();
            return Token(TokenType::QUESTMARK, lexeme, tokenLine, tokenColumn);
        }

        lexeme += ch;
        return Token(TokenType::ERROR, lexeme, tokenLine, tokenColumn);
    }
};

class Parser
{
private:
    Lexer lexer;
    Token currentToken;
    bool isStorageTypeQualifier(TokenType t)
    {
        return t == TokenType::STATIC || t == TokenType::EXTERN || t == TokenType::REGISTER || t == TokenType::CONST || t == TokenType::TYPEDEF;
    }

public:
    Parser(istream &in) : lexer(in), currentToken(lexer.gettoken()) {};
    void advance()
    {
        currentToken = lexer.gettoken();
    }
    void eat(TokenType expected)
    {
        if (currentToken.type == expected)
        {
            advance();
        }
        else
        {
            throw std::runtime_error(
                "Syntax error at line " + std::to_string(currentToken.line) +
                ", column " + std::to_string(currentToken.column) +
                ": expected " + tokenTypeToString(expected) +
                ", got " + tokenTypeToString(currentToken.type));
        }
    }
    ASTNode *program()
    {
        auto node = new programNode();
        while (currentToken.type != TokenType::END_OF_FILE)
        {
            auto ext = extdef();
            if (ext)
                node->extdeflists.push_back(ext);
            else
            {
            }
        }
    }

    ASTNode *extdef()
    {
        if (currentToken.type == TokenType::HASHTAG)
        {
            return preprocessor();
        }
        vector<string> typeName;
        bool storageClass = false, isUsigned = false;
        while (isStorageTypeQualifier(currentToken.type))
        {
            if (storageClass && (currentToken.type == TokenType::STATIC || currentToken.type == TokenType::EXTERN || currentToken.type == TokenType::REGISTER || currentToken.type == TokenType::TYPEDEF))
            {
                throw std::runtime_error(
                    "Syntax error at line " + std::to_string(currentToken.line) +
                    ", column " + std::to_string(currentToken.column) +
                    ": multiple storage class specifiers");
            }
            if (currentToken.type == TokenType::STATIC || currentToken.type == TokenType::EXTERN || currentToken.type == TokenType::REGISTER || currentToken.type == TokenType::TYPEDEF)
                storageClass = true;
            typeName.push_back(currentToken.lexeme);
            advance();
        }
        if (currentToken.type == TokenType::UNSIGNED)
        {
            isUsigned = true;
            typeName.push_back(currentToken.lexeme);
            advance();
        }
        if (currentToken.type == TokenType::LONG)
        {
            typeName.push_back(currentToken.lexeme);
            advance();
            if (currentToken.type == TokenType::LONG)
            {
                typeName.push_back(currentToken.lexeme);
                advance();
                if (currentToken.type == TokenType::INT)
                {
                    typeName.push_back(currentToken.lexeme);
                    advance();
                }
            }
            else if (currentToken.type == TokenType::INT || (currentToken.type == TokenType::DOUBLE && isUsigned == false))
            {
                typeName.push_back(currentToken.lexeme);
                advance();
            }
        }
        else if (currentToken.type == TokenType::SHORT)
        {
        }
    }

    ASTNode *preprocessor()
    {
        eat(TokenType::HASHTAG);
        if (currentToken.type == TokenType::INCLUDE)
        {
            string directive = "#";
            directive += currentToken.lexeme;
            directive += ' ';
            eat(TokenType::INCLUDE);
            if (currentToken.type == TokenType::STRING)
            {
                directive += "\"" + currentToken.lexeme + "\"";
                eat(TokenType::STRING);
                return new Preprocessor(directive);
            }
            else if (currentToken.type == TokenType::LESS_THAN)
            {
                directive += "<";
                eat(TokenType::LESS_THAN);
                if (currentToken.type == TokenType::IDENTIFIER)
                {
                    directive += currentToken.lexeme;
                    eat(TokenType::IDENTIFIER);
                    while (currentToken.type == TokenType::DOT || currentToken.type == TokenType::IDENTIFIER)
                    {
                        if (currentToken.type == TokenType::DOT)
                        {
                            directive += ".";
                            eat(TokenType::DOT);
                        }
                        if (currentToken.type == TokenType::IDENTIFIER)
                        {
                            directive += currentToken.lexeme;
                            eat(TokenType::IDENTIFIER);
                        }
                    }
                    if (currentToken.type == TokenType::GREATER_THAN)
                    {
                        directive += ">";
                        eat(TokenType::GREATER_THAN);
                        return new Preprocessor(directive);
                    }
                    else
                    {
                        throw std::runtime_error(
                            "Syntax error at line " + std::to_string(currentToken.line) +
                            ", column " + std::to_string(currentToken.column) +
                            ": expected '>' after #include <...>");
                    }
                }
                else
                {
                    throw std::runtime_error(
                        "Syntax error at line " + std::to_string(currentToken.line) +
                        ", column " + std::to_string(currentToken.column) +
                        ": expected IDENTIFIER after #include <");
                }
            }
            else
            {
                throw std::runtime_error(
                    "Syntax error at line " + std::to_string(currentToken.line) +
                    ", column " + std::to_string(currentToken.column) +
                    ": expected STRING after #include");
            }
        }
    }
    ASTNode *extvardef()
    {
    }
};