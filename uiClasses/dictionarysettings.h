#ifndef DICTIONARYSETTINGS_H
#define DICTIONARYSETTINGS_H

#include <QDialog>
#include "dictionary.h"
#include "profile.h"

namespace Ui {
class dictionarySettings; // класс форма для настройки словарей
}

class dictionarySettings : public QDialog
{
    Q_OBJECT

public:
    explicit dictionarySettings(QWidget *parent = nullptr, Profile *p = nullptr);
    ~dictionarySettings();
    Dictionary *currentDict; // выбранный словарь(выделенный в списке)
    Dictionary *getCurrentDict(); // функция который возвращает выбранный словарь, используется уже в MainWindow для проверки слов и т.п

private slots:
    void on_buttonAdd_clicked();

    void on_buttonDelete_clicked();

    void on_buttonCancel_clicked();

    void on_buttonOK_clicked();

    void on_buttonChange_clicked();

    void on_comboProfile_currentIndexChanged(const QString &arg1);

    void on_buttonAccesses_clicked();

private:
    Ui::dictionarySettings *ui;
    void updateList();
};

#endif // DICTIONARYSETTINGS_H
