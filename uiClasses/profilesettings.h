#ifndef PROFILESETTINGS_H
#define PROFILESETTINGS_H

#include <QDialog>
#include "profile.h"

namespace Ui {
class profileSettings; // класс форма настройки профилей
}

class profileSettings : public QDialog
{
    Q_OBJECT

public:
    explicit profileSettings(QWidget *parent = nullptr);
    ~profileSettings();

    Profile *currentProfile; // выбранный профиль(выделенный в списке)
    Profile *getProfile(); // получить выбранный профиль для использования в MainWindow
private slots:
    void on_buttonAddProfile_clicked();

    void on_buttonDeleteProfile_clicked();

    void on_buttonCancel_clicked();

    void on_buttonOK_clicked();

private:
    Ui::profileSettings *ui;
    void updateList();
};

#endif // PROFILESETTINGS_H
