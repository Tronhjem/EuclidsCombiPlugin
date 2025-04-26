#pragma once

#include <string>

class ErrorReporting
{
public:
    void LogError(int line, std::string& message);
    void LogError(std::string& message);
    void LogWarning(int line, std::string& message);
    void LogWarning(std::string& message);
    void LogMessage(std::string& message);
};
