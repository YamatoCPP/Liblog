#include <mutex>
#include <optional>
#include <stdexcept>
#include <thread>
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <liblog/log.h>

auto ParseMessageImportance(const char*)    -> ymt::Log::MessageImportance;
auto InitLogByArgs(int, char*[])            -> std::optional<ymt::Log>;

int main(int argc, char* argv[])
{
    auto log = InitLogByArgs(argc, argv);
    if (!log) 
        return 1; 

    std::mutex mutex;
    auto WriteMessage = [&log, &mutex] 
        (std::string message, std::string messageImportanceText) 
    {
        ymt::Log::MessageImportance messageImportance;
        
        try 
        {
            if (!messageImportanceText.empty())
                messageImportance = ParseMessageImportance
                    (messageImportanceText.c_str());
        }
        catch (const std::exception& ex)
        {
            /*
            * std::cerr is commented out because there were output bugs. 
            * Error output should be uncommented and redirected to a file.
            * 
            * std::cerr << ex.what() << std::endl;
            */
            return;
        }
        
        /*
         * I use std::mutex because 
         * different threads should not access the same data at the same time
         */
        std::lock_guard<std::mutex> lg(mutex);
        if (messageImportanceText.empty())
            log.value().WriteMessage(message);
        else
            log.value().WriteMessage(message, messageImportance);
    }; 

    std::string message;
    std::string messageImportanceText;
    std::vector<std::thread> threadVector;
    
    std::cout << "Please enter a blank message to end your entry." << std::endl;
    try
    {
        while (true)
        {
            std::cout << "Enter a message: ";
            std::getline(std::cin, message);
            
            if (message.empty())
            {
                std::cout << "Empty message entered, input stopped" << std::endl;
                break;
            }
            
            std::cout << "Enter a message importance(low, medium, critical): ";
            std::getline(std::cin, messageImportanceText);

            std::thread tr(WriteMessage, std::move(message), std::move(messageImportanceText));    
            threadVector.push_back(std::move(tr));
        }

        for (auto& i : threadVector)
            i.join();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}

auto ParseMessageImportance(const char* messageImportance) 
    -> ymt::Log::MessageImportance
{
    static const std::map<std::string, ymt::Log::MessageImportance> importanceMap
    {
        { "low", ymt::Log::LOW },
        { "medium", ymt::Log::MEDIUM },
        { "critical", ymt::Log::CRITICAL }
    };
    
    if (importanceMap.find(messageImportance) == importanceMap.end())
        throw std::invalid_argument("Bad importance");        
   
    return importanceMap.at(messageImportance);
}

auto InitLogByArgs(int argc, char* argv[]) -> std::optional<ymt::Log>
{
    if (argc < 3)
    {
        std::cerr << "Not enough arguments" << std::endl;
        return std::nullopt;
    }

    try
    {
        return std::make_optional<ymt::Log>(argv[1], 
                ParseMessageImportance(argv[2]));
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return std::nullopt;
    }
}
