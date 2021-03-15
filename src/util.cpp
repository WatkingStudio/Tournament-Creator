#include "include/util.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <direct.h>

namespace util{

    std::string GetWorkingDirectory()
    {
        char* cwd = _getcwd(0,0);
        std::string working_directory(cwd);
        std::free(cwd);
        return working_directory;
    }

    void WriteToFile(const std::string &message, const std::string &file, const std::string &directory){
        if(!FileExists(file.c_str()))
        {
            CreateFile(file.c_str(), directory.c_str());
        }

        QFile writeToFile(file.c_str());
        writeToFile.open(QIODevice::ReadWrite | QIODevice::Text);
        QByteArray data = writeToFile.readAll();
        writeToFile.write(message.c_str());
        writeToFile.close();
    }

}

bool CreateFile(const QString &path, const QString &directory)
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
            UtilLog("ERROR: Unable to create file or directory");
            return false;
        }
    }
    return true;
}

bool FileExists(const QString &path)
{
    QFileInfo checkFile(path);
    //check if file exists and if yes: Is it really a file and not a directory?
    if(checkFile.exists()
            && checkFile.isFile())
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::string GetDate()
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

std::string GetTime()
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

QString IntToQString(const int num)
{
    return QString::fromStdString(std::to_string(num));
}

void SaveDebug(const std::string &message)
{
    std::string file = "DebugLogs/debug_" + GetDate() + ".txt";
    std::string array = "[" + GetTime() + "]" + message + "\n";
    util::WriteToFile(array, file, "DebugLogs");
}

void SaveLog(const std::string &message)
{
    std::string file = "Logs/log_" + GetDate() + ".txt";
    std::string array = "[" + GetTime() + "]" + message + "\n";
    util::WriteToFile(array, file, "Logs");
}

//example message- [string][24/07/28][12:38:15] Debug Message: This is a log message
void UtilDebug(const std::string &message)
{
    if(Debugging)
    {
        std::string logMessage = "[string]";
        logMessage += "[";
        logMessage += GetDate();
        logMessage += "]";
        logMessage += "[";
        logMessage += GetTime();
        logMessage += "]";
        logMessage += " DebugMessage: ";
        logMessage += message;

        std::cout<<logMessage<<std::endl;
        SaveDebug(logMessage);
    }
}

//example message- [float][24/07/28][12:38:15] Log Message: 80.5
void UtilLog(const float &message)
{
    std::string logMessage = "[float]";
    logMessage += "[";
    logMessage += GetDate();
    logMessage += "]";
    logMessage += "[";
    logMessage += GetTime();
    logMessage += "]";
    logMessage += " LogMessage: ";
    logMessage += std::to_string(message);

    std::cout<<logMessage<<std::endl;
    SaveLog(logMessage);
}

//example message- [string][24/07/28][12:38:15] Log Message: This is a log message
void UtilLog(const std::string &message)
{
    std::string logMessage = "[string]";
    logMessage += "[";
    logMessage += GetDate();
    logMessage += "]";
    logMessage += "[";
    logMessage += GetTime();
    logMessage += "]";
    logMessage += " LogMessage: ";
    logMessage += message;

    std::cout<<logMessage<<std::endl;
    SaveLog(logMessage);
}
