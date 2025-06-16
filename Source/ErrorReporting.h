#pragma once

#include <string>
#include <vector>

enum class EntryType { Messasge, Warning, Error };

struct LogEntry
{
    EntryType mEntryType;
    int mLine;
    std::string mMessage;
};

//TODO: make this take line and position as well for reporting properly syntax erros
class ErrorReporting
{
public:
    void LogError(int line, std::string& message);
    void LogError(std::string& message);
    void LogWarning(int line, std::string& message);
    void LogWarning(std::string& message);
    void LogMessage(std::string& message);
    void Clear();
    std::vector<LogEntry>& GetErrors() {return mLogEntries; } 
    
private:
    std::vector<LogEntry> mLogEntries;
};
