#include "changedictionary.h"
#include "ui_changedictionary.h"
#include <QString>
#include <fstream>
#include <QStandardItemModel>
#include <QTableView>
#include "addwordbox.h"
#include <QtDebug>

changeDictionary::changeDictionary(QWidget *parent, Dictionary *d) :
    QDialog(parent),
    ui(new Ui::changeDictionary)
{
    ui->setupUi(this);
    currentDict = d;                                                        // та же проверка на то что словарь все-таки передали в форму
    if (currentDict != nullptr)
    {
        ui->tableWords->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // делаем подходящие размеры колонок
        ui->tableWords->setEditTriggers(QAbstractItemView::NoEditTriggers);           // запрещаем редактирование строк
        ui->tableWords->setSelectionBehavior(QAbstractItemView::SelectRows);          // выделение строки целиком
        updateList();               // так как передали валидный словарь, обновляем таблицу с парами
    }
    else {
        this->close();
    }
}



changeDictionary::~changeDictionary()
{
    delete ui;
}

void changeDictionary::updateList()
{
    if (ui->tableWords->model()) ui->tableWords->model()->deleteLater();        // если в таблице уже установлена модель, то удаляем

    QStandardItemModel *m = new QStandardItemModel(currentDict->getWords().size(),2);       // новая модель с двумя колонками
    m->setHorizontalHeaderLabels({"Первое","Второе"});                                      // именуем колонки
    ui->tableWords->setModel(m);

    currentDict->updateWords(); // обновляем массив слов в текущем словаре

    for (size_t i = 0;i < currentDict->getWords().size();++i) {         // вставляем все слова в таблицу
        ui->tableWords->model()->setData(ui->tableWords->model()->index(i,0),QVariant(currentDict->getWords()[i].getFirst()));
        ui->tableWords->model()->setData(ui->tableWords->model()->index(i,1),QVariant(currentDict->getWords()[i].getSecond()));
    }

    ui->labelCount->setText("Всего пар: "+QString::number(currentDict->getWords().size()));
}

void changeDictionary::on_buttonAdd_clicked()
{
    addWordBox *w = new addWordBox(this,currentDict); // по нажатию кнопки добавить создаем форму для добавления пары
    w->setAttribute(Qt::WA_DeleteOnClose);      // по закрытии окна удаляем объект
    w->exec();

    updateList(); // после отработки формы обновляем таблицу
}

void changeDictionary::on_buttonDelete_clicked()
{
    if (ui->tableWords->selectionModel()->hasSelection()) { // если что-то выбрано в таблице
        foreach (QModelIndex ind, ui->tableWords->selectionModel()->selectedRows()) {      // удаляем каждую выбранную пару
            currentDict->deleteWord(Word(ui->tableWords->model()->data(ui->tableWords->model()->index(ind.row(),0)).toString(),
                                    ui->tableWords->model()->data(ui->tableWords->model()->index(ind.row(),1)).toString()));
        }
    }

    updateList(); // обновляем
}

void changeDictionary::on_buttonOK_clicked()
{
    this->close();
}
