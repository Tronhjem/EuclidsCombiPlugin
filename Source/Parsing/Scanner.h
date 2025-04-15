#pragma once

#include <vector>
#include <string>
#include "Token.h"

class Logger;

class Scanner
{
// Functions
public:
    Scanner(char* data, Logger& logger);
    ~Scanner();
    void ScanTokens();

private:
    Token ScanToken();
    Token MakeToken(TokenType token);
    Token MakeErrorToken(const char*);
    Token BuildString();
    Token BuildDigit();
    Token BuildIdentifier();
    TokenType IdentifierToken();
    void SkipWhiteSpace();
    bool IsAtEnd();
    bool Match(char expected);
    char PeekCurrent();
    char PeekNext();
    char AdvanceCurrent();
    bool IsAlpha(char c);
    bool IsDigit(char c);

// Members
public:

private:
    Logger& mLogger;
    const char* mData;
    std::vector<Token> mTokens;

    char* mStart;
    char* mCurrent;
    int mCurrentLine = 1;
};