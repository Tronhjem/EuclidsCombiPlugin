#include "Scanner.h"
#include "Logger.h"
#include "Token.h"
#include <stdio.h>

Scanner::Scanner(char* data, Logger& logger) : mData(data), mLogger(logger)
{
    mCurrent = data;
    mStart = data;
}

Scanner::~Scanner()
{
}

Token Scanner::MakeToken(TokenType tokenType)
{
    return Token(tokenType, mStart, static_cast<int>(mCurrent - mStart), mCurrentLine);
}

Token Scanner::MakeErrorToken(const char* message)
{
    return Token(TokenType::ERROR, message, (int)strlen(message), mCurrentLine);
}

void Scanner::SkipWhiteSpace()
{
    for (;;)
    {
        char c = PeekCurrent();
        switch (c)
        {
        case ' ':
        case '\r':
        case '\t':
            AdvanceCurrent();
            break;

        case '\n':
            mCurrentLine++;
            AdvanceCurrent();
            break;

        case '/':
        {
            if (Match('/'))
            {
                // A comment goes until the end of the line.
                while (PeekCurrent() != '\n' && !IsAtEnd())
                    AdvanceCurrent();
            }
            else
            {
                return;
            }

            break;
        }
        default:
            return;
        }
    }
}

void DebugPrint(const char* data, int length)
{
    const char* toPrint = data;
    const char* end = data + length;
    while(toPrint != end)
    {
        printf("%c", *toPrint);
        ++toPrint;
    }
    printf("%c", '\n');
}

void Scanner::ScanTokens()
{
    mLogger.LogMessage("Scanning tokens...");

    for (;;)
    {
        Token t = ScanToken();
        DebugPrint(t.mStart, t.mLength);
        if (t.GetType() == TokenType::END || t.GetType() == TokenType::ERROR)
            break;
    }
}

Token Scanner::ScanToken()
{
    SkipWhiteSpace();

    mStart = mCurrent;
    if(IsAtEnd())
        return MakeToken(TokenType::END);

    char c = AdvanceCurrent();
    if (IsAlpha(c))
        return BuildIdentifier();

    if (IsDigit(c))
        return BuildDigit();

    switch (c)
    {
    case '(':
        return MakeToken(TokenType::LEFT_PAREN);
    case ')':
        return MakeToken(TokenType::RIGHT_PAREN);
    case '{':
        return MakeToken(TokenType::LEFT_BRACE);
    case '}':
        return MakeToken(TokenType::RIGHT_BRACE);
    case ',':
        return MakeToken(TokenType::COMMA);
    case '.':
        return MakeToken(TokenType::DOT);
    case '-':
        return MakeToken(TokenType::MINUS);
    case '+':
        return MakeToken(TokenType::PLUS);
    case ';':
        return MakeToken(TokenType::SEMICOLON);
    case '*':
        return MakeToken(TokenType::STAR);
    case '/':
            return MakeToken(TokenType::SLASH);
    case '!':
        return MakeToken(Match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
    case '=':
        return MakeToken(Match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
    case '<':
        return MakeToken(Match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
    case '>':
        return MakeToken(Match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
    case '"':
        return BuildString();

    default:
        return MakeToken(TokenType::ERROR);
    }
}

bool Scanner::IsAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Scanner::IsDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool Scanner::IsAtEnd()
{
    return *mCurrent == '\0';
}

char Scanner::PeekCurrent()
{
    if (IsAtEnd())
        return '\0';

    return *mCurrent;
}

char Scanner::PeekNext()
{
    if (IsAtEnd())
        return '\0';

    return mCurrent[1];
}

char Scanner::AdvanceCurrent()
{
    ++mCurrent;
    return mCurrent[-1];
}

bool Scanner::Match(char expected)
{
    if(IsAtEnd())
        return false;

    if(*mCurrent != expected)
        return false;

    AdvanceCurrent();
    return true;
}

TokenType Scanner::IdentifierToken()
{
    
    auto checkKeyword = [this](int start, int length,
                               const char* rest, TokenType type)
    {
        if (mCurrent - mStart == start + length &&
            memcmp(mStart + start, rest, length) == 0)
        {
            return type;
        }

        return TokenType::IDENTIFIER;
    };

    // Checking if any of these are reserved words.
    switch (mStart[0])
    {
    case 'a':
        return checkKeyword(1, 2, "nd", TokenType::AND);
    case 'c':
        return checkKeyword(1, 4, "lass", TokenType::CLASS);
    case 'e':
        return checkKeyword(1, 3, "lse", TokenType::ELSE);
    case 'i':
        return checkKeyword(1, 1, "f", TokenType::IF);
    case 'n':
        return checkKeyword(1, 2, "il", TokenType::NIL);
    case 'o':
        return checkKeyword(1, 1, "r", TokenType::OR);
    case 'p':
        return checkKeyword(1, 4, "rint", TokenType::PRINT);
    case 'r':
        return checkKeyword(1, 5, "eturn", TokenType::RETURN);
    case 's':
        return checkKeyword(1, 4, "uper", TokenType::SUPER);
    case 'v':
        return checkKeyword(1, 2, "ar", TokenType::VAR);
    case 'w':
        return checkKeyword(1, 4, "hile", TokenType::WHILE);
    }

    return TokenType::IDENTIFIER;
}

Token Scanner::BuildIdentifier()
{
    while (IsAlpha(PeekCurrent()) || IsDigit(PeekCurrent()))
        AdvanceCurrent();


    return MakeToken(IdentifierToken());
}

Token Scanner::BuildString()
{
    while (PeekCurrent() != '"' && !IsAtEnd())
    {
        if(PeekCurrent() == '\n')
        {
            ++mCurrentLine;
            return MakeErrorToken("Expected \" but didn't get one");
        }

        AdvanceCurrent();
    }

    if(IsAtEnd())
    {
        return MakeErrorToken("Reached end and expected \" but didn't get one");
    }

    AdvanceCurrent();
    Token token = MakeToken(TokenType::STRING);
    return token;
}

Token Scanner::BuildDigit()
{
    while( IsDigit( PeekCurrent() )) 
        AdvanceCurrent();

    if (PeekCurrent() == '.' && IsDigit(PeekNext()))
    {
        AdvanceCurrent();

        while (IsDigit(PeekCurrent()))
            AdvanceCurrent();
    }

    return MakeToken(TokenType::NUMBER);
}