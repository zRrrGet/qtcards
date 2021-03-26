#include "profile.h"
#include <QSettings>

QString Profile::getUserName() const
{
    return userName;
}

Profile::Profile(QString name, QString path)
{
    if (path=="") pathToUserStats = "profiles/"+name+".config";
    else {
        this->pathToUserStats = path;
        QSettings stats(this->pathToUserStats,QSettings::IniFormat);
        this->userName = name;
        this->days = stats.value("days").toLongLong();

    }
}
