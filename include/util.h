#ifndef UTIL_H
#define UTIL_H

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>

namespace util {

// Gets the current working directory.
// @return - a string holding the working directory.
std::string GetWorkingDirectory();

// Writes a message to the end of specified file.
void WriteToFile(const std::string &message, const std::string &file = "default.txt", const std::string &directory = "Data");

}

// Creates a file.
// @return - returns true if the file was created, false if it was unable to.
bool CreateFile(const QString &path, const QString &directory);

// Checks to see if a file exists.
// @return - returns true if the file exists, false if the file doesn't exist.
bool FileExists(const QString &path);

// Converts an integer into a QString.
// @return - The integer in a QString form.
QString IntToQString(const int num);

// Saves a message in the Debug Log file.
void SaveDebug(const std::string &message);

// Saves a message in the Log file.
void SaveLog(const std::string &message);

// Logs the message as a debug log.
void UtilDebug(const std::string &message);

// Logs the string message as a log.
void UtilLog(const std::string &message);

// Logs the float as a log.
void UtilLog(const float &message);

const bool Debugging = true;
typedef unsigned char uchar;
typedef unsigned int uint;


#endif // UTIL_H
