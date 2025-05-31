#ifndef UTIL_H
#define UTIL_H

#include <QString>

namespace util {

// return path to config dir. If it does not exist it is created.
QString configDirPath(const QString& appName);

}// end namespace

#endif // UTIL_H
