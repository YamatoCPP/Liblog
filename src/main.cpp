#include <mutex>
#include <stdexcept>
#include <thread>
#include <optional>
#include <map>
#include <iostream>
#include <string>
#include <list>

#include <liblog/log.h>

auto ParseMessageImportance(const std::string&)    -> ymt::Log::MessageImportance;
auto InitLogByArgs(int, char*[])                   -> std::unique_ptr<ymt::Log>;

using MessageImportanceT = std::optional<ymt::Log::MessageImportance>;

auto main(int argc, char* argv[]) -> int {
    std::unique_ptr<ymt::Log> log;

    try {
        log = InitLogByArgs(argc, argv);
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    std::mutex mutex;
    auto WriteMessage = [&log, &mutex] 
        (const std::string& message, const MessageImportanceT messageImportance) -> void  {
        std::lock_guard<std::mutex> lg(mutex);

        if (messageImportance) {
            log->WriteMessage(message);
        }
        else {
            log->WriteMessage(message, *messageImportance);
        }
    }; 

    
    std::list<std::thread> threads;
    std::cout << "Please enter a blank message to end your entry." << std::endl;
    while (true) {
        std::string message;
        std::string messageImportanceText;

        std::cout << "Enter a message: ";
        std::getline(std::cin, message);
        
        if (message.empty()) {
            std::cout << "Empty message entered, input stopped" << std::endl;
            break;
        }
        
        std::cout << "Enter a message importance(low, medium, critical): ";
        std::getline(std::cin, messageImportanceText);
        
        try {
            MessageImportanceT msgImportance = std::nullopt;
            
            if (!messageImportanceText.empty()) {
                msgImportance = ParseMessageImportance(messageImportanceText);
            }

            std::thread tr(WriteMessage, message, msgImportance);    
            threads.push_back(std::move(tr));
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
        }   
    }

    for (auto& i : threads) {
        try { 
            i.join(); 
        }
        catch (const std::exception& ex) { 
            std::cerr << ex.what() << std::endl;
        }
    }
}

auto ParseMessageImportance(const std::string& messageImportance) -> ymt::Log::MessageImportance {
    static const std::map<std::string, ymt::Log::MessageImportance> importanceMap {
        { "low", ymt::Log::LOW },
        { "medium", ymt::Log::MEDIUM },
        { "critical", ymt::Log::CRITICAL }
    };
    
    const auto it = importanceMap.find(messageImportance);
    if (it == importanceMap.end()) {
        throw std::invalid_argument("Bad importance");        
    }

    return it->second;
}

auto InitLogByArgs(int argc, char* argv[]) -> std::unique_ptr<ymt::Log> {
    if (argc < 3) {
        throw std::invalid_argument{"Not enough argument"};
    }

    return std::make_unique<ymt::Log>(argv[1], ParseMessageImportance(argv[2]));
}
