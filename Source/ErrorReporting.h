#pragma once

#include <string>

//TODO: make this take line and position as well for reporting properly syntax erros
class ErrorReporting
{
public:
    void LogError(int line, std::string& message);
    void LogError(std::string& message);
    void LogWarning(int line, std::string& message);
    void LogWarning(std::string& message);
    void LogMessage(std::string& message);
};
