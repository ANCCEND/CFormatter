#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>
#include <stack>
#include <unordered_map>
#include "ASTNodes.hpp"

using namespace std;

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

const vector<TokenType> constants = {
    TokenType::INT_CONST, TokenType::FLOAT_CONST, TokenType::CHAR_CONST, TokenType::STRING, TokenType::IDENTIFIER, TokenType::LONG_CONST, TokenType::DOUBLE_CONST, TokenType::LONG_LONG_CONST, TokenType::UNSIGNED_LONG_CONST, TokenType::UNSIGNED_LONG_LONG_CONST};

inline std::string tokenTypeToString(TokenType type)
{
    auto it = TokenMap.find(type);
    if (it != TokenMap.end())
    {
        return it->second;
    }
    return "UNKNOWN";
}

void printToken(Token token)
{
    cout << "Token(Type: " << tokenTypeToString(token.type) << ", Lexeme: \"" << token.lexeme << "\", Line: " << token.line << ", Column: " << token.column << ")\n";
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
                if (dot)
                {
                    lexeme += ch;
                    next();
                    return Token(TokenType::FLOAT_CONST, lexeme, tokenLine, tokenColumn);
                }
                else
                {
                    lexeme += ch;
                    next();
                    return Token(TokenType::ERROR, lexeme, tokenLine, tokenColumn);
                }
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
                {
                    lexeme += ch;
                    next();
                }
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
                        lexeme += ch;
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
        else if (ch == '\\')
        {
            // 处理行继续符
            lexeme += ch;
            next();
            return Token(TokenType::BACKSLASH, lexeme, tokenLine, tokenColumn);
        }

        lexeme += ch;
        Token errorToken(TokenType::ERROR, lexeme, tokenLine, tokenColumn);
        throw std::runtime_error("Unknown token: " + lexeme + " at line " + std::to_string(tokenLine) + ", column " + std::to_string(tokenColumn));
        next();
        return errorToken;
    }

    Token peektoken()
    {
        auto currentPos = input.tellg();
        int currentLine = line;
        int currentColumn = column;
        char currentCh = ch;

        try
        {
            Token nextToken = gettoken();
            while (nextToken.type == TokenType::SIGNAL_COMMENT || nextToken.type == TokenType::BLOCK_COMMENT)
            {
                nextToken = gettoken();
            }

            input.clear(); // 清除EOF标志
            input.seekg(currentPos);
            line = currentLine;
            column = currentColumn;
            ch = currentCh;

            return nextToken;
        }
        catch (...)
        {
            // Restore the lexer state in case of an exception
            input.clear(); // 清除EOF标志
            input.seekg(currentPos);
            line = currentLine;
            column = currentColumn;
            ch = currentCh;

            throw; // Re-throw the exception
        }
    }
};

