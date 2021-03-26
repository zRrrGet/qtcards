#include "dictionarysettings.h"
#include "ui_dictionarysettings.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QSettings>
#include <QDir>
#include <dictionary.h>
#include <QDebug>
#include "profile.h"
#include "changedictionary.h"

dictionarySettings::dictionarySettings(QWidget *parent, Profile *p) :
    QDialog(parent),
    ui(new Ui::dictionarySettings)
{
    ui->setupUi(this);

    currentDict = nullptr;


    ui->comboProfile->addItem("Не указан");


    QStringList conf = QDir("profiles").entryList(QStringList() << "*.config",QDir::Files);     //каждый .config файл в папке profiles это профиль
    foreach(QString fname, conf)                                                                // добавляем в таблицу все профили
        ui->comboProfile->addItem(fname.remove(".config"));


    if (p) {
        ui->comboProfile->setCurrentText(p->getUserName());         // если профиль уже выбран, то автоматически выбираем его
        ui->comboProfile->setEnabled(false);                        //и убираем возможность менять его
    }


    updateList(); // обновляем список(если профиль уже выбран, то будут показываться только доступные профилю словари)
}

dictionarySettings::~dictionarySettings()
{
    delete ui;
}

Dictionary *dictionarySettings::getCurrentDict()
{
    return currentDict;
}

void dictionarySettings::on_buttonAdd_clicked()
{
    bool isSuccess; // пользователь нажал ОК или Отмена
    QString name = QInputDialog::getText(this, "Ввод имени словаря", "Имя словаря:", QLineEdit::Normal, "", &isSuccess);
    // получаем имя профиля от юзера


    bool existance = false;
    QStringList dict = QDir("dictionaries").entryList(QStringList() << "*.dict",QDir::Files);
    foreach(QString fname, dict)                                // проверка на существущее имя пользователя, записываем результат в existance
        if(fname.remove(".dict")==name) existance=true;

    if (name==""&&isSuccess) {
        QMessageBox messageBox;
        messageBox.critical(this,"Ошибка!","Имя не задано");        // если пользователь ничего не ввел
    }


    else if (!existance&&isSuccess) {
        QDir().mkdir("dictionaries");
        QFile fileDict("dictionaries/"+name+".dict");         // создаем папку(если ее нет)

        fileDict.open(QIODevice::WriteOnly);                    // и файл(создается при открытии потока)
        fileDict.close();

        if (ui->comboProfile->currentText()!="Не указано"){
            QSettings sets("profiles/"+ui->comboProfile->currentText()+".config",QSettings::IniFormat);  // открываем .config
            sets.beginGroup("Accesses");    // [Accesses]
            sets.setValue(name,true);       // устанавливаем доступ к словарю выбранному пользователю
            sets.endGroup();
        }

    }
    else if (isSuccess){
        QMessageBox messageBox;
        messageBox.critical(this,"Ошибка!","Такой словарь уже существует, введите другое имя словаря.");
    }
    updateList(); // обновляем список
}

void dictionarySettings::updateList() { // ищет и добавляет в таблицу файлы .dict

    ui->listDictionary->clear(); // очищаем лист

    QStringList keys, values;
    if (ui->comboProfile->currentText()!="Не указан"){ // здесь как и в других частях кода часто используется в качестве проверки текст, поэтому при переводе чего-либо лучше использовать ctrl+f и "заменить на", чтобы заменить все вхождения

        QSettings sets("profiles/"+ui->comboProfile->currentText()+".config", QSettings::IniFormat); // открываем конфиг
        sets.beginGroup("Accesses");
        keys = sets.allKeys();                                                              // определяем массивы ключей и значений
        for (int i = 0; i < keys.size();++i)
            values.push_back(sets.value(keys[i]).toString());

    }


    QStringList dict = QDir("dictionaries").entryList(QStringList() << "*.dict",QDir::Files);
    foreach(QString fname, dict) {
        bool access = true, accessExist=false;
        for(int i = 0;i < keys.size();++i) {
            if (values[i] == "false" && keys[i] == fname.remove(".dict")) access = false;                   // здесь мы вставляем в таблицу все разрешенные словари
            if (keys[i]==fname.remove(".dict")) accessExist=true;
        }
        if ((ui->comboProfile->currentText()=="Не указан") || (access&&accessExist)) ui->listDictionary->addItem(fname.remove(".dict"));
    }
}

