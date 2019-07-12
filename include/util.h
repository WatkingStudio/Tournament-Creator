#ifndef UTIL_H
#define UTIL_H

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <QString>
#include <QFileInfo>
#include <QDir>

namespace util {

void writeToFile(const std::string &file = "default.txt");

std::string getWorkingDirectory();

}

void utilLog(const std::string &message);
void utilLog(const int &message);
void utilDebug(const std::string &message);
QString intToQString(int num);

static bool debugging = false;
void setDebugging(bool debug);
bool fileExists(const QString &path);
bool createFile(const QString &path, const QString &directory);

typedef unsigned int uint;
typedef unsigned char uchar;


#endif // UTIL_H
