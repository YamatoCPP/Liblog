#include <liblog/log.h>

#include <ctime>
#include <iomanip>
#include <fstream>
#include <stdexcept>

namespace ymt
{
    struct Log::Impl
    {
        std::ofstream logFile;
        MessageImportance defaultImportance;
    };

    Log::Log(const std::filesystem::path& filePath, MessageImportance defaultImportance)
        : pimpl{new Impl{filePath, defaultImportance}}
    {
        if (!pimpl->logFile.is_open())
            throw std::ios_base::failure("Unable to open file: " + std::string(filePath));
    }

    Log::~Log() = default;

    auto Log::PutCurrentTime() const noexcept
    {
        std::time_t tt = std::time(nullptr);
        return std::put_time(std::localtime(&tt), "%H:%M:%S");
    }

    auto Log::StringMessageImportance(MessageImportance messageImportance) const noexcept
    {
        switch (messageImportance)
        {     
            case LOW:      return "Low message importance";
            case MEDIUM:   return "Medium message importance";
            case CRITICAL: return "Critical message importance";
        }
        return "";
    }

    auto Log::WriteMessage(std::string_view messageText, 
            MessageImportance messageImportance) const -> void
    {
        if (messageImportance < pimpl->defaultImportance)
            throw std::invalid_argument{"message importance less than default"};

        auto strMessageImportance{StringMessageImportance(messageImportance)};
        pimpl->logFile << std::left            << std::setw(29) 
                       << strMessageImportance << " | "
                       << PutCurrentTime()     << " | "
                       << messageText          << std::endl;
        
        if (!pimpl->logFile.good())
            throw std::runtime_error{"message write error"};
    }

    auto Log::WriteMessage(std::string_view messageText) const -> void
    {
        WriteMessage(messageText, pimpl->defaultImportance);
    }

    auto Log::SetDefaultMessageImportance(MessageImportance messageImportance) noexcept -> void
    {
        pimpl->defaultImportance = messageImportance;
    }
}
