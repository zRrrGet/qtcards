#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uiClasses/profilesettings.h"
#include "uiClasses/dictionarysettings.h"
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    currentProfile = nullptr;
    currentDictionary = nullptr;


    ui->setupUi(this);



    connect(ui->optionProfile, SIGNAL(triggered()), this, SLOT(optionProfileTriggered()));
    connect(ui->optionDict, SIGNAL(triggered()), this, SLOT(optionDictionaryTriggered()));
    connect(ui->optionExit, SIGNAL(triggered()), this, SLOT(optionExitTriggered()));                // сигналы для опций, что в настройках
    connect(ui->optionSetCount, SIGNAL(triggered()), this, SLOT(optionSetCountTriggered()));
    connect(ui->optionProfileExit, SIGNAL(triggered()), this, SLOT(optionProfileExitTriggered()));



    ui->buttonStart->setEnabled(false);
    ui->editAns->setVisible(false);
    ui->labelWord->setVisible(false);               // прячем элементы для сессии
    ui->labelCorrect->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::optionProfileTriggered()
{
    profileSettings *w = new profileSettings;
    w->setModal(true);                              // по нажатию опции Профиль запускаем настройку профилей
    w->exec();


    currentProfile = w->getProfile();           // получаем выбранный профиль

    w->deleteLater();


    if (currentProfile) ui->labelWelcome->setText("Добро пожаловать, " + currentProfile->getUserName());        // устанавливаем текст для выбранного профиля
    if (currentProfile&&currentDictionary) ui->buttonStart->setEnabled(true);       // если профиль и словарь установлены, то разрешаем нажать на кнопку старт


}

void MainWindow::optionDictionaryTriggered()
{
    dictionarySettings *w = new dictionarySettings(this, currentProfile);
    w->setModal(true);
    w->exec();

    currentDictionary = w->getCurrentDict();                            // тут аналогично опции Профиль

    w->deleteLater();

    if (currentDictionary) {

    if (currentDictionary) ui->labelStats->setText("Выбран словарь: " + currentDictionary->getDictName() + "\nКоличество пар: " + QString::number(currentDictionary->getWords().size()));
    if (currentProfile&&currentDictionary) ui->buttonStart->setEnabled(true);
    }
}

void MainWindow::optionExitTriggered()
{
    this->close();
}

void MainWindow::optionSetCountTriggered()
{
    sessionWordCount = QInputDialog::getInt(this, "Введите число", "Количество слов за сессию:", sessionWordCount, 0); // спрашиваем у юзера сколько давать ему слов на сессию
}

void MainWindow::optionProfileExitTriggered()   // выход из профиля
{
    delete currentProfile;
    ui->labelWelcome->setText("Выберите профиль в настройках");
    currentProfile = nullptr;
}


