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
    char buffer [80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d_%m_%Y", timeinfo);
    std::string str(buffer);

    return str;
}

std::string getTime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%I:%M:%S", timeinfo);
    std::string str(buffer);

    return str;
}

//example message- [int][24/07/28][12:38] Log Message: 80
void utilLog(const int &message)
{
    std::string logMessage = "[int]";
    logMessage += "[";
    logMessage += getDate();
    logMessage += "]";
    logMessage += " LogMessage: ";
    logMessage += std::to_string(message);

    std::cout<<logMessage<<std::endl;
    saveLog(logMessage);
}

//example message- [string][24/07/28][12:38] Log Message: This is a log message
void utilLog(const std::string &message)
{
    std::string logMessage = "[string]";
    logMessage += "[";
    logMessage += getDate();
    logMessage += "]";
    logMessage += " LogMessage: ";
    logMessage += message;

    std::cout<<logMessage<<std::endl;
    saveLog(logMessage);
}

void utilDebug(const std::string &message)
{
    if(debugging)
    {
        std::string logMessage = "[string]";
        logMessage += "[";
        logMessage += getDate();
        logMessage += "]";
        logMessage += " DebugMessage: ";
        logMessage += message;

        std::cout<<logMessage<<std::endl;
        saveLog(logMessage);
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

bool fileExists(const QString &path)
{
    QFileInfo checkFile(path);
    //check if file exists and if yes: Is it really a file and no directory?
    if(checkFile.exists() && checkFile.isFile())
        return true;
    else
        return false;
}

bool createFile(const QString &path, const QString &directory)
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        if(!QDir(directory).exists())
        {
            QDir().mkdir(directory);
        }
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            utilLog("ERROR: Unable to create file or directory");
            return false;
        }
    }
    return true;
}

void saveLog(const std::string &message)
{
    std::string file = "Logs/log_" + getDate() + ".txt";
    if(!fileExists(file.c_str()))
        createFile(file.c_str(), "Logs");

    QFile logFile(file.c_str());
    logFile.open(QIODevice::ReadWrite | QIODevice::Text);
    QByteArray data = logFile.readAll();
    std::string array = "[" + getTime() + "]" + message + "\n";
    logFile.write(array.c_str());
    logFile.close();
}
