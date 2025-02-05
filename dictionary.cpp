#include "dictionary.h"
#include <fstream>
#include <string>
#include <QString>
#include <QFile>
#include <iostream>


Dictionary::Dictionary(QString path)
{
    pathToDict = path;
    path.remove("dictionaries/");
    path.remove(".dict");
    dictName = path;
    updateWords();
}
void Dictionary::insertWord(Word w)         // функция для вставки слова
{
    std::fstream dictFile(pathToDict.toStdString(), std::ios::app);
    dictFile << w.getFirst().toStdString() << ";" << w.getSecond().toStdString() << ";"; // разделителем будет ;
    dictFile.close();
    words.push_back(w);
}

void Dictionary::deleteWord(Word w) // если слово есть в словаре то удаляем его из массива и обновляем файл словаря в соответствии с массивом
{
    if (findWord(w)!=-1) {
        words.erase(words.begin()+findWord(w));
        updateFile();
    }
}



void Dictionary::updateWords()
{
    std::string f,s;
    words.clear(); // чистим массив со словами с словаре
    std::fstream dictFile(pathToDict.toStdString(), std::ios::in);
    while (std::getline(dictFile,f,';')) {                                          // считываем по 2 слова и вставляем в массив слов
        std::getline(dictFile,s,';');
        words.push_back(Word(QString::fromStdString(f),QString::fromStdString(s)));

    }
    dictFile.close();
}

void Dictionary::updateFile()               // перезаписываем файл, добавляя туда слова из массива слов, с разделителем ; для каждого слова из пары
{
    std::fstream dictFile(pathToDict.toStdString(), std::ios::out);
    for (size_t i = 0; i < words.size();++i)
        dictFile << words[i].getFirst().toStdString() << ";" << words[i].getSecond().toStdString() << ";";
    dictFile.close();
}

long long Dictionary::findWord(Word w)      // находим индекс слова в словаре, -1 если такого нет
{
    for (size_t i = 0;i < words.size();++i)
        if (w.getFirst()==words[i].getFirst()&&w.getSecond()==words[i].getSecond()) return i;
    return -1;
}

std::vector<Word> Dictionary::getWords() const
{
    return words;
}

QString Dictionary::getPathToDict() const
{
    return pathToDict;
}

QString Dictionary::getDictName() const
{
    return dictName;
}

Dictionary::Dictionary()
{

}


