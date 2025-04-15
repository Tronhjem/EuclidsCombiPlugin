#include "Logger.h"
#include <iostream>

void Logger::LogError(int line, const char* message)
{
    std::cout << "Error: " << message << " at line: " << line << std::endl;
}

void Logger::LogWarning(int line, const char* message)
{
    std::cout << "Warning: " << message << " at line: " << line << std::endl;
}

void Logger::LogMessage(const char* message)
{
    std::cout << message << std::endl;
}