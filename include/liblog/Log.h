#ifndef LOG_HEADER
#define LOG_HEADER

#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>

#endif 

namespace ymt
{
    class Log 
    {
    public:
        enum MessageImportance
        {
            LOW = 0,
            MEDIUM,
            CRITICAL
        };
        
        Log(const std::string&, MessageImportance);

        constexpr void SetDefaultMessageImportance(MessageImportance); 
        bool WriteMessage(const std::string&, MessageImportance) const;
        bool WriteMessage(const std::string&) const;
    private:
        auto StringCurrentTime() const;
        std::string StringMessageImportance(MessageImportance) const;
        
        mutable std::ofstream logFile;
        MessageImportance defaultImportance;
    };
}
