#include <liblog/log.h>

namespace ymt
{
    Log::Log(const std::string& filePath, MessageImportance defaultImportance) 
        : logFile(filePath, std::ios_base::app | std::ios_base::ate), defaultImportance(defaultImportance) 
    {
        // if logFile not open throw exception
        if (!logFile.is_open())
            throw std::ios_base::failure("Unable to open file: " + filePath);
    }

    auto Log::StringCurrentTime() const
    {
        auto messageWriteTime = std::chrono::system_clock::now();
        auto tt = std::chrono::system_clock::to_time_t(messageWriteTime);

        return std::put_time(std::localtime(&tt), "%H-%M-%S");
    }

    bool Log::WriteMessage(const std::string& messageText, 
            MessageImportance messageImportance) const
    {
        /*
        * If the importance is less than the default importance, 
        * then false is returned without logging.
        */
        if (messageImportance < defaultImportance)
            return false;

        logFile << messageText << ' '
                << StringMessageImportance(messageImportance) << ' '
                << StringCurrentTime() << '\n'
                << std::endl;
        
        return logFile.good();
    }
    
    bool Log::WriteMessage(const std::string& messageText) const
    {
        return WriteMessage(messageText, defaultImportance);
    }

    void Log::SetDefaultMessageImportance(MessageImportance messageImportance)
    {
        defaultImportance = messageImportance;
    }

    std::string Log::StringMessageImportance(MessageImportance messageImportance) const
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
    

}
