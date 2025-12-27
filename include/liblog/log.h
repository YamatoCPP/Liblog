#ifndef LOG_HEADER
#define LOG_HEADER

#include <filesystem>
#include <string_view>
#include <memory>

namespace ymt {

class Log final {
public:
    enum MessageImportance : char {
        LOW = 0,
        MEDIUM,
        CRITICAL
    };
    
    Log(const std::filesystem::path&, MessageImportance);
    
    Log(const Log&) = delete;
    Log(Log&&);
    Log& operator=(const Log&) = delete;
    Log& operator=(Log&&);
    
    ~Log();
    
    auto SetDefaultMessageImportance(MessageImportance)    noexcept -> void;
    auto WriteMessage(std::string_view, MessageImportance) const    -> void; 
    auto WriteMessage(std::string_view)                    const    -> void;  

private:
    auto PutCurrentTime()                           const noexcept; 
    auto StringMessageImportance(MessageImportance) const noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

} // namespace ymt
#endif // ifndef LOG_HEADER
