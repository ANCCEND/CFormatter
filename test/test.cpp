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