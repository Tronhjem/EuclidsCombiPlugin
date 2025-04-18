#include "Logger.h"
#include <iostream>

void Logger::LogError(int line, std::string& message)
{
    std::cout << "Error: " << message << " at line: " << line << std::endl;
}

void Logger::LogWarning(int line, std::string& message)
{
    std::cout << "Warning: " << message << " at line: " << line << std::endl;
}

void Logger::LogMessage(std::string& message)
{
    std::cout << message << std::endl;
}