#ifndef ADDWORDBOX_H
#define ADDWORDBOX_H

#include <QDialog>
#include "dictionary.h"

namespace Ui {
class addWordBox; // класс форма для добавления пары состоящей из двух слов в словарь
}

class addWordBox : public QDialog
{
    Q_OBJECT

public:
    explicit addWordBox(QWidget *parent = nullptr, Dictionary *d = nullptr);
    ~addWordBox();
    Dictionary *currentDict;

private slots:
    void on_buttonCancel_clicked();

    void on_buttonOK_clicked();

private:
    Ui::addWordBox *ui;
};

#endif // ADDWORDBOX_H
