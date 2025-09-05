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

class ProgramNode : public ASTNode
{
public:
    vector<ASTNode *> extdeflists;
    ProgramNode() : ASTNode(ASTNodeType::Program) {}
    ~ProgramNode()
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

class ExtVarDecl : public ASTNode
{
public:
    TypeSpec *typeName;      // 变量类型，例如 "int"
    vector<string> varNames; // 变量名列表
    ExtVarDecl(TypeSpec *type, const vector<string> &names)
        : ASTNode(ASTNodeType::ExtVarDecl), typeName(type), varNames(names) {}
    ~ExtVarDecl()
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

class FunctionDef : public ASTNode
{
public:
    TypeSpec *returnType;                        // 返回类型
    string functionName;                         // 函数名
    vector<pair<TypeSpec *, string>> parameters; // 参数列表，包含类型和名称
    ASTNode *body;                               // 函数体

    FunctionDef(TypeSpec *retType, const string &funcName,
                const vector<pair<TypeSpec *, string>> &params, ASTNode *bdy)
        : ASTNode(ASTNodeType::FunctionDef), returnType(retType), functionName(funcName), parameters(params), body(bdy) {}
    ~FunctionDef()
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

class FuncionDeclNode : public ASTNode
{
public:
    TypeSpec *returnType;                        // 返回类型
    string functionName;                         // 函数名
    vector<pair<TypeSpec *, string>> parameters; // 参数列表，包含类型和名称
    FuncionDeclNode(TypeSpec *retType, const string &funcName,
                    const vector<pair<TypeSpec *, string>> &params)
        : ASTNode(ASTNodeType::FunctionDecl), returnType(retType), functionName(funcName), parameters(params) {}
    ~FuncionDeclNode()
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
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "FunctionDecl: " << functionName << "\n";
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

class TypeDefNode : public ASTNode
{
public:
    TypeSpec *typeName; // 类型名称
    string alias;       // 别名
    TypeDefNode(TypeSpec *type, const string &al)
        : ASTNode(ASTNodeType::TypeDef), typeName(type), alias(al) {}
    ~TypeDefNode()
    {
        if (typeName)
        {
            delete typeName;
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "TypeDef: " << alias << "\n";
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
    SHORT,
    UNSIGNED,
    SIGNED,
    INT,
    LONG,
    FLOAT,
    DOUBLE,

    CHAR_CONST,
    INT_CONST,
    FLOAT_CONST,
    LONG_CONST,
    DOUBLE_CONST,
    LONG_LONG_CONST,
    USIGNED_INT_CONST,
    UNSIGNED_LONG_CONST,
    UNSIGNED_LONG_LONG_CONST,

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

static const unordered_map<TokenType, string> TokenMap{
    {TokenType::ERROR, "ERROR"},
    {TokenType::IDENTIFIER, "IDENTIFIER"},

    {TokenType::VOID, "VOID"},
    {TokenType::CHAR, "CHAR"},
    {TokenType::SHORT, "SHORT"},
    {TokenType::UNSIGNED, "UNSIGNED"},
    {TokenType::SIGNED, "SIGNED"},
    {TokenType::INT, "INT"},
    {TokenType::LONG, "LONG"},
    {TokenType::FLOAT, "FLOAT"},
    {TokenType::DOUBLE, "DOUBLE"},

    {TokenType::CHAR_CONST, "CHAR_CONST"},
    {TokenType::INT_CONST, "INT_CONST"},
    {TokenType::FLOAT_CONST, "FLOAT_CONST"},
    {TokenType::LONG_CONST, "LONG_CONST"},
    {TokenType::DOUBLE_CONST, "DOUBLE_CONST"},
    {TokenType::LONG_LONG_CONST, "LONG_LONG_CONST"},
    {TokenType::USIGNED_INT_CONST, "USIGNED_INT_CONST"},
    {TokenType::UNSIGNED_LONG_CONST, "UNSIGNED_LONG_CONST"},
    {TokenType::UNSIGNED_LONG_LONG_CONST, "UNSIGNED_LONG_LONG_CONST"},

    {TokenType::IF, "IF"},
    {TokenType::ELSE, "ELSE"},
    {TokenType::SWITCH, "SWITCH"},
    {TokenType::WHILE, "WHILE"},
    {TokenType::DO, "DO"},
    {TokenType::FOR, "FOR"},
    {TokenType::BREAK, "BREAK"},
    {TokenType::CONTINUE, "CONTINUE"},
    {TokenType::RETURN, "RETURN"},
    {TokenType::SIZEOF, "SIZEOF"},
    {TokenType::TYPEDEF, "TYPEDEF"},
    {TokenType::DEFAULT, "DEFAULT"},
    {TokenType::CASE, "CASE"},
    {TokenType::STATIC, "STATIC"},
    {TokenType::EXTERN, "EXTERN"},
    {TokenType::REGISTER, "REGISTER"},
    {TokenType::CONST, "CONST"},
    {TokenType::STRUCT, "STRUCT"},
    {TokenType::UNION, "UNION"},
    {TokenType::ENUM, "ENUM"},

    {TokenType::INCLUDE, "INCLUDE"},
    {TokenType::DEFINE, "DEFINE"},
    {TokenType::UNDEF, "UNDEF"},

    {TokenType::ADD, "ADD"},
    {TokenType::DIV, "DIV"},
    {TokenType::MOD, "MOD"},
    {TokenType::MUL, "MUL"},
    {TokenType::ASSIGN, "ASSIGN"},
    {TokenType::SUB, "SUB"},
    {TokenType::ADD_ASSIGN, "ADD_ASSIGN"},
    {TokenType::DIV_ASSIGN, "DIV_ASSIGN"},
    {TokenType::MOD_ASSIGN, "MOD_ASSIGN"},
    {TokenType::MUL_ASSIGN, "MUL_ASSIGN"},
    {TokenType::SUB_ASSIGN, "SUB_ASSIGN"},
    {TokenType::LEFT_SHIFT, "LEFT_SHIFT"},
    {TokenType::RIGHT_SHIFT, "RIGHT_SHIFT"}, // <<, >>
    {TokenType::BITWISE_AND, "BITWISE_AND"},
    {TokenType::BITWISE_OR, "BITWISE_OR"},
    {TokenType::BITWISE_XOR, "BITWISE_XOR"}, // &, |, ^
    {TokenType::BITWISE_NOT, "BITWISE_NOT"}, // ~
    {TokenType::INCREMENT, "INCREMENT"},
    {TokenType::DECREMENT, "DECREMENT"}, // ++, --
    {TokenType::ARROW, "ARROW"},         // ->
    {TokenType::AND, "AND"},
    {TokenType::OR, "OR"},
    {TokenType::NOT, "NOT"}, // &&, ||, !
    {TokenType::LEFT_SHIFT_ASSIGN, "LEFT_SHIFT_ASSIGN"},
    {TokenType::RIGHT_SHIFT_ASSIGN, "RIGHT_SHIFT_ASSIGN"},
    {TokenType::AND_ASSIGN, "AND_ASSIGN"},
    {TokenType::OR_ASSIGN, "OR_ASSIGN"},
    {TokenType::NOT_ASSIGN, "NOT_ASSIGN"},
    {TokenType::BITWISE_AND_ASSIGN, "BITWISE_AND_ASSIGN"},
    {TokenType::BITWISE_OR_ASSIGN, "BITWISE_OR_ASSIGN"},
    {TokenType::BITWISE_XOR_ASSIGN, "BITWISE_XOR_ASSIGN"}, // <<=, >>=, &=, |=, ^=
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::NOT_EQUAL, "NOT_EQUAL"},
    {TokenType::LESS_THAN, "LESS_THAN"},
    {TokenType::GREATER_THAN, "GREATER_THAN"},
    {TokenType::LESS_EQUAL, "LESS_EQUAL"},
    {TokenType::GREATER_EQUAL, "GREATER_EQUAL"}, // ==, !=, <, >, <=, >=
    {TokenType::STRING, "STRING"},
    {TokenType::LPAREN, "LPAREN"},     // (
    {TokenType::RPAREN, "RPAREN"},     // )
    {TokenType::LBRACE, "LBRACE"},     // {
    {TokenType::RBRACE, "RBRACE"},     // }
    {TokenType::LBRACKET, "LBRACKET"}, // [
    {TokenType::RBRACKET, "RBRACKET"}, // ]
    {TokenType::DOT, "DOT"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::SEMI, "SEMI"},                     // ;
    {TokenType::HASHTAG, "HASHTAG"},               // #
    {TokenType::COLON, "COLON"},                   // :
    {TokenType::QUESTMARK, "QUESTMARK"},           // ?
    {TokenType::SIGNAL_COMMENT, "SIGNAL_COMMENT"}, // //
    {TokenType::BLOCK_COMMENT, "BLOCK_COMMENT"},   // /* */
    {TokenType::END_OF_FILE, "END_OF_FILE"},
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
    auto it = TokenMap.find(type); 
    if (it != TokenMap.end())
    {
        return it->second;
    }
    return "UNKNOWN";
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
        else if (isdigit(ch))
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
                    if (ch == 'u' || ch == 'U')
                    {
                        lexeme += ch;
                        next();
                        if (ch == 'l' || ch == 'L')
                        {
                            lexeme += ch;
                            next();
                            if (ch == 'l' || ch == 'L')
                            {
                                lexeme += ch;
                                next();
                                return Token(TokenType::UNSIGNED_LONG_LONG_CONST, lexeme, tokenLine, tokenColumn);
                            }
                            return Token(TokenType::UNSIGNED_LONG_CONST, lexeme, tokenLine, tokenColumn);
                        }
                        return Token(TokenType::USIGNED_INT_CONST, lexeme, tokenLine, tokenColumn);
                    }
                    if (ch == 'l' || ch == 'L')
                    {
                        lexeme += ch;
                        next();
                        if (ch == 'l' || ch == 'L')
                        {
                            lexeme += ch;
                            next();
                            return Token(TokenType::LONG_LONG_CONST, lexeme, tokenLine, tokenColumn);
                        }
                        return Token(TokenType::LONG_CONST, lexeme, tokenLine, tokenColumn);
                    }
                }
                else if (isdigit(ch))
                { // 八进制
                    while (ch >= '0' && ch <= '7')
                    {
                        lexeme += ch;
                        next();
                    }
                    if (ch == 'u' || ch == 'U')
                    {
                        lexeme += ch;
                        next();
                        if (ch == 'l' || ch == 'L')
                        {
                            lexeme += ch;
                            next();
                            if (ch == 'l' || ch == 'L')
                            {
                                lexeme += ch;
                                next();
                                return Token(TokenType::UNSIGNED_LONG_LONG_CONST, lexeme, tokenLine, tokenColumn);
                            }
                            return Token(TokenType::UNSIGNED_LONG_CONST, lexeme, tokenLine, tokenColumn);
                        }
                        return Token(TokenType::USIGNED_INT_CONST, lexeme, tokenLine, tokenColumn);
                    }
                    if (ch == 'l' || ch == 'L')
                    {
                        lexeme += ch;
                        next();
                        if (ch == 'l' || ch == 'L')
                        {
                            lexeme += ch;
                            next();
                            return Token(TokenType::LONG_LONG_CONST, lexeme, tokenLine, tokenColumn);
                        }
                        return Token(TokenType::LONG_CONST, lexeme, tokenLine, tokenColumn);
                    }
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
            if (ch == 'f' || ch == 'F')
            {
                lexeme += ch;
                next();
                return Token(TokenType::FLOAT_CONST, lexeme, tokenLine, tokenColumn);
            }
            if (ch == 'l' || ch == 'L')
            {
                lexeme += ch;
                next();
                if (dot)
                {
                    return Token(TokenType::DOUBLE_CONST, lexeme, tokenLine, tokenColumn);
                }
                if (ch == 'l' || ch == 'L')
                {
                    lexeme += ch;
                    next();
                    return Token(TokenType::LONG_LONG_CONST, lexeme, tokenLine, tokenColumn);
                }
            }
            if (ch == 'u' || ch == 'U')
            {
                lexeme += ch;
                next();
                if (dot)
                {
                    return Token(TokenType::ERROR, lexeme, tokenLine, tokenColumn);
                }
                if (ch == 'l' || ch == 'L')
                {
                    lexeme += ch;
                    next();
                    if (ch == 'l' || ch == 'L')
                    {
                        lexeme += ch;
                        next();
                        return Token(TokenType::UNSIGNED_LONG_LONG_CONST, lexeme, tokenLine, tokenColumn);
                    }
                    return Token(TokenType::UNSIGNED_LONG_CONST, lexeme, tokenLine, tokenColumn);
                }
                return Token(TokenType::USIGNED_INT_CONST, lexeme, tokenLine, tokenColumn);
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
            {
                return Token(TokenType::DOUBLE_CONST, lexeme, tokenLine, tokenColumn);
            }
            else
            {
                return Token(TokenType::INT_CONST, lexeme, tokenLine, tokenColumn);
            }
        }
        else if (ch == '.')
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
        else if (ch == ',')
        {
            next();
            return Token(TokenType::COMMA, ",", tokenLine, tokenColumn);
        }
        else if (ch == '"')
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
        else if (ch == ';')
        {
            next();
            return Token(TokenType::SEMI, ";", tokenLine, tokenColumn);
        }
        else if (ch == '#')
        {
            next();
            return Token(TokenType::HASHTAG, "#", tokenLine, tokenColumn);
        }
        else if (ch == ':')
        {
            next();
            return Token(TokenType::COLON, ":", tokenLine, tokenColumn);
        }
        else if (ch == '(')
        {
            next();
            return Token(TokenType::LPAREN, "(", tokenLine, tokenColumn);
        }
        else if (ch == ')')
        {
            next();
            return Token(TokenType::RPAREN, ")", tokenLine, tokenColumn);
        }
        else if (ch == '{')
        {
            next();
            return Token(TokenType::LBRACE, "{", tokenLine, tokenColumn);
        }
        else if (ch == '}')
        {
            next();
            return Token(TokenType::RBRACE, "}", tokenLine, tokenColumn);
        }
        else if (ch == '[')
        {
            next();
            return Token(TokenType::LBRACKET, "[", tokenLine, tokenColumn);
        }
        else if (ch == ']')
        {
            next();
            return Token(TokenType::RBRACKET, "]", tokenLine, tokenColumn);
        }
        else if (ch == '+')
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
        else if (ch == '-')
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
        else if (ch == '*')
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
        else if (ch == '/')
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
        else if (ch == '%')
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
        else if (ch == '=')
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
        else if (ch == '<')
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
        else if (ch == '>')
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
        else if (ch == '!')
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
        else if (ch == '&')
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
        else if (ch == '|')
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
        else if (ch == '^')
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
        else if (ch == '~')
        {
            lexeme += ch;
            next();
            return Token(TokenType::BITWISE_NOT, lexeme, tokenLine, tokenColumn);
        }
        else if (ch == '\'')
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
        else if (ch == '?')
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
    void throwError(const string &message)
    {
        throw std::runtime_error(
            "Syntax error at line " + std::to_string(currentToken.line) +
            ", column " + std::to_string(currentToken.column) +
            ": " + message);
    }
    bool isStorageType(TokenType t)
    {
        return t == TokenType::STATIC || t == TokenType::EXTERN || t == TokenType::REGISTER; // || t == TokenType::CONST || t == TokenType::TYPEDEF;
    }
    void checkTypeCombination(const std::vector<std::string> &typeName)
    {
        int longCount = 0, shortCount = 0, intCount = 0, voidCount = 0, doubleCount = 0, floatCount = 0, charCount = 0, signedCount = 0, unsignedCount = 0;

        for (auto &t : typeName)
        {
            if (t == "unsigned")
                unsignedCount++;
            else if (t == "signed")
                signedCount++;
            else if (t == "int")
                intCount++;
            else if (t == "char")
                charCount++;
            else if (t == "float")
                floatCount++;
            else if (t == "double")
                doubleCount++;
            else if (t == "long")
                longCount++;
            else if (t == "short")
                shortCount++;
            else if (t == "void")
                voidCount++;
        }

        // 1. 数量约束
        if (shortCount > 1)
            throwError("multiple 'short'");
        if (longCount > 2)
            throwError("too many 'long'");
        if (intCount > 1)
            throwError("multiple 'int'");
        if (voidCount > 1)
            throwError("multiple 'void'");
        if (floatCount > 1)
            throwError("multiple 'float'");
        if (doubleCount > 1)
            throwError("multiple 'double'");
        if (charCount > 1)
            throwError("multiple 'char'");
        if (signedCount > 1)
            throwError("multiple 'signed'");
        if (unsignedCount > 1)
            throwError("multiple 'unsigned'");

        // 2. signed / unsigned 互斥
        if (unsignedCount && signedCount)
            throwError("'signed' and 'unsigned' cannot be used together");

        // 3. void 不能和其他类型组合
        if (voidCount && (intCount || charCount || floatCount || doubleCount || shortCount || longCount || signedCount || unsignedCount))
            throwError("'void' cannot be combined with other types");

        // 4. char 规则
        if (charCount & (intCount || floatCount || doubleCount || shortCount || longCount))
            throwError("'char' cannot be combined with 'int', 'float', 'double', 'short', or 'long'");
        // 5. float / double 规则
        if (floatCount && (intCount || charCount || shortCount || longCount || signedCount || unsignedCount))
            throwError("'float' cannot be combined with 'int', 'char', 'short', 'long', 'signed', or 'unsigned'");
        if (doubleCount)
        {
            if (shortCount)
                throwError("'short double' is not allowed");
            if (unsignedCount || signedCount)
                throwError("'unsigned double' or 'signed double' is not allowed");
            if (longCount > 1)
                throwError("too many 'long' for 'double'");
            // allow "long double"
        }
        // 6. int 规则
        if (intCount && (floatCount || doubleCount))
            throwError("'int' cannot be combined with 'float' or 'double'");
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
            throwError("expected token " + tokenTypeToString(expected) + ", got " + tokenTypeToString(currentToken.type));
        }
    }
    ASTNode *program()
    {
        auto node = new ProgramNode();
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
            return preProcessor();
        }
        vector<string> typeName;
        bool HasStorageClass = false, HasTypeSpec = false;
        int count = 0;
        //处理存储类型
        while (isStorageType(currentToken.type))
        {
            if (HasStorageClass && isStorageType(currentToken.type))
                throwError("multiple storage class specifiers");

            HasStorageClass = true;
            typeName.push_back(currentToken.lexeme);

            advance();
        }
        //处理typedef
        if (currentToken.type == TokenType::TYPEDEF)
        {
            if (HasStorageClass)
                throwError("storage class specifier and 'typedef' cannot be used together");
            typeName.push_back(currentToken.lexeme);
            advance();
            return typeDef();
        }
        //处理类型说明符
        while (currentToken.type == TokenType::VOID || currentToken.type == TokenType::CHAR || currentToken.type == TokenType::SHORT || currentToken.type == TokenType::INT || currentToken.type == TokenType::LONG || currentToken.type == TokenType::FLOAT || currentToken.type == TokenType::DOUBLE || currentToken.type == TokenType::UNSIGNED || currentToken.type == TokenType::SIGNED || currentToken.type == TokenType::CONST)
        {
            if (currentToken.type != TokenType::CONST)
            {
                HasTypeSpec = true;
            }
            typeName.push_back(currentToken.lexeme);
            advance();
        }
        if (HasTypeSpec == false)
            throwError("expected type specifier");
        checkTypeCombination(typeName);
        TypeSpec *typeSpec = new TypeSpec(typeName);

        if (currentToken.type == TokenType::IDENTIFIER)
        {
            string varName = currentToken.lexeme;
            eat(TokenType::IDENTIFIER);
            if (currentToken.type == TokenType::LPAREN)
            {
                // 函数定义或声明
                return funcDeclOrDef();
            }
            else
            {
                // 变量定义
                return extVarDef();
            }
        }
        else
            throwError("expected IDENTIFIER");
    }