void MainWindow::on_buttonStart_clicked()
{
    QSettings sets("profiles/"+currentProfile->getUserName()+".config", QSettings::IniFormat);
    sets.beginGroup(currentDictionary->getDictName());

    if (ui->buttonStart->text()!="Продолжить"){ // опять же, нахождение в сессии определяется текстом на кнопке, по нажатию текст меняется


        for (int i = 0;i<6;++i) {   // формируем массив в котором будут слова для проверки, первыми идут слова, которые не имеют стадии(0), дальше 1 и т.д, т.к в цикле i<6, то слова с 6 стадией не будут показываться пользователю
            for (size_t j = 0;j < currentDictionary->getWords().size() && sessionWords.size()<sessionWordCount; ++j) {
                QString key = (currentDictionary->getWords()[j].getFirst()+";"+currentDictionary->getWords()[j].getSecond());
                key.replace(" ","_");   // стадии слов для словаря записываются в конфиге в группе [имя словаря], далее в группе идут строки value=1-6, где value это склееные первое и второе слово из пары(вместе с ;)
                if (!i&&!sets.allKeys().contains(key)) sessionWords.push_back(Word(currentDictionary->getWords()[j].getFirst(),
                                                                                   currentDictionary->getWords()[j].getSecond()));
                else if (sets.value(key)==i){
                    sessionWords.push_back(Word(currentDictionary->getWords()[j].getFirst(),
                                                currentDictionary->getWords()[j].getSecond()));
                }
            }
        }


        if (sessionWords.size()==0) {
            QMessageBox m;
            m.information(this,"Поздравляем","Вы выучили всё из данного словаря! Вперед, к новым свершениям!");
            return;
        }


        correctWordCount = 0;
        ui->buttonStart->setText("Продолжить");
        ui->labelStats->setVisible(false);
        ui->labelWelcome->setVisible(false);                        // включаем элементы для сессии и выключаем заглавные элементы
        ui->labelWord->setVisible(true);
        ui->editAns->setVisible(true);

        wordIndex = 0;

        currentWord = sessionWords[wordIndex]; // текущее слово
        ui->labelWord->setText(currentWord.getFirst()); // пишем первое слово из пары
    }

    else {
        if (!(ui->labelCorrect->isVisible())){  // если лейбл с "Правильно" или "Неправильно" не видно, значит сверяем введенный ответ

            ui->labelCorrect->setVisible(true);

            QString key = (currentWord.getFirst()+";"+currentWord.getSecond());
            key.replace(" ","_");

            //
            // при сверке ответа мы не учитываем регистр, а ответ считается правильным, если на каждые 6 символов не более 1 ошибки, т.е мелкие помарки можно засчитать
            if (abs(QString::compare(currentWord.getSecond(),ui->editAns->text(), Qt::CaseInsensitive)) <= currentWord.getSecond().length()/6) {

                ui->labelCorrect->setText("Правильно");
                ui->labelCorrect->setStyleSheet("color: green");

                ++correctWordCount;

                if (sets.allKeys().contains(key)) sets.setValue(key,sets.value(key).toInt()+1); // увеличиваем стадию пары в конфиге
                else sets.setValue(key,1);                                                      // если пара не значится, то создаем ее в конфиге

            }

            else {

                ui->labelCorrect->setText("Неправильно");
                ui->labelCorrect->setStyleSheet("color: red");
                if (sets.allKeys().contains(key)&&sets.value(key).toInt()-1>0) sets.setValue(key,sets.value(key).toInt()-1); // аналогично, уменьшаем стадию
                else if (sets.allKeys().contains(key)) sets.remove(key);

            }

            ui->labelWord->setText(ui->labelWord->text()+" - "+currentWord.getSecond()); // показываем правильный ответ

            ++wordIndex;    // следующий индекс

            ui->editAns->setEnabled(false);
        }

        else if (wordIndex < sessionWordCount && wordIndex<sessionWords.size()){    // если слова еще остались и не лимит на слова на сессию не вышел, то даем следующее слово

            ui->labelCorrect->setVisible(false);
            currentWord = sessionWords[wordIndex];
            ui->labelWord->setText(currentWord.getFirst());
            ui->editAns->clear();
            ui->editAns->setEnabled(true);

        }


        else {  // здесь выполняется код, когда слова из массива слов на сессию закончились, соответственно сессия тоже
            sets.endGroup();
            sets.beginGroup("Stats");

            if (sets.allKeys().contains("lastday")&&sets.value("lastday").toDate()!=QDate().currentDate()) {
                sets.setValue("days", sets.value("days").toInt()+1);
                sets.setValue("lastday",QDate().currentDate());
            }                                                           // статистика по дням, записывается последний день, когда завершалась сессия, и прибавляется 1 день в статистике
            else if (!(sets.allKeys().contains("lastday"))) {
                sets.setValue("lastday", QDate().currentDate());
                sets.setValue("days", sets.value("days").toInt()+1);;
            }


            sets.endGroup();
            sets.beginGroup(currentDictionary->getDictName());

            long long remain = 6*currentDictionary->getWords().size(); // для статистики, оставшиеся слова в словаре, с учетом стадий

            QStringList l = sets.allKeys();
            foreach(QString key, l)                     // вычитаем стадии слов из общего кол-ва
                remain-=sets.value(key).toInt();
            sets.endGroup();

            sets.beginGroup("Stats");



            // статистика выводится в конце сессии в отдельном окне
            QMessageBox m;
            m.information(this,"Отлично!","Вы закончили сессию.\n"
                                          "За данную сессию вы:\n"
                                          "- Написали перевод к "+QString::number(sessionWordCount<currentDictionary->getWords().size()?sessionWordCount:currentDictionary->getWords().size()) + " словам!\n"
                                          "- Правильных переводов: " + QString::number(correctWordCount)+"\n"
                                          "- Неправильных: "+ QString::number((sessionWordCount<currentDictionary->getWords().size()?sessionWordCount:currentDictionary->getWords().size())-correctWordCount)+"\n"
                                          "- Вы занимаетесь: " + sets.value("days").toString() +" дней\n"
                                          "Осталось слов: "+QString::number(remain)+" ("+QString::number(double(remain)/(6*currentDictionary->getWords().size())*100, 'f', 3)+ "%)"+
                                            "\nПродолжайте в том же духе!");


            ui->labelStats->setVisible(true);
            ui->labelWelcome->setVisible(true);
            ui->labelWord->setVisible(false);
            ui->editAns->setVisible(false);
            ui->labelCorrect->setVisible(false);
            ui->editAns->clear();
            ui->editAns->setEnabled(true);

            ui->buttonStart->setText("Начать сессию");  //
            sessionWords.clear();


        }

    }

}


void MainWindow::closeEvent(QCloseEvent *event) // обработка преждевременного выхода
{
    if (ui->buttonStart->text() == "Продолжить") {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Выход", "Вы действительно хотите выйти? Несохраненные данные могут быть потеряны.", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) event->accept();
        else event->ignore();
    }
}

void MainWindow::on_editAns_returnPressed()
{
    on_buttonStart_clicked();
}
