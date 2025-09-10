#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>

using namespace std;

enum class ASTNodeType
{
    Program,      // 程序
    Preprocessor, // 预处理指令
    ExtVarDecl,   // 外部变量定义
    TypeSpec,     // 类型说明符
    FunctionDef,  // 函数定义
    FunctionDecl, // 函数声明
    VarDeclList,  // 变量声明列表
    LocalVarDecl, // 变量声明
    VarClass,     // 变量类别（标识符或数组）
    VarInit,      // 变量初始化
    TypeDef,      // 类型定义
    CompoundStmt, // 语句块 { ... }
    IfStmt,       // if 语句
    THenStmt,     // then 语句
    ElseStmt,     // else 语句
    WhileStmt,    // while 语句
    DoWhileStmt,  // do-while 语句
    ReturnStmt,   // return 语句
    SwitchStmt,   // switch 语句
    SwitchCase,   // case 语句
    DefaultCase,  // default 语句
    ForStmt,      // for 语句
    BreakStmt,    // break 语句
    ContinueStmt, // continue 语句
    StuctStmt,    // struct 语句
    EmptyStmt,    // 空语句
    Statement,    // 语句
    BinaryExpr,   // 二元表达式
    UnaryExpr,    // 一元表达式
    Literal,      // 常量
    Identifier,   // 标识符
    FuncCallExpr, // 函数调用
    AssignExpr    // 赋值表达式
};

enum class VarKind
{
    IDENT,
    ARRAY,
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

    NONE
};

class ASTNode
{
public:
    ASTNodeType type;
    ASTNode(ASTNodeType t) : type(t) {}
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
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

class TypeSpec : public ASTNode
{
public:
    vector<string> typeName; // 类型名称，例如 "int", "float"
    TypeSpec(const vector<string> &names)
        : ASTNode(ASTNodeType::TypeSpec), typeName(names) {}
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "Type: ";
        for (const auto &name : typeName)
        {
            cout << name << " ";
        }
        cout << "\n";
    }
};

class VarDeclNode : public ASTNode
{
public:
    TypeSpec *typeName;
    string name;                  // 变量名
    vector<ASTNode *> arraySizes; // 数组维度大小表达式列表
    ASTNode *init = nullptr;      // 初始化表达式，若无初始化则为 nullptr
    VarDeclNode(TypeSpec *type, const string &varName, const vector<ASTNode *> &sizes, ASTNode *initializer = nullptr)
        : ASTNode(ASTNodeType::VarDeclList), typeName(type), name(varName), arraySizes(sizes), init(initializer) {}
    ~VarDeclNode()
    {
        if (typeName)
        {
            delete typeName;
        }
        for (auto size : arraySizes)
        {
            if (size)
            {
                delete size;
            }
        }
        if (init)
        {
            delete init;
        }
    }

    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "VarDecl: " << name << "\n";
        if (typeName)
        {
            typeName->print(indent + 2);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null TypeSpec\n";
        }
        if (!arraySizes.empty())
        {
            cout << string(indent + 2, ' ') << "Array Sizes:\n";
            for (const auto &size : arraySizes)
            {
                cout << string(indent + 4, ' ') << '[';
                if (size)
                {
                    size->print(0);
                }
                cout << string(indent + 4, ' ') << ']' << endl;
            }
        }
        if (init)
        {
            cout << string(indent + 2, ' ') << "Initializer:\n";
            init->print(indent + 4);
        }
    }
};

class VarInitList : public ASTNode
{
public:
    vector<ASTNode *> inits;
    VarInitList(const vector<ASTNode *> &initializers)
        : ASTNode(ASTNodeType::VarDeclList), inits(initializers) {}
    ~VarInitList()
    {
        for (auto init : inits)
        {
            if (init)
            {
                delete init;
            }
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "VarInitList:\n";
        for (const auto &init : inits)
        {
            if (init)
            {
                init->print(indent + 2);
            }
            else
            {
                cout << string(indent + 2, ' ') << "Error: Null Initializer\n";
            }
        }
    }
};

class LocalVarDecl : public ASTNode
{
public:
    vector<VarDeclNode *> varDecls; // 变量声明列表
    LocalVarDecl(const vector<VarDeclNode *> &vars)
        : ASTNode(ASTNodeType::LocalVarDecl), varDecls(vars) {}
    ~LocalVarDecl()
    {
        for (auto var : varDecls)
        {
            if (var)
            {
                delete var;
            }
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "LocalVarDecl:\n";
        for (const auto &var : varDecls)
        {
            if (var)
            {
                var->print(indent + 2);
            }
            else
            {
                cout << string(indent + 2, ' ') << "Error: Null VarDecl\n";
            }
        }
    }
};

class ExtVarDecl : public ASTNode
{
public:
    vector<VarDeclNode *> varDecls;
    ExtVarDecl(const vector<VarDeclNode *> &vars)
        : ASTNode(ASTNodeType::ExtVarDecl), varDecls(vars) {}
    ~ExtVarDecl()
    {
        for (auto var : varDecls)
        {
            if (var)
            {
                delete var;
            }
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "ExtVarDecl:\n";
        for (const auto &var : varDecls)
        {
            if (var)
            {
                var->print(indent + 2);
            }
            else
            {
                cout << string(indent + 2, ' ') << "Error: Null VarDecl\n";
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
    ASTNode *body = nullptr;                     // 函数体

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
            cout << string(indent + 2, ' ') << "Return ";
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
    TypeSpec *returnType;                                // 返回类型
    vector<string> functionNames;                        // 函数名
    vector<vector<pair<TypeSpec *, string>>> parameters; // 参数列表，包含类型和名称
    FuncionDeclNode(TypeSpec *retType, const vector<string> &funcName,
                    const vector<vector<pair<TypeSpec *, string>>> &params)
        : ASTNode(ASTNodeType::FunctionDecl), returnType(retType), functionNames(funcName), parameters(params) {}
    ~FuncionDeclNode()
    {
        if (returnType)
        {
            delete returnType;
        }
        for (auto &param : parameters)
        {
            for (auto &p : param)
            {
                if (p.first)
                {
                    delete p.first;
                }
            }
        }
    }
    void print(int indent = 0) const override
    {
        for (auto &funcName : functionNames)
        {
            cout << string(indent, ' ') << "FunctionDecl: " << funcName << "\n";
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
                for (const auto &p : param)
                {
                    if (p.first)
                    {
                        p.first->print(indent + 4);
                        cout << string(indent + 4, ' ') << "Param Name: " << p.second << "\n";
                    }
                    /*else
                    {
                        cout << string(indent + 4, ' ') << "Error: Null Parameter Type\n";
                    }*/
                }
            }
        }
    }
};

class TypeDefNode : public ASTNode
{
public:
    TypeSpec *typeName;   // 类型名称
    vector<string> alias; // 别名
    TypeDefNode(TypeSpec *type, const vector<string> &al)
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
        cout << string(indent, ' ') << "TypeDef: ";
        if (typeName)
        {
            typeName->print(indent + 2);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null TypeSpec\n";
        }
        for (const auto &al : alias)
        {
            if (!al.empty())
            {
                cout << string(indent + 2, ' ') << "Alias: " << al << "\n";
            }
            else
            {
                cout << string(indent + 2, ' ') << "Error: Empty Alias Name\n";
            }
        }
    }
};

class CompoundStmt : public ASTNode
{
public:
    vector<ASTNode *> vardecls;
    vector<ASTNode *> statements; // 语句列表
    CompoundStmt(const vector<ASTNode *> &vars, const vector<ASTNode *> &stmts)
        : ASTNode(ASTNodeType::CompoundStmt), vardecls(vars), statements(stmts) {}
    ~CompoundStmt()
    {
        for (auto decl : vardecls)
        {
            if (decl)
                delete decl;
        }
        for (auto stmt : statements)
        {
            if (stmt)
                delete stmt;
        }
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "CompoundStmt:\n";
        cout << string(indent + 2, ' ') << "VarDecls:\n";
        for (const auto &decl : vardecls)
        {
            if (decl)
            {
                decl->print(indent + 4);
            }
            else
            {
                cout << string(indent + 4, ' ') << "Error: Null VarDecl\n";
            }
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

class DoWhileStmt : public ASTNode
{
public:
    ASTNode *body;
    ASTNode *condition;
    DoWhileStmt(ASTNode *bdy, ASTNode *cond) : ASTNode(ASTNodeType::DoWhileStmt), body(bdy), condition(cond) {}
    ~DoWhileStmt()
    {
        if (body)
            delete body;
        if (condition)
            delete condition;
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "DoWhileStmt:\n";
        if (body)
        {
            cout << string(indent + 2, ' ') << "Body:\n";
            body->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Body\n";
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

class EmptyStmt : public ASTNode
{
public:
    EmptyStmt() : ASTNode(ASTNodeType::EmptyStmt) {}
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "EmptyStmt\n";
    }
};

class BinaryExpr : public ASTNode
{
public:
    ASTNode *left;
    ASTNode *right;
    string op;
    ASTNode *funcCallExpr = nullptr;
    BinaryExpr(ASTNode *l, ASTNode *r, ASTNode *funcCall)
        : ASTNode(ASTNodeType::BinaryExpr), left(l), right(r), funcCallExpr(funcCall) {}
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
        if (funcCallExpr)
        {
            cout << string(indent, ' ') << "FuncCallExpr:\n";
            funcCallExpr->print(indent + 2);
            return;
        }
        else if (!op.empty())
        {
            cout << string(indent, ' ') << "BinaryExpr: " << op << "\n";
        }
        if (left)
        {
            cout << string(indent + 2, ' ') << "Left:\n";
            left->print(indent + 4);
        }
        /*else
        {
            cout << string(indent + 2, ' ') << "Error: Null Left Operand\n";
        }*/
        if (right)
        {
            cout << string(indent + 2, ' ') << "Right:\n";
            right->print(indent + 4);
        }
        /*else
        {
            cout << string(indent + 2, ' ') << "Error: Null Right Operand\n";
        }*/
    }
};

class Literal : public ASTNode
{
    // 字面量
public:
    string value;
    TokenType tokenType;
    Literal() : ASTNode(ASTNodeType::Literal), value(""), tokenType(TokenType::NONE) {}
    Literal(const string &val, TokenType lt) : ASTNode(ASTNodeType::Literal), value(val), tokenType(lt) {}
    void print(int indent = 0) const override
    {
        if (!value.empty())
        {
            cout << string(indent, ' ') << "Literal: " << value << "\n";
        }
    }
};

class Identifier : public ASTNode
{
    // 标识符
public:
    string name;
    Identifier(const string &n)
        : ASTNode(ASTNodeType::Identifier), name(n) {}
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "Identifier: " << name << "\n";
    }
};

class FuncCallExpr : public ASTNode
{
public:
    string functionName;
    vector<ASTNode *> arguments;
    FuncCallExpr(const string &fname, const vector<ASTNode *> &args)
        : ASTNode(ASTNodeType::FuncCallExpr), functionName(fname), arguments(args) {}
    ~FuncCallExpr()
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

class AssignExpr : public ASTNode
{
public:
    string varName;
    ASTNode *value;
    AssignExpr(const string &vname, ASTNode *val)
        : ASTNode(ASTNodeType::AssignExpr), varName(vname), value(val) {}
    ~AssignExpr()
    {
        if (value)
            delete value;
    }
    void print(int indent = 0) const override
    {
        cout << string(indent, ' ') << "AssignExpr: " << varName << "\n";
        if (value)
        {
            cout << string(indent + 2, ' ') << "Value:\n";
            value->print(indent + 4);
        }
        else
        {
            cout << string(indent + 2, ' ') << "Error: Null Value\n";
        }
    }
};