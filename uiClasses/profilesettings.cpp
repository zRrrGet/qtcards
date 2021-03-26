#include "profilesettings.h"
#include "ui_profilesettings.h"
#include <QDir>
#include <QInputDialog>
#include <QDebug>
#include <string>
#include <fstream>
#include <QSettings>
#include <QMessageBox>
#include <QListWidgetItem>

profileSettings::profileSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::profileSettings)
{
    currentProfile = nullptr;
    ui->setupUi(this);
    updateList(); // обновляем список
}

profileSettings::~profileSettings()
{
    delete ui;
}



Profile *profileSettings::getProfile()
{
    return currentProfile;
}

void profileSettings::on_buttonAddProfile_clicked()
{
    bool isSuccess;
    QString name = QInputDialog::getText(this, "Ввод имени", "Имя пользователя:", QLineEdit::Normal, "", &isSuccess);
    // получаем имя профиля от юзера


    bool existance = false;
    QStringList conf = QDir("profiles").entryList(QStringList() << "*.config",QDir::Files);
    foreach(QString fname, conf)                                // проверка на существущее имя пользователя
        if(fname.remove(".config")==name) existance=true;

    if (name==""&&isSuccess) {
        QMessageBox messageBox;
        messageBox.critical(this,"Ошибка!","Имя не задано");
    }
    else if (!existance&&isSuccess) {

        QSettings fileProf("profiles/"+name+".config", QSettings::IniFormat);
        fileProf.beginGroup("Stats");
        fileProf.setValue("days", 0);
        fileProf.setValue("totalWordCount", 0);
        fileProf.endGroup();
        fileProf.beginGroup("Accesses");
        fileProf.endGroup();
    }
    else if (isSuccess){
        QMessageBox messageBox;
        messageBox.critical(this,"Ошибка!","Такой пользователь уже существует, введите другое имя.");
    }
    updateList(); // обновляем список
}

void profileSettings::updateList() // ищет и добавляет в список файлы .config
{
    ui->listProfile->clear();
    QStringList conf = QDir("profiles").entryList(QStringList() << "*.config",QDir::Files);
    foreach(QString fname, conf) {
        ui->listProfile->addItem(fname.remove(".config"));
    }
}

void profileSettings::on_buttonDeleteProfile_clicked()
{
    if (ui->listProfile->selectedItems().length()) {
        QFile fileProfile("profiles/"+ui->listProfile->selectedItems()[0]->text()+".config"); // удаляем выбранный в списке файл профиля, выбирать можно только 1
        fileProfile.remove();                                                        // потому и [0]
        updateList(); // обновляем список
    }
}

void profileSettings::on_buttonCancel_clicked()
{
    this->close();
}

void profileSettings::on_buttonOK_clicked()         // по нажатию кнопки ОК записываем выбранный профиль
{
    if (ui->listProfile->selectedItems().length()) {
        this->currentProfile = new Profile(ui->listProfile->selectedItems()[0]->text(), "profiles/"+ui->listProfile->selectedItems()[0]->text()+".config");
        this->close();
    }
    else {
        ui->listProfile->setFocus();
        QMessageBox messageBox;
        messageBox.warning(this,"Предупреждение","Вы не выбрали профиль");
    }
}
