#pragma once

#include <vector>
#include <string>

#include "Token.h"

class ErrorReporting;

class Scanner
{
public:
    Scanner(ErrorReporting& logger);
    ~Scanner();
    
    bool ScanFile(char *data);
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
    
    //Helpers
    inline bool IsAtEnd();
    inline bool Match(char expected);
    inline char PeekCurrent();
    inline char PeekNext();
    inline char AdvanceCurrent();
    inline bool IsAlpha(char c);
    inline bool IsDigit(char c);

private:
    ErrorReporting& mErrorReporting;
    std::vector<Token> mTokens;

    char* mStart;
    char* mCurrent;
    int mCurrentLine = 1;
    char ERROR_UNEXPECTED_CHAR[50] = "ERROR: Unexpected character ";
    char ERROR_NO_END_QUOTE[50] = "ERROR: Expected \" but didn't find one ";
};
