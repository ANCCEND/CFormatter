#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>

using namespace std;

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

int main()
{
    Lexer lexer(cin);
    while (true)
    {
        Token token = lexer.gettoken();
        cout << "Token: " << tokenTypeToString(token.type)
             << ", Lexeme: " << token.lexeme
             << ", Line: " << token.line
             << ", Column: " << token.column << endl;
        if (token.type == TokenType::END_OF_FILE)
            break;
        if (token.type == TokenType::ERROR)
        {
            cerr << "Lexical Error at Line: " << token.line << ", Column: " << token.column << endl;
            break;
        }
    }
    return 0;
}