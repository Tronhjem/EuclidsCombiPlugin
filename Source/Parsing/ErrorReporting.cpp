#include "ErrorReporting.h"
#include <iostream>

void ErrorReporting::LogError(int line, std::string& message)
{
    std::cout << "[Error] " << message << " at line: " << line << std::endl;
}

void ErrorReporting::LogError(std::string& message)
{
    std::cout << "[Error] " << message << std::endl;
}

void ErrorReporting::LogWarning(int line, std::string &message)
{
    std::cout << "[Warning] " << message << " at line: " << line << std::endl;
}

void ErrorReporting::LogWarning(std::string &message)
{
    std::cout << "[Warning] " << message << std::endl;
}

void ErrorReporting::LogMessage(std::string& message)
{
    std::cout << message << std::endl;
}