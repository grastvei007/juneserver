#include "util.h"

#include <QDir>

namespace util {

QString configDirPath(const QString &appName)
{
    QString path = QDir::homePath() + QDir::separator() + ".config"
           + QDir::separator() + "june"
           + QDir::separator() + appName;

    if(QDir dir(path); !dir.exists())
        QDir().mkpath(path);
    return path;
}

}// end namespace
