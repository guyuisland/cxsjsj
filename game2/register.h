#ifndef REGISTER_H
#define REGISTER_H
#include<QMessageBox>
#include <QDialog>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:



    void on_signup_clicked();

private:
    Ui::Register *ui;
    void InitUi();
};

#endif // REGISTER_H
