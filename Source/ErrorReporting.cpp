#include <iostream>

#include "ErrorReporting.h"

void ErrorReporting::LogError(int line, std::string& message)
{
    message = message + " at line " + std::to_string(line) + "\n";
    mLogEntries.emplace_back(LogEntry{EntryType::Error, line, message});
    std::cout << message << std::endl;
}

void ErrorReporting::LogError(std::string& message)
{
    mLogEntries.emplace_back(LogEntry{EntryType::Error, 0, message + "\n"});
    std::cout << message << std::endl;
}

void ErrorReporting::LogWarning(int line, std::string &message)
{
    message = message + " at line " + std::to_string(line) + "\n";
    mLogEntries.emplace_back(LogEntry{EntryType::Warning, line, message});
    std::cout << message << std::endl;
}

void ErrorReporting::LogWarning(std::string& message)
{
    mLogEntries.emplace_back(LogEntry{EntryType::Warning, 0, message + "\n"});
    std::cout << message << std::endl;
}

void ErrorReporting::LogMessage(std::string& message)
{
    mLogEntries.emplace_back(LogEntry{EntryType::Messasge, 0, message + "\n"});
    std::cout << message << std::endl;
}

void ErrorReporting::Clear()
{
    mLogEntries.clear();
}
