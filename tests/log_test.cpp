#include "liblog/Log.h"
#include <iostream>

void test1()
{
    ymt::Log log("logfile.txt", ymt::Log::LOW);
    log.WriteMessage("First Test");
    log.WriteMessage("Recorder message", ymt::Log::MEDIUM);
    
    log.SetDefaultMessageImportance(ymt::Log::CRITICAL);
    log.WriteMessage("Not recorder message", ymt::Log::LOW); 
    /* 
     * The logfile.txt file 
     * should only contain the first and second messages.
     */

}

void test2()
{
    ymt::Log log("logfile.txt", ymt::Log::CRITICAL);
    log.WriteMessage("Second Test"); // CRITICAL message
    log.SetDefaultMessageImportance(ymt::Log::LOW);
    log.WriteMessage("Message with default importance"); // LOW message
    /*
     * The logfile.txt file
     * should contain two messages, one of low importance and the other of high importance.
     */
} 


int main()
{
    test1();
    test2();
    return 0;
}
