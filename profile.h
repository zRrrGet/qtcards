#ifndef PROFILE_H
#define PROFILE_H
#include <vector>
#include <QString>

class Profile // класс профиль
{
private:
    QString userName;
    QString pathToUserStats; // путь к статистике по пользователю
    std::vector<QString> dictAccesses; // cловари к которым пользователь имеет доступ
    long long days; // количество дней
public:
    Profile(QString name, QString path = "");
    QString getUserName() const;
};

#endif // PROFILE_H
