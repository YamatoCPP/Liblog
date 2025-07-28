#include <liblog/log.h>

#include <ctime>
#include <iomanip>
#include <string>
#include <fstream>

namespace ymt
{
    struct Log::Impl
    {
        std::ofstream logFile;
        MessageImportance defaultImportance;
    };

    Log::Log(const std::filesystem::path& filePath, MessageImportance defaultImportance) 
        : pimpl{new Impl{{filePath, std::ios::app | std::ios::ate}, defaultImportance}}
    {
        if (!pimpl->logFile.is_open())
            throw std::ios_base::failure("Unable to open file: " + std::string(filePath));
    }

    Log::~Log() = default;

    auto Log::PutCurrentTime() const
    {
        std::time_t tt = std::time(nullptr);
        return std::put_time(std::localtime(&tt), "%H:%M:%S");
    }

    auto Log::StringMessageImportance(MessageImportance messageImportance) const
    {
        std::string str;
        switch (messageImportance)
        {     
            case LOW:
                str = "Low message importance";
                break;
            case MEDIUM:
                str = "Medium message importance";
                break;
            case CRITICAL: 
                str = "Critical message importance";
                break;
        }
        return str;
    }

    auto Log::WriteMessage(std::string_view messageText, 
            MessageImportance messageImportance) const -> bool
    {
        /*
        * If the importance is less than the default importance, 
        * then false is returned without logging.
        */
        if (messageImportance < pimpl->defaultImportance)
            return false;

        std::string strMessageImportance{StringMessageImportance(messageImportance)};
        
        pimpl->logFile << std::left            << std::setw(29) 
                       << strMessageImportance << " | "
                       << PutCurrentTime()     << " | "
                       << messageText          << std::endl; 
        
        return pimpl->logFile.good();
    }
    
    auto Log::WriteMessage(std::string_view messageText) const -> bool
    {
        return WriteMessage(messageText, pimpl->defaultImportance);
    }

    auto Log::SetDefaultMessageImportance(MessageImportance messageImportance) -> void
    {
        pimpl->defaultImportance = messageImportance;
    }
}