    ASTNode *preProcessor()
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

    ASTNode *extVarDef()
    {
    }
    ASTNode *typeDef()
    {
        bool HasTypeSpec = false;
        vector<string> typeName;
        
        while (currentToken.type == TokenType::VOID || currentToken.type == TokenType::CHAR || currentToken.type == TokenType::SHORT || currentToken.type == TokenType::INT || currentToken.type == TokenType::LONG || currentToken.type == TokenType::FLOAT || currentToken.type == TokenType::DOUBLE || currentToken.type == TokenType::UNSIGNED || currentToken.type == TokenType::SIGNED || currentToken.type == TokenType::CONST)
        {
            if (currentToken.type != TokenType::CONST)
            {
                HasTypeSpec = true;
            }
            typeName.push_back(currentToken.lexeme);
            advance();
        }
        if(isStorageType(currentToken.type))
            throwError("storage class specifier cannot appear after type specifier");
        if (HasTypeSpec == false)
            throwError("expected type specifier");
        checkTypeCombination(typeName);
        TypeSpec *typeSpec = new TypeSpec(typeName);

        if (currentToken.type == TokenType::IDENTIFIER)
        {
            string typeDefName = currentToken.lexeme;
            eat(TokenType::IDENTIFIER);
            eat(TokenType::SEMI);
            return new TypeDefNode(typeSpec, typeDefName);
        }
        else
            throwError("expected IDENTIFIER in typedef");
    }
    ASTNode *funcDeclOrDef()
    {
    }
};