class Parser
{
private:
    Lexer lexer;
    Token currentToken;
    vector<string> funcNames; // 用于存储函数名
    bool debug = false;
    void throwError(const string &message)
    {
        throw std::runtime_error(
            "Syntax error at line " + std::to_string(currentToken.line) +
            ", column " + std::to_string(currentToken.column) +
            ": " + message);
    }
    bool isStorageType(TokenType t)
    {
        return t == TokenType::STATIC || t == TokenType::EXTERN || t == TokenType::REGISTER;
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
    VarInitList *arrInitList()
    {
        if (debug)
            cout << "arrInitList" << endl;
        if (currentToken.type == TokenType::SIGNAL_COMMENT || currentToken.type == TokenType::BLOCK_COMMENT)
        {
            advance();
        }
        else if (currentToken.type == TokenType::STRING)
        {
            // 字符数组初始化
            auto strNode = new Literal(currentToken.lexeme, currentToken.type);
            advance();
            return new VarInitList({strNode});
        }

        eat(TokenType::LBRACE);
        vector<ASTNode *> initList;
        // 允许空初始化列表 {}
        if (currentToken.type != TokenType::RBRACE)
        {
            while (true)
            {
                if (currentToken.type == TokenType::LBRACE)
                {
                    initList.push_back(arrInitList()); // 嵌套初始化器
                }
                else
                {
                    initList.push_back(Expression()); // 普通表达式
                }

                if (currentToken.type == TokenType::COMMA)
                {
                    eat(TokenType::COMMA);
                    // 允许逗号后直接右花括号，例如 {1,2,}
                    if (currentToken.type == TokenType::RBRACE)
                        break;
                }
                else
                {
                    break;
                }
            }
        }
        eat(TokenType::RBRACE);
        return new VarInitList(initList);
    }

public:
    Parser(istream &in, bool debug) : lexer(in), currentToken(lexer.gettoken()), debug(debug) {};
    void advance()
    {
        currentToken = lexer.gettoken();
        while (currentToken.type == TokenType::SIGNAL_COMMENT || currentToken.type == TokenType::BLOCK_COMMENT)
        {
            advance();
        }
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
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "program" << endl;
        auto node = new ProgramNode();
        while (currentToken.type != TokenType::END_OF_FILE)
        {
            auto ext = extdef();
            if (debug)
                cout << "extdef-parsed" << endl;
            if (ext)
                node->extdeflists.push_back(ext);
            else
            {
                throwError("invalid external definition");
            }
            if (debug)
                printToken(currentToken);
        }
        if (debug)
            cout << "number of extdefs: " << node->extdeflists.size() << endl;
        return node;
    }

    ASTNode *extdef()
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "extdef " << currentToken.lexeme << endl;
        if (currentToken.type == TokenType::SIGNAL_COMMENT || currentToken.type == TokenType::BLOCK_COMMENT)
        {
            advance();
        }
        if (currentToken.type == TokenType::HASHTAG)
        {
            return preProcessor();
        }
        vector<string> typeName;
        bool HasStorageClass = false, HasTypeSpec = false;
        // 处理存储类型
        while (isStorageType(currentToken.type))
        {
            if (HasStorageClass && isStorageType(currentToken.type))
                throwError("multiple storage class specifiers");

            HasStorageClass = true;
            typeName.push_back(currentToken.lexeme);

            advance();
        }
        // 处理typedef
        if (currentToken.type == TokenType::TYPEDEF)
        {
            if (HasStorageClass)
                throwError("storage class specifier and 'typedef' cannot be used together");
            typeName.push_back(currentToken.lexeme);
            advance();
            return typeDef();
        }
        // 处理类型说明符
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
            vector<string> Names;
            Names.push_back(currentToken.lexeme);
            tokenTypeToString(currentToken.type);

            Token nextToken = lexer.peektoken();
            if (nextToken.type == TokenType::LPAREN)
            {
                // 函数定义或声明

                ASTNode *nodetest = funcDeclOrDef(typeSpec, Names);
                if (debug)
                    cout << "funcDeclOrDef-parsed" << endl;
                return nodetest;
            }
            else if (nextToken.type == TokenType::COMMA || nextToken.type == TokenType::SEMI || nextToken.type == TokenType::LBRACKET || nextToken.type == TokenType::ASSIGN)
            {
                // 变量定义
                return extVarDecl(typeSpec);
            }
            else
            {
                throwError("unexpected token after IDENTIFIER");
            }
        }
        else
            throwError("expected IDENTIFIER");
        return nullptr;
    }

    ASTNode *preProcessor()
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "preprocessor " << endl;
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
        else if (currentToken.type == TokenType::DEFINE)
        {
            string directive = "#";
            directive += currentToken.lexeme;
            directive += ' ';
            eat(TokenType::DEFINE);
            if (currentToken.type == TokenType::IDENTIFIER)
            {
                directive += currentToken.lexeme;
                eat(TokenType::IDENTIFIER);
                int line = currentToken.line;
                while (currentToken.type != TokenType::END_OF_FILE)
                {
                    if (currentToken.type == TokenType::BACKSLASH)
                    {
                        eat(TokenType::BACKSLASH);
                        if (currentToken.type == TokenType::END_OF_FILE)
                            break;
                        if (currentToken.line == line)
                            throwError("expected newline after '\\'");
                        line = currentToken.line;
                    }
                    if (currentToken.line != line)
                        break;

                    directive += ' ' + currentToken.lexeme;
                    advance();
                }
                return new Preprocessor(directive);
            }
            else
            {
                throwError("expected IDENTIFIER after #define");
            }
        }
        else
        {
            throwError("expected 'include' or 'define' after '#'");
        }
        return nullptr;
    }

    ASTNode *extVarDecl(TypeSpec *typeSpec)
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "extvaldecl" << endl;
        vector<VarDeclNode *> varDecls;
        string currentName;
        while (currentToken.type != TokenType::SEMI)
        {
            if (currentToken.type == TokenType::SIGNAL_COMMENT || currentToken.type == TokenType::BLOCK_COMMENT)
            {
                advance();
                continue;
            }
            if (currentToken.type == TokenType::IDENTIFIER)
            {
                currentName = currentToken.lexeme;
                eat(TokenType::IDENTIFIER);
                if (currentToken.type == TokenType::SEMI)
                {
                    varDecls.push_back(new VarDeclNode(typeSpec, currentName, {}, nullptr));
                    break;
                }
                else if (currentToken.type == TokenType::LBRACKET)
                {
                    // 处理数组
                    vector<ASTNode *> arraySizes;
                    while (currentToken.type == TokenType::LBRACKET)
                    {
                        advance();
                        if (currentToken.type != TokenType::RBRACKET)
                        {
                            arraySizes.push_back(Expression());
                        }
                        else
                        {
                            arraySizes.push_back(nullptr); // 未指定大小
                        }
                        eat(TokenType::RBRACKET);
                    }
                    if (currentToken.type == TokenType::ASSIGN)
                    {
                        advance();
                        VarInitList *initList = arrInitList();
                        // 处理数组初始化
                        varDecls.push_back(new VarDeclNode(typeSpec, currentName, arraySizes, initList));
                        if (currentToken.type == TokenType::SEMI)
                            break;
                        else if (currentToken.type == TokenType::COMMA)
                            eat(TokenType::COMMA);
                        else
                            throwError("unexpected token after array initialization");
                    }
                    else if (currentToken.type == TokenType::COMMA || currentToken.type == TokenType::SEMI)
                    {
                        // 未初始化
                        varDecls.push_back(new VarDeclNode(typeSpec, currentName, arraySizes, nullptr));
                        if (currentToken.type == TokenType::SEMI)
                            break;
                        else
                            eat(TokenType::COMMA);
                    }
                    else
                    {
                        throwError("expected '=' or ',' or ';' after array declaration");
                    }
                }
                else if (currentToken.type == TokenType::COMMA)
                {
                    varDecls.push_back(new VarDeclNode(typeSpec, currentName, {}, nullptr));
                    // 处理多个变量声明
                    eat(TokenType::COMMA);
                    continue;
                }
                else if (currentToken.type == TokenType::ASSIGN)
                {
                    advance();
                    // 处理初始化
                    varDecls.push_back(new VarDeclNode(typeSpec, currentName, {}, Expression()));
                    if (currentToken.type == TokenType::SEMI)
                        break;
                    else if (currentToken.type == TokenType::COMMA)
                        eat(TokenType::COMMA);
                    else
                        throwError("unexpected token after variable initialization");
                }
                else
                {
                    throwError("expected '[' or ',' or '=' or ';' after IDENTIFIER in variable declaration");
                }
            }
            else
            {
                throwError("expected IDENTIFIER in variable declaration");
            }
        }
        eat(TokenType::SEMI);
        return new ExtVarDecl(varDecls);
    }

    ASTNode *localVarDecl()
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "localvaldecl" << endl;
        vector<string> typeName;
        string currentName;
        bool HasStorageClass = false, HasTypeSpec = false;
        // 处理存储类型
        while (isStorageType(currentToken.type))
        {
            if (HasStorageClass && isStorageType(currentToken.type))
                throwError("multiple storage class specifiers");

            HasStorageClass = true;
            typeName.push_back(currentToken.lexeme);
            advance();
        }
        // 处理类型说明符
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
        if (currentToken.type != TokenType::IDENTIFIER)
        {
            throwError("expected IDENTIFIER in variable declaration");
        }

        vector<VarDeclNode *> varDecls;
        while (currentToken.type != TokenType::SEMI)
        {
            if (currentToken.type == TokenType::IDENTIFIER)
            {
                currentName = currentToken.lexeme;
                eat(TokenType::IDENTIFIER);
                if (currentToken.type == TokenType::SEMI)
                {
                    varDecls.push_back(new VarDeclNode(typeSpec, currentName, {}, nullptr));
                    break;
                }
                else if (currentToken.type == TokenType::LBRACKET)
                {
                    // 处理数组
                    vector<ASTNode *> arraySizes;
                    while (currentToken.type == TokenType::LBRACKET)
                    {
                        advance();
                        if (currentToken.type != TokenType::RBRACKET)
                        {
                            arraySizes.push_back(Expression());
                            // if(debug) printToken(currentToken);
                        }
                        else
                        {
                            arraySizes.push_back(nullptr); // 未指定大小
                        }
                        eat(TokenType::RBRACKET);
                    }
                    if (currentToken.type == TokenType::ASSIGN)
                    {
                        advance();
                        VarInitList *initList = arrInitList();
                        // 处理数组初始化
                        varDecls.push_back(new VarDeclNode(typeSpec, currentName, arraySizes, initList));
                        if (currentToken.type == TokenType::SEMI)
                        {
                            break;
                        }
                        else if (currentToken.type == TokenType::COMMA)
                            eat(TokenType::COMMA);
                        else
                            throwError("unexpected token after array initialization");
                    }
                    else if (currentToken.type == TokenType::COMMA || currentToken.type == TokenType::SEMI)
                    {
                        // 未初始化
                        varDecls.push_back(new VarDeclNode(typeSpec, currentName, arraySizes, nullptr));
                        if (currentToken.type == TokenType::SEMI)
                        {
                            eat(TokenType::SEMI);
                            break;
                        }
                        else
                            eat(TokenType::COMMA);
                    }
                    else
                    {
                        throwError("expected '=' or ',' or ';' after array declaration");
                    }
                }
                else if (currentToken.type == TokenType::COMMA)
                {
                    varDecls.push_back(new VarDeclNode(typeSpec, currentName, {}, nullptr));
                    // 处理多个变量声明
                    eat(TokenType::COMMA);
                }
                else if (currentToken.type == TokenType::ASSIGN)
                {
                    advance();
                    // 处理初始化
                    varDecls.push_back(new VarDeclNode(typeSpec, currentName, {}, Expression()));
                    if (currentToken.type == TokenType::SEMI)
                        break;
                    else if (currentToken.type == TokenType::COMMA)
                        eat(TokenType::COMMA);
                    else
                        throwError("unexpected token after variable initialization");
                }
                else
                {
                    throwError("expected '[' or ',' or '=' or ';' after IDENTIFIER in variable declaration");
                }
            }
            else
            {
                throwError("expected IDENTIFIER in variable declaration");
            }
        }
        return new LocalVarDecl(varDecls);
    }

    ASTNode *funcDeclOrDef(TypeSpec *FuncReturnType, vector<string> &FuncNames)
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "fundeclordef" << endl;
        eat(TokenType::IDENTIFIER);
        eat(TokenType::LPAREN);
        vector<vector<pair<TypeSpec *, string>>> allParams;
        vector<pair<TypeSpec *, string>> params;

        while (currentToken.type != TokenType::RPAREN)
        {
            if (debug)
            {
                cout << "parsing params for func " << FuncNames.back() << ":\n  ";
                printToken(currentToken);
            }
            bool HasTypeSpec = false, HasVoidType = false;
            vector<string> paramTypeName;
            // 处理参数类型说明符
            while (currentToken.type == TokenType::VOID || currentToken.type == TokenType::CHAR || currentToken.type == TokenType::SHORT || currentToken.type == TokenType::INT || currentToken.type == TokenType::LONG || currentToken.type == TokenType::FLOAT || currentToken.type == TokenType::DOUBLE || currentToken.type == TokenType::UNSIGNED || currentToken.type == TokenType::SIGNED || currentToken.type == TokenType::CONST)
            {
                if (currentToken.type != TokenType::CONST)
                {
                    HasTypeSpec = true;
                }
                if (currentToken.type == TokenType::VOID)
                    HasVoidType = true;
                if (currentToken.type == TokenType::VOID && paramTypeName.size() > 0)
                {
                    throwError("'void' must be the only type specifier in parameter");
                }
                paramTypeName.push_back(currentToken.lexeme);
                advance();
            }
            // printToken(currentToken);
            if (HasTypeSpec == false)
            {
                throwError("expected type specifier in function parameter");
            }
            if (HasVoidType && paramTypeName.size() > 1)
                throwError("'void' must be the only type specifier in parameter");
            checkTypeCombination(paramTypeName);
            TypeSpec *paramTypeSpec = new TypeSpec(paramTypeName);

            // 处理参数名
            if (paramTypeName.size() == 1 && paramTypeName[0] == "void")
            {
                // 参数类型为void，表示无参数
                if (currentToken.type != TokenType::RPAREN)
                {
                    throwError("expected ')' after 'void' in function parameter");
                }
                allParams.push_back(params);
                break;
            }

            else if (currentToken.type == TokenType::IDENTIFIER)
            {
                string paramName = currentToken.lexeme;
                eat(TokenType::IDENTIFIER);
                params.push_back({paramTypeSpec, paramName});
            }
            else
            {
                throwError("expected IDENTIFIER in function parameter");
            }

            if (currentToken.type == TokenType::COMMA)
            {
                eat(TokenType::COMMA);
            }
        }
        eat(TokenType::RPAREN);
        allParams.push_back(params);
        params.clear();
        if (debug)
        {
            cout << "allparams parsed" << endl;
            printToken(currentToken);
        }
        if (currentToken.type == TokenType::SEMI)
        {
            // 函数声明
            eat(TokenType::SEMI);
            if (debug)
                cout << FuncNames.size() << " function names parsed" << endl;
            return new FuncionDeclNode(FuncReturnType, FuncNames, allParams);
        }
        else if (currentToken.type == TokenType::COMMA)
        {
            // 函数声明，多个函数名
            while (currentToken.type == TokenType::COMMA)
            {
                eat(TokenType::COMMA);
                if (currentToken.type == TokenType::IDENTIFIER)
                {
                    FuncNames.push_back(currentToken.lexeme);
                    if (debug)
                        cout << "fun number: " << FuncNames.size() << endl;
                    eat(TokenType::IDENTIFIER);
                }
                else
                {
                    throwError("expected IDENTIFIER after ',' in function declaration");
                }
                eat(TokenType::LPAREN);
                while (currentToken.type != TokenType::RPAREN)
                {
                    if (debug)
                    {
                        cout << "parsing params for func " << FuncNames.back() << endl;
                        printToken(currentToken);
                    }
                    bool HasTypeSpec = false, HasVoidType = false;
                    vector<string> paramTypeName;
                    // 处理参数类型说明符
                    while (currentToken.type == TokenType::VOID || currentToken.type == TokenType::CHAR || currentToken.type == TokenType::SHORT || currentToken.type == TokenType::INT || currentToken.type == TokenType::LONG || currentToken.type == TokenType::FLOAT || currentToken.type == TokenType::DOUBLE || currentToken.type == TokenType::UNSIGNED || currentToken.type == TokenType::SIGNED || currentToken.type == TokenType::CONST)
                    {
                        if (currentToken.type != TokenType::CONST)
                        {
                            HasTypeSpec = true;
                        }
                        if (currentToken.type == TokenType::VOID)
                            HasVoidType = true;
                        if (currentToken.type == TokenType::VOID && paramTypeName.size() > 0)
                        {
                            throwError("'void' must be the only type specifier in parameter");
                        }
                        paramTypeName.push_back(currentToken.lexeme);
                        advance();
                    }
                    // printToken(currentToken);
                    if (HasTypeSpec == false)
                    {
                        throwError("expected type specifier in function parameter");
                    }
                    if (HasVoidType && paramTypeName.size() > 1)
                        throwError("'void' must be the only type specifier in parameter");
                    checkTypeCombination(paramTypeName);
                    TypeSpec *paramTypeSpec = new TypeSpec(paramTypeName);

                    // 处理参数名
                    if (paramTypeName.size() == 1 && paramTypeName[0] == "void")
                    {
                        // 参数类型为void，表示无参数
                        if (currentToken.type != TokenType::RPAREN)
                        {
                            throwError("expected ')' after 'void' in function parameter");
                        }
                        allParams.push_back(params);
                        break;
                    }

                    else if (currentToken.type == TokenType::IDENTIFIER)
                    {
                        string paramName = currentToken.lexeme;
                        eat(TokenType::IDENTIFIER);
                        params.push_back({paramTypeSpec, paramName});
                    }
                    else
                    {
                        throwError("expected IDENTIFIER in function parameter");
                    }

                    if (currentToken.type == TokenType::COMMA)
                    {
                        eat(TokenType::COMMA);
                    }
                }
                eat(TokenType::RPAREN);
                allParams.push_back(params);
                params.clear();
                if (debug)
                {
                    cout << "allparams parsed" << endl;
                    printToken(currentToken);
                }
            }
            eat(TokenType::SEMI);
            return new FuncionDeclNode(FuncReturnType, FuncNames, allParams);
        }
        else if (currentToken.type == TokenType::LBRACE)
        {
            // 函数定义
            ASTNode *body = compoundStmt();
            if (debug)
            {
                cout << "func-body-parsed" << endl;
                if (FuncNames.size() == 0)
                {
                    throwError("function must have a name");
                }
            }
            if (allParams.size() == 0)
            {
                allParams.push_back({}); // 无参数
            }
            if (FuncNames.size() == 0)
            {
                throwError("function must have a name");
            }

            return new FunctionDef(FuncReturnType, FuncNames[0], allParams[0], body);
        }
        else
        {
            throwError("unexpected token after function parameter list");
        }
        return nullptr;
    }

    ASTNode *typeDef()
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "typedef" << endl;
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
        if (isStorageType(currentToken.type))
            throwError("storage class specifier cannot appear after typedef");
        if (HasTypeSpec == false)
            throwError("expected type specifier");
        checkTypeCombination(typeName);
        TypeSpec *typeSpec = new TypeSpec(typeName);
        bool hasTypeDefName = false;
        while (currentToken.type == TokenType::IDENTIFIER)
        {
            vector<string> typeDefName;
            typeDefName.push_back(currentToken.lexeme);
            eat(TokenType::IDENTIFIER);
            eat(TokenType::SEMI);
            return new TypeDefNode(typeSpec, typeDefName);
        }
        if (hasTypeDefName == false)
            throwError("expected IDENTIFIER after typedef");
        return nullptr;
    }

    ASTNode *compoundStmt()
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "compoundstmt" << endl;
        eat(TokenType::LBRACE);
        vector<ASTNode *> localDecls;
        vector<ASTNode *> statements;
        while (currentToken.type != TokenType::RBRACE)
        {
            if (currentToken.type == TokenType::INT || currentToken.type == TokenType::CHAR || currentToken.type == TokenType::SHORT || currentToken.type == TokenType::LONG || currentToken.type == TokenType::FLOAT || currentToken.type == TokenType::DOUBLE || currentToken.type == TokenType::UNSIGNED || currentToken.type == TokenType::SIGNED || currentToken.type == TokenType::CONST || isStorageType(currentToken.type))
            {
                statements.push_back(localVarDecl());
                eat(TokenType::SEMI);
            }
            else
            {
                statements.push_back(statement(false));
            }
        }
        eat(TokenType::RBRACE);
        return new CompoundStmt(statements);
    }

    ASTNode *statement(bool isInParen)
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (currentToken.type == TokenType::END_OF_FILE)
        {
            throwError("unexpected end of file in statement");
        }
        if (debug)
        {
            cout << "statement ";
            printToken(currentToken);
        }
        if (currentToken.type == TokenType::IF)
        {
            return ifStatement();
        }
        else if (currentToken.type == TokenType::WHILE)
        {
            return whileStatement();
        }
        else if (currentToken.type == TokenType::DO)
        {
            return doWhileStatement();
        }
        else if (currentToken.type == TokenType::FOR)
        {
            return forStatement();
        }
        else if (currentToken.type == TokenType::RETURN)
        {
            return returnStatement();
        }
        else if (currentToken.type == TokenType::SWITCH)
        {
            return SwitchStatement();
        }
        else if (currentToken.type == TokenType::BREAK)
        {
            return BreakStatement();
        }
        else if (currentToken.type == TokenType::CONTINUE)
        {
            return ContinueStatement();
        }
        else if (currentToken.type == TokenType::LBRACE)
        {
            return compoundStmt();
        }
        else if (currentToken.type == TokenType::SEMI)
        {
            eat(TokenType::SEMI);
            return new EmptyStmt(); // 空语句
        }
        else if (currentToken.type == TokenType::VOID)
        {
            throwError("variable cannot be of type void");
        }
        else if (currentToken.type == TokenType::INT || currentToken.type == TokenType::CHAR || currentToken.type == TokenType::SHORT || currentToken.type == TokenType::LONG || currentToken.type == TokenType::FLOAT || currentToken.type == TokenType::DOUBLE || currentToken.type == TokenType::UNSIGNED || currentToken.type == TokenType::SIGNED || currentToken.type == TokenType::CONST || currentToken.type == TokenType::STATIC || currentToken.type == TokenType::EXTERN || currentToken.type == TokenType::REGISTER)
        {
            if (debug)
                printToken(currentToken);
            return localVarDecl();
        }
        else if (currentToken.type == TokenType::IDENTIFIER)
        {
            Token nextToken = lexer.peektoken();
            if (nextToken.type == TokenType::ASSIGN || nextToken.type == TokenType::ADD_ASSIGN || nextToken.type == TokenType::SUB_ASSIGN || nextToken.type == TokenType::MUL_ASSIGN || nextToken.type == TokenType::DIV_ASSIGN || nextToken.type == TokenType::MOD_ASSIGN || nextToken.type == TokenType::AND_ASSIGN || nextToken.type == TokenType::OR_ASSIGN || nextToken.type == TokenType::BITWISE_XOR_ASSIGN || nextToken.type == TokenType::LEFT_SHIFT_ASSIGN || nextToken.type == TokenType::RIGHT_SHIFT_ASSIGN || nextToken.type == TokenType::BITWISE_AND_ASSIGN || nextToken.type == TokenType::BITWISE_OR_ASSIGN)
            {
                auto assignstmt = assignExpression();
                if (!isInParen)
                {
                    eat(TokenType::SEMI);
                }
                return assignstmt;
            }
            else if (nextToken.type == TokenType::LBRACKET)
            {
                string varName = currentToken.lexeme;
                eat(TokenType::IDENTIFIER);
                while (currentToken.type == TokenType::LBRACKET)
                {
                    varName += "[";
                    eat(TokenType::LBRACKET);
                    while (currentToken.type != TokenType::RBRACKET)
                    {
                        varName += currentToken.lexeme;
                        advance();
                    }
                    varName += "]";
                    eat(TokenType::RBRACKET);
                }
                if (currentToken.type == TokenType::ASSIGN || currentToken.type == TokenType::ADD_ASSIGN || currentToken.type == TokenType::SUB_ASSIGN || currentToken.type == TokenType::MUL_ASSIGN || nextToken.type == TokenType::DIV_ASSIGN || currentToken.type == TokenType::MOD_ASSIGN || currentToken.type == TokenType::AND_ASSIGN || currentToken.type == TokenType::OR_ASSIGN || currentToken.type == TokenType::BITWISE_XOR_ASSIGN || currentToken.type == TokenType::LEFT_SHIFT_ASSIGN || currentToken.type == TokenType::RIGHT_SHIFT_ASSIGN || currentToken.type == TokenType::BITWISE_AND_ASSIGN || currentToken.type == TokenType::BITWISE_OR_ASSIGN)
                {
                    auto assignOp = currentToken.lexeme;
                    advance(); // 吃掉赋值运算符
                    auto expr = Expression();
                    if (!isInParen)
                    {
                        eat(TokenType::SEMI);
                    }
                    return new AssignExpr(varName, assignOp, expr);
                }
                else
                {
                    throwError("expected '=' after array variable");
                }
            }
            else
            {
                auto expr = Expression();
                if (!isInParen)
                {
                    eat(TokenType::SEMI);
                }
                return expr;
            }
        }

        else if (find(constants.begin(), constants.end(), currentToken.type) != constants.end())
        {
            auto expr = Expression();
            if (!isInParen)
                eat(TokenType::SEMI);
            return expr;
        }
        return nullptr;
    }

    ASTNode *ifStatement()
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "ifstmt" << endl;
        eat(TokenType::IF);
        eat(TokenType::LPAREN);
        ASTNode *condition = ExpressionInFuncCall();
        if (condition == nullptr)
            throwError("expected expression in if condition");
        eat(TokenType::RPAREN);
        ASTNode *thenBranch = nullptr;
        if (currentToken.type == TokenType::LBRACE)
        {
            thenBranch = compoundStmt();
        }
        else
        {
            thenBranch = statement(false);
            if (thenBranch == nullptr)
                throwError("expected statement after if condition");
        }
        ASTNode *elseBranch = nullptr;
        if (currentToken.type == TokenType::ELSE)
        {
            eat(TokenType::ELSE);
            if (currentToken.type == TokenType::IF)
            {
                elseBranch = ifStatement();
            }
            else if (currentToken.type == TokenType::LBRACE)
            {
                elseBranch = compoundStmt();
            }
            else
            {
                elseBranch = statement(false);
            }
        }

        return new IfStmt(condition, thenBranch, elseBranch);
    }

    ASTNode *whileStatement()
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "whilestmt" << endl;
        ASTNode *condition = nullptr;
        eat(TokenType::WHILE);
        eat(TokenType::LPAREN);
        condition = ExpressionInFuncCall();
        eat(TokenType::RPAREN);
        ASTNode *body = nullptr;
        if (currentToken.type == TokenType::LBRACE)
        {
            body = compoundStmt();
        }
        else
        {
            body = statement(false);
        }
        return new WhileStmt(condition, body);
    }

    ASTNode *doWhileStatement()
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "dowhilestmt" << endl;
        eat(TokenType::DO);
        ASTNode *body = nullptr;
        if (currentToken.type == TokenType::LBRACE)
        {
            body = compoundStmt();
        }
        else
        {
            body = statement(false);
        }
        eat(TokenType::WHILE);
        if (currentToken.type != TokenType::LPAREN)
            throwError("expected '(' after 'while'");
        ASTNode *condition = Expression();
        eat(TokenType::SEMI);
        return new DoWhileStmt(body, condition);
    }

    ASTNode *forStatement()
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "forstmt" << endl;
        eat(TokenType::FOR);
        eat(TokenType::LPAREN);
        ASTNode *init = nullptr;
        ASTNode *condition = nullptr;
        ASTNode *increment = nullptr;
        ASTNode *body = nullptr;
        Token nextToken = lexer.peektoken();
        if (currentToken.type != TokenType::SEMI)
        {
            if (currentToken.type == TokenType::INT || currentToken.type == TokenType::CHAR || currentToken.type == TokenType::SHORT || currentToken.type == TokenType::LONG || currentToken.type == TokenType::FLOAT || currentToken.type == TokenType::DOUBLE || currentToken.type == TokenType::UNSIGNED || currentToken.type == TokenType::SIGNED || currentToken.type == TokenType::CONST || isStorageType(currentToken.type))
            {
                if (debug)
                {
                    cout << "for-init-localvar";
                    printToken(currentToken);
                    printToken(nextToken);
                }
                init = statement(false);
                eat(TokenType::SEMI);
            }
            else
                init = statement(false);
        }
        if (currentToken.type != TokenType::SEMI)
        {
            condition = Expression();
            eat(TokenType::SEMI);
        }
        if (currentToken.type != TokenType::RPAREN)
        {
            increment = statement(true);
            eat(TokenType::RPAREN);
        }
        if (currentToken.type == TokenType::LBRACE)
        {
            body = compoundStmt();
        }
        else
        {
            body = statement(false);
        }
        return new ForStmt(init, condition, increment, body);
    }

    ASTNode *returnStatement()
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "returnstmt" << endl;
        eat(TokenType::RETURN);
        ASTNode *expr = nullptr;
        if (currentToken.type != TokenType::SEMI)
        {
            expr = Expression();
            if (debug)
                cout << "return-expr-parsed" << endl;
        }
        eat(TokenType::SEMI);
        return new ReturnStmt(expr);
    }

    ASTNode *SwitchStatement()
    {
        if (currentToken.type == TokenType::ERROR)
        {
            throwError("invalid token: " + currentToken.lexeme);
        }
        if (debug)
            cout << "switchstmt" << endl;
        eat(TokenType::SWITCH);
        eat(TokenType::LPAREN);
        auto expr = Expression();
        eat(TokenType::RPAREN);
        eat(TokenType::LBRACE);
        vector<ASTNode *> cases;
        ASTNode *defaultCase = nullptr;
        while (currentToken.type != TokenType::RBRACE)
        {
            if (currentToken.type == TokenType::CASE)
            {
                eat(TokenType::CASE);
                auto caseExpr = Expression();
                eat(TokenType::COLON);
                vector<ASTNode *> stmts;
                while (currentToken.type != TokenType::CASE && currentToken.type != TokenType::DEFAULT && currentToken.type != TokenType::RBRACE)
                {
                    auto stmt = statement(false);
                    if (stmt)
                        stmts.push_back(stmt);
                }
                cases.push_back(new SwitchCase(caseExpr, stmts));
            }
            else if (currentToken.type == TokenType::DEFAULT)
            {
                eat(TokenType::DEFAULT);
                eat(TokenType::COLON);
                vector<ASTNode *> stmts;
                while (currentToken.type != TokenType::RBRACE)
                {
                    auto stmt = statement(false);
                    if (stmt)
                        stmts.push_back(stmt);
                }
                if (defaultCase)
                    throwError("multiple default cases in switch");
                defaultCase = new DefaultCase(stmts);
            }
            else
            {
                throwError("expected 'case' or 'default' in switch statement");
            }
        }
        eat(TokenType::RBRACE);
        return new SwitchStmt(expr, cases, defaultCase);
    }

    ASTNode *BreakStatement()
    {
        if (debug)
            cout << "breakstmt" << endl;
        eat(TokenType::BREAK);
        eat(TokenType::SEMI);
        return new BreakStmt();
    }

    ASTNode *ContinueStatement()
    {
        if (debug)
            cout << "continuestmt" << endl;
        eat(TokenType::CONTINUE);
        eat(TokenType::SEMI);
        return new ContinueStmt();
    }

    ASTNode *assignExpression()
    {
        if (debug)
            cout << "assignexpression" << endl;
        string indentifier = currentToken.lexeme;
        eat(TokenType::IDENTIFIER);
        auto op = currentToken.lexeme;
        advance(); // 处理赋值运算符
        auto expr = Expression();
        return new AssignExpr(indentifier, op, expr);
    }

    FuncCallExpr *funcCall(Token nextToken)
    {
        if (debug)
            cout << "funccall" << endl;
        string funcName = currentToken.lexeme;
        eat(TokenType::IDENTIFIER);
        currentToken = nextToken;
        eat(TokenType::LPAREN);
        vector<ASTNode *> args;
        if (currentToken.type != TokenType::RPAREN)
        {
            args.push_back(ExpressionInFuncCall());
            while (currentToken.type == TokenType::COMMA)
            {
                eat(TokenType::COMMA);
                args.push_back(ExpressionInFuncCall());
            }
        }
        eat(TokenType::RPAREN);
        if (debug)
            cout << "func-call-parsed" << endl;
        return new FuncCallExpr(funcName, args);
    }

    ASTNode *Expression()
    {
        if (debug)
        {
            cout << "expression ";
            printToken(currentToken);
        }
        const vector<TokenType> operators = {
            TokenType::OR,
            TokenType::AND,
            TokenType::BITWISE_OR,
            TokenType::BITWISE_XOR,
            TokenType::BITWISE_AND,
            TokenType::EQUAL,
            TokenType::NOT_EQUAL,
            TokenType::LESS_THAN,
            TokenType::LESS_EQUAL,
            TokenType::GREATER_THAN,
            TokenType::GREATER_EQUAL,
            TokenType::LEFT_SHIFT,
            TokenType::RIGHT_SHIFT,
            TokenType::ADD,
            TokenType::SUB,
            TokenType::MUL,
            TokenType::DIV,
            TokenType::MOD,
        };

        const unordered_map<string, int> opPrecedence = {
            {"||", 1},
            {"&&", 2},
            {"|", 3},
            {"^", 4},
            {"&", 5},
            {"==", 6},
            {"!=", 6},
            {"<", 7},
            {"<=", 7},
            {">", 7},
            {">=", 7},
            {"<<", 8},
            {">>", 8},
            {"+", 9},
            {"-", 9},
            {"*", 10},
            {"/", 10},
            {"%", 10},
            {"(", 0} // 用于处理括号
        };

        stack<BinaryExpr *> opStack;
        stack<BinaryExpr *> valStack;
        BinaryExpr *root = nullptr;
        do
        {
            if (debug)
                printToken(currentToken);
            if (find(constants.begin(), constants.end(), currentToken.type) != constants.end() || currentToken.type == TokenType::IDENTIFIER)
            {
                // 处理操作数
                Token nextToken = lexer.peektoken();
                if (currentToken.type == TokenType::IDENTIFIER)
                {
                    if (nextToken.type == TokenType::LPAREN)
                    {
                        valStack.push(new BinaryExpr(nullptr, nullptr, funcCall(nextToken)));
                        continue;
                    }
                    else if (nextToken.type == TokenType::LBRACKET)
                    {
                        // 处理数组变量
                        string varName = currentToken.lexeme;
                        eat(TokenType::IDENTIFIER);
                        while (currentToken.type == TokenType::LBRACKET)
                        {
                            varName += "[";
                            eat(TokenType::LBRACKET);
                            while (currentToken.type != TokenType::RBRACKET)
                            {
                                varName += currentToken.lexeme;
                                advance();
                            }
                            varName += "]";
                            eat(TokenType::RBRACKET);
                        }
                        valStack.push(new BinaryExpr(nullptr, nullptr, varName));
                        continue;
                    }
                }
                if (currentToken.type == TokenType::CHAR_CONST)
                {
                    valStack.push(new BinaryExpr(nullptr, nullptr, currentToken.lexeme, false, true));
                }
                else if (currentToken.type == TokenType::STRING)
                {
                    valStack.push(new BinaryExpr(nullptr, nullptr, currentToken.lexeme, true, false));
                }
                else
                    valStack.push(new BinaryExpr(nullptr, nullptr, currentToken.lexeme));
                advance();
            }
            else if (find(operators.begin(), operators.end(), currentToken.type) != operators.end())
            {
                // 处理操作符
                while (!opStack.empty() && opPrecedence.at(opStack.top()->op) >= opPrecedence.at(currentToken.lexeme))
                {
                    // 处理栈顶运算符
                    auto opNode = opStack.top();
                    opStack.pop();
                    if (valStack.size() < 2)
                    {
                        throwError("insufficient values for operator " + opNode->op);
                    }
                    auto right = valStack.top();
                    valStack.pop();
                    auto left = valStack.top();
                    valStack.pop();
                    opNode->left = left;
                    opNode->right = right;
                    valStack.push(opNode);
                }
                // 将当前运算符入栈
                opStack.push(new BinaryExpr(nullptr, nullptr, currentToken.lexeme));
                advance();
            }
            else if (currentToken.type == TokenType::LPAREN)
            {
                // 处理左括号
                opStack.push(new BinaryExpr(nullptr, nullptr, "("));
                advance();
            }
            else if (currentToken.type == TokenType::RPAREN)
            {
                // 处理右括号
                while (!opStack.empty() && opStack.top()->op != "(")
                {
                    auto opNode = opStack.top();
                    opStack.pop();
                    if (valStack.size() < 2)
                    {
                        throwError("insufficient values for operator " + opNode->op);
                    }
                    auto right = valStack.top();
                    valStack.pop();
                    auto left = valStack.top();
                    valStack.pop();
                    opNode->left = left;
                    opNode->right = right;
                    valStack.push(opNode);
                }
                if (opStack.empty() || opStack.top()->op != "(")
                {
                    // throwError("mismatched parentheses");
                    break;
                }

                opStack.pop(); // 弹出左括号
                advance();
            }
            else if (currentToken.type == TokenType::SEMI || currentToken.type == TokenType::COMMA || currentToken.type == TokenType::RBRACKET || currentToken.type == TokenType::RBRACE)
            {
                // 表达式结束
                if (debug)
                    cout << "expression-end" << endl;
                break;
            }
            else
            {
                // throwError("unexpected token in expression: " + tokenTypeToString(currentToken.type));
                break;
            }
        } while (true);
        while (!opStack.empty())
        {
            // 处理剩余的运算符
            auto opNode = opStack.top();
            opStack.pop();
            if (valStack.size() < 2)
            {
                throwError("insufficient values for operator " + opNode->op);
            }
            auto right = valStack.top();
            valStack.pop();
            auto left = valStack.top();
            valStack.pop();
            opNode->left = left;
            opNode->right = right;
            valStack.push(opNode);
        }
        if (valStack.size() != 1)
        {
            throwError("invalid expression");
        }
        root = valStack.top();
        valStack.pop();
        // if(debug) printToken(currentToken);

        return root;
    }

    ASTNode *ExpressionInFuncCall()
    {
        if (debug)
        {
            cout << "expression ";
            printToken(currentToken);
        }
        const vector<TokenType> operators = {
            TokenType::OR,
            TokenType::AND,
            TokenType::BITWISE_OR,
            TokenType::BITWISE_XOR,
            TokenType::BITWISE_AND,
            TokenType::EQUAL,
            TokenType::NOT_EQUAL,
            TokenType::LESS_THAN,
            TokenType::LESS_EQUAL,
            TokenType::GREATER_THAN,
            TokenType::GREATER_EQUAL,
            TokenType::LEFT_SHIFT,
            TokenType::RIGHT_SHIFT,
            TokenType::ADD,
            TokenType::SUB,
            TokenType::MUL,
            TokenType::DIV,
            TokenType::MOD,
        };

        const unordered_map<string, int> opPrecedence = {
            {"||", 1},
            {"&&", 2},
            {"|", 3},
            {"^", 4},
            {"&", 5},
            {"==", 6},
            {"!=", 6},
            {"<", 7},
            {"<=", 7},
            {">", 7},
            {">=", 7},
            {"<<", 8},
            {">>", 8},
            {"+", 9},
            {"-", 9},
            {"*", 10},
            {"/", 10},
            {"%", 10},
            {"(", 0} // 用于处理括号;
        };

        stack<BinaryExpr *> opStack;
        stack<BinaryExpr *> valStack;
        BinaryExpr *root = nullptr;
        do
        {
            if (debug)
                printToken(currentToken);
            if (find(constants.begin(), constants.end(), currentToken.type) != constants.end() || currentToken.type == TokenType::IDENTIFIER)
            {
                // 处理操作数
                Token nextToken = lexer.peektoken();
                if (currentToken.type == TokenType::IDENTIFIER)
                {
                    if (nextToken.type == TokenType::LPAREN)
                    {
                        valStack.push(new BinaryExpr(nullptr, nullptr, funcCall(nextToken)));
                        advance();
                        continue;
                    }
                    else if (nextToken.type == TokenType::LBRACKET)
                    {
                        // 处理数组变量
                        string varName = currentToken.lexeme;
                        eat(TokenType::IDENTIFIER);
                        while (currentToken.type == TokenType::LBRACKET)
                        {
                            varName += "[";
                            eat(TokenType::LBRACKET);
                            while (currentToken.type != TokenType::RBRACKET)
                            {
                                varName += currentToken.lexeme;
                                advance();
                            }
                            varName += "]";
                            eat(TokenType::RBRACKET);
                        }
                        valStack.push(new BinaryExpr(nullptr, nullptr, varName));
                        continue;
                    }
                }
                if (currentToken.type == TokenType::CHAR_CONST)
                {
                    valStack.push(new BinaryExpr(nullptr, nullptr, currentToken.lexeme, false, true));
                }
                else if (currentToken.type == TokenType::STRING)
                {
                    valStack.push(new BinaryExpr(nullptr, nullptr, currentToken.lexeme, true, false));
                }
                else
                    valStack.push(new BinaryExpr(nullptr, nullptr, currentToken.lexeme));
                advance();
            }
            else if (find(operators.begin(), operators.end(), currentToken.type) != operators.end())
            {
                // 处理操作符
                while (!opStack.empty() && opPrecedence.at(opStack.top()->op) >= opPrecedence.at(currentToken.lexeme))
                {
                    // 处理栈顶运算符
                    auto opNode = opStack.top();
                    opStack.pop();
                    if (valStack.size() < 2)
                    {
                        throwError("insufficient values for operator " + opNode->op);
                    }
                    auto right = valStack.top();
                    valStack.pop();
                    auto left = valStack.top();
                    valStack.pop();
                    opNode->left = left;
                    opNode->right = right;
                    valStack.push(opNode);
                }
                // 将当前运算符入栈
                opStack.push(new BinaryExpr(nullptr, nullptr, currentToken.lexeme));
                advance();
            }
            else if (currentToken.type == TokenType::LPAREN)
            {
                // 处理左括号
                opStack.push(new BinaryExpr(nullptr, nullptr, "("));
                advance();
            }
            else if (currentToken.type == TokenType::RPAREN)
            {
                // 处理右括号
                while (!opStack.empty() && opStack.top()->op != "(")
                {
                    auto opNode = opStack.top();
                    opStack.pop();
                    if (valStack.size() < 2)
                    {
                        throwError("insufficient values for operator " + opNode->op);
                    }
                    auto right = valStack.top();
                    valStack.pop();
                    auto left = valStack.top();
                    valStack.pop();
                    opNode->left = left;
                    opNode->right = right;
                    valStack.push(opNode);
                }
                if (opStack.empty() || opStack.top()->op != "(")
                {
                    break;
                }

                opStack.pop(); // 弹出左括号
                advance();
            }
            else if (currentToken.type == TokenType::SEMI || currentToken.type == TokenType::COMMA || currentToken.type == TokenType::RBRACKET || currentToken.type == TokenType::RBRACE)
            {
                // 表达式结束
                break;
            }
            else
            {
                throwError("unexpected token in expression: " + tokenTypeToString(currentToken.type));
            }
        } while (true);
        while (!opStack.empty())
        {
            // 处理剩余的运算符
            auto opNode = opStack.top();
            opStack.pop();
            if (valStack.size() < 2)
            {
                throwError("insufficient values for operator " + opNode->op);
            }
            auto right = valStack.top();
            valStack.pop();
            auto left = valStack.top();
            valStack.pop();
            opNode->left = left;
            opNode->right = right;
            valStack.push(opNode);
        }
        if (valStack.size() != 1)
        {
            throwError("invalid expression");
        }
        root = valStack.top();
        valStack.pop();
        return root;
    }
};
