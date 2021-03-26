#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <vector>
#include <word.h>
#include <QString>

class Dictionary // класс словарь
{
private:
    QString dictName;
    std::vector<Word> words; // массив слов
    QString pathToDict; // путь к словарю
public:
    Dictionary();
    Dictionary(QString path);
    void insertWord(Word w);
    void deleteWord(Word w);
    void updateWords();
    void updateFile();
    long long findWord(Word w);
    long long getWordCount() const;
    void setWordCount(long long value);
    std::vector<Word> getWords() const;
    QString getPathToDict() const;
    QString getDictName() const;
};

#endif // DICTIONARY_H
