#ifndef LOG_HEADER
#define LOG_HEADER

#include <filesystem>
#include <string_view>
#include <memory>

namespace ymt
{
    class Log final
    {
    public:
        enum MessageImportance : char
        {
            LOW = 0,
            MEDIUM,
            CRITICAL
        };
        
        explicit Log(const std::filesystem::path&, MessageImportance);
        ~Log();
        
        auto SetDefaultMessageImportance(MessageImportance)             -> void;
        auto WriteMessage(std::string_view, MessageImportance)  const   -> bool; 
        auto WriteMessage(std::string_view)                     const   -> bool;  
  
    private:
        auto PutCurrentTime()                           const; 
        auto StringMessageImportance(MessageImportance) const;
    
    private:
        struct Impl;
        std::unique_ptr<Impl> pimpl;
    };
}
#endif // ifndef LOG_HEADER
