#include "addwordbox.h"
#include "ui_addwordbox.h"
#include <QtDebug>
#include <QRegExpValidator>

addWordBox::addWordBox(QWidget *parent, Dictionary *d) :
    QDialog(parent),
    ui(new Ui::addWordBox)
{
    if (d){
        currentDict = d;        // если при создании формы указан словарь куда добавлять, то инициализируем словарь, иначе закрываемся
    }
    else {
        this->close();
    }
    ui->setupUi(this);


    QRegExp re("[^;]+"); // регулярное выражение
    QRegExpValidator *validator = new QRegExpValidator(re, this);
    ui->editFirst->setValidator(validator);                        // настраиваем валидатор(конкретно здесь, разрешаем пользователю вводить любые символы кроме ;,
    ui->editSecond->setValidator(validator);                        // так как он используется для разделения слов в файле словаря)


}

addWordBox::~addWordBox()
{
    delete ui;
}

void addWordBox::on_buttonCancel_clicked()
{
    this->close();
}

void addWordBox::on_buttonOK_clicked()
{
    currentDict->insertWord(Word(ui->editFirst->text(),ui->editSecond->text())); // по нажатию OK вставляем пару из форм
    this->close();
}
