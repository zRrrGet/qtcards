#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "profile.h"
#include "dictionary.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    long long sessionWordCount = 10; // количество слов на сессию
    Profile *currentProfile; // текущий профиль
    Dictionary *currentDictionary; // текущий словарь
    QList<Word> sessionWords; // слова для сессии
    long long wordIndex = 0; // индекс текущего слова из массива слов для сессии
    Word currentWord; // текущее слово
    long long correctWordCount=0; // количество правильных ответов
protected:
    void closeEvent(QCloseEvent *event); // обработка выхода из программы во время сессии
public slots:
    void optionProfileTriggered();
    void optionDictionaryTriggered();
    void optionExitTriggered();
    void optionSetCountTriggered();
    void optionProfileExitTriggered();

private slots:
    void on_buttonStart_clicked();
    void on_editAns_returnPressed();
};
#endif // MAINWINDOW_H
