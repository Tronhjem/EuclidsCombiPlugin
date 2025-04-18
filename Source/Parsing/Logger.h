#pragma once
#include <string>

class Logger
{
public:
    void LogError(int line, std::string& message);
    void LogWarning(int line, std::string& message);
    void LogMessage(std::string& message);
};