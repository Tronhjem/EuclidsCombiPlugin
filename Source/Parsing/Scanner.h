#pragma once

#include <vector>
#include <string>
#include "Token.h"

class Logger;


class Scanner
{
public:
    Scanner(Logger& logger);
    ~Scanner();
    bool ScanTokens(char *data);

    std::vector<Token>& GetTokens() { return mTokens; }

private:
    Token ScanToken();
    Token MakeToken(TokenType token);
    Token MakeErrorToken(char* message, char symbol);
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

private:
    Logger& mLogger;
    // const char* mData;
    std::vector<Token> mTokens;

    char* mStart;
    char* mCurrent;
    int mCurrentLine = 1;
    char ERROR_UNEXPECTED_CHAR[50] = "ERROR: Unexpected character ";
    char ERROR_NO_END_QUOTE[50] = "ERROR: Expected \" but didn't find one ";
};