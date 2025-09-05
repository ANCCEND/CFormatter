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
    VarDecl,      // 变量声明
    TypeDef,      // 类型定义
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

/*if (currentToken.type == TokenType::UNSIGNED)
        {
            isUsigned = true;
            typeName.push_back(currentToken.lexeme);
            advance();
        }
        else if (currentToken.type == TokenType::SIGNED)
        {
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
            typeName.push_back(currentToken.lexeme);
            advance();
            if (currentToken.type == TokenType::INT)
            {
                typeName.push_back(currentToken.lexeme);
                advance();
            }
        }
        else if (currentToken.type == TokenType::INT || currentToken.type == TokenType::CHAR || currentToken.type == TokenType::FLOAT || currentToken.type == TokenType::DOUBLE || currentToken.type == TokenType::VOID)
        {
            typeName.push_back(currentToken.lexeme);
            advance();
        }
        else if (currentToken.type == TokenType::STRUCT || currentToken.type == TokenType::UNION || currentToken.type == TokenType::ENUM)
        {
            // struct, union, enum 还没写
        }

        else
        {
            throw std::runtime_error(
                "Syntax error at line " + std::to_string(currentToken.line) +
                ", column " + std::to_string(currentToken.column) +
                ": expected type specifier");
        }*/

/*while (currentToken.type == TokenType::STRUCT || currentToken.type == TokenType::UNION)
        {
            typeName.push_back(currentToken.lexeme);
            advance();
            if (currentToken.type == TokenType::IDENTIFIER)
            {
                typeName.push_back(currentToken.lexeme);
                advance();
            }
            if (currentToken.type == TokenType::LBRACE)
            {
                return
            }
        }*/