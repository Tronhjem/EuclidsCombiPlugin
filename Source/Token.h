#pragma once

#include <string>

enum class TokenType
{
    // Single-character tokens.
    LEFT_PAREN = 0,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COMMA,
    
    // MATH
    DOT,
    MINUS,
    PLUS,
    SLASH,
    STAR,

    // One or two character tokens.
    EQUAL,
    EQUAL_EQUAL,
    BANG_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals.
    IDENTIFIER,
    STRING,
    NUMBER,

    // Logic
    AND,
    OR,
    XOR,
    
    // Keywords.
    PRINT,
    TEST,
    RANDOM,
    EUCLIDEAN,
    ERROR,
    
    // Specific
    //TRACK,
    NOTE,
    CC,

    // ENDS
    EOL, // End Of Line
    END,
    
// UNUSED STUFF
//    SEMICOLON,
//    BANG,
//    CLASS,
//    ELSE,
//    FALSE,
//    FUN,
//    FOR,
//    IF,
//    NIL,
//    RETURN,
//    SUPER,
//    THIS,
//    TRUE,
//    VAR,
//    WHILE,
    
};

class Token
{
public:
    Token(TokenType tokenType, 
          const char* start,
          int length, 
          int line);
        
    TokenType GetType() { return mTokenType; }

    TokenType mTokenType;
    const char* mStart;
    int mLength;
    int mLine;

private:
    Token() = delete;
};

