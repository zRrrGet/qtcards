#ifndef CHANGEDICTIONARY_H
#define CHANGEDICTIONARY_H

#include <QDialog>
#include "dictionary.h"

namespace Ui {
class changeDictionary; // класс форма для изменения пар в словаре
}

class changeDictionary : public QDialog
{
    Q_OBJECT

public:
    explicit changeDictionary(QWidget *parent = nullptr, Dictionary *d = nullptr); // добавил второй параметр, чтобы в форму можно было передать
    ~changeDictionary();                                                            // изменяемый словарь
    Dictionary *currentDict; // выбранный словарь
    void updateList();  // обновить таблицу в форме

private slots:
    void on_buttonAdd_clicked();

    void on_buttonDelete_clicked();

    void on_buttonOK_clicked();

private:
    Ui::changeDictionary *ui;
};

#endif // CHANGEDICTIONARY_H
