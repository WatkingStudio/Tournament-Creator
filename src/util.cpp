#include "include/util.h"
#include <iostream>
#include <ctime>
#include <iomanip>

namespace util{

void writeToFile(const std::string &file){

}

std::string getWorkingDirectory()
{
    std::cout<<"This path has been hard-coded"<<std::endl;
    return "C:/Users/gethi/Documents/Coding/Repositories/tournament_creator/";
}

}

std::string getDate()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "[%d-%m-%Y] [%I:%M:%S]", timeinfo);
    std::string str(buffer);

    return str;
}

//example message- [int][24/07/28][12:38] Log Message: 80
void utilLog(const int &message)
{
    std::string logMessage = "[int]";
    logMessage += getDate();
    logMessage += " LogMessage: ";
    logMessage += std::to_string(message);

    std::cout<<logMessage<<std::endl;
}

//example message- [string][24/07/28][12:38] Log Message: This is a log message
void utilLog(const std::string &message)
{
    std::string logMessage = "[string]";
    logMessage += getDate();
    logMessage += " LogMessage: ";
    logMessage += message;

    std::cout<<logMessage<<std::endl;
}

void utilDebug(const std::string &message)
{
    if(debugging)
    {
        std::string logMessage = "[string]";
        logMessage += getDate();
        logMessage += " DebugMessage: ";
        logMessage += message;

        std::cout<<logMessage<<std::endl;
    }
}

void setDebugging(bool debug)
{
    debugging = debug;
}

QString intToQString(int num)
{
    return QString::fromStdString(std::to_string(num));
}