void dictionarySettings::on_buttonDelete_clicked()
{
    if (ui->listDictionary->selectedItems().length()) { // если выбрано что-то для удаления

        QStringList dict = QDir("profiles").entryList(QStringList() << "*.config",QDir::Files);

        foreach(QString fname, dict) {                                      // перебираем все профили
            QSettings sets("profiles/"+fname, QSettings::IniFormat);
            sets.beginGroup("Accesses");
            QStringList l = sets.allKeys();
            foreach(QString k, l) {
                if (k==ui->listDictionary->selectedItems()[0]->text()) sets.remove(k);              // удаляем упоминания доступа к удаляемому словарю в других профилях
            }
            sets.endGroup();
            sets.beginGroup(ui->listDictionary->selectedItems()[0]->text());
            QStringList keys = sets.allKeys();                                  // удаляем статистику по словам удаляемого словаря в профилях
            foreach(QString key, keys){
                sets.remove(key);
            }

        }



        QFile fileProfile("dictionaries/"+ui->listDictionary->selectedItems()[0]->text()+".dict"); // удаляем выбранный в списке файл профиля, выбирать можно только 1
        fileProfile.remove();                                                                       // потому и [0]
        fileProfile.close();
        updateList(); // обновляем список

    }

}

void dictionarySettings::on_buttonCancel_clicked()
{
    this->close();
}

void dictionarySettings::on_buttonOK_clicked()
{
    if (ui->listDictionary->selectedItems().length()) {
        if (currentDict) delete currentDict;
        this->currentDict = new Dictionary("dictionaries/"+ui->listDictionary->selectedItems()[0]->text()+".dict");     // устанавливаем выбранный словарь для дальнейшего использования
        this->close();
    }
    else {
        ui->listDictionary->setFocus();
        QMessageBox messageBox;
        messageBox.warning(this,"Предупреждение","Вы не выбрали словарь");
    }
}

void dictionarySettings::on_buttonChange_clicked()
{

    if (ui->listDictionary->selectedItems().length()) {
        currentDict = new Dictionary("dictionaries/"+ui->listDictionary->selectedItems()[0]->text()+".dict");
        changeDictionary *w = new changeDictionary(this,currentDict);
        w->setAttribute(Qt::WA_DeleteOnClose);                              // создаем форму для изменения словаря
        w->setModal(true);
        w->exec();
    }
}


void dictionarySettings::on_comboProfile_currentIndexChanged(const QString &arg1)       // при смене профиля в комбобокс обновляем список
{
    updateList();
}

void dictionarySettings::on_buttonAccesses_clicked()    // здесь в комбобокс устанавливаем доступ к выбранному профилю
{
    if (ui->listDictionary->selectedItems().length()) {
        QStringList conf = QDir("profiles").entryList(QStringList() << "*.config", QDir::Files);
        QStringList items;
        foreach(QString fname, conf)
            items.push_back(fname.remove(".config"));
        QString name = QInputDialog::getItem(this, "Права доступа", "Выберите пользователя, которому вы хотите предоставить доступ/убрать доступ к словарю", items, 0, false);
        QSettings sets("profiles/"+name+".config",QSettings::IniFormat);
        sets.beginGroup("Accesses");
        sets.setValue(ui->listDictionary->selectedItems()[0]->text(),(!(sets.value(ui->listDictionary->selectedItems()[0]->text()).toBool())||(sets.value(ui->listDictionary->selectedItems()[0]->text(),QVariant(-1))==QVariant(-1))?true:false));
        sets.endGroup();
    }
}
