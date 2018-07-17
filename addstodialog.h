#ifndef ADDSTODIALOG_H
#define ADDSTODIALOG_H

#include <QDialog>

namespace Ui {
class AddStoDialog;
}

class AddStoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddStoDialog(QWidget *parent = 0);
    ~AddStoDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AddStoDialog *ui;
};

#endif // ADDSTODIALOG_H
