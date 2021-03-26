#ifndef WORD_H
#define WORD_H
#include <QString>


class Word // класс, содержит пару слов
{
private:
    QString first; // первое слово
    QString second; // второе слово
public:
    Word(QString f, QString s) : first(f), second(s) {
    }
    Word();
    ~Word(){}
    QString getFirst() const;
    QString getSecond() const;
};

#endif // WORD_H
