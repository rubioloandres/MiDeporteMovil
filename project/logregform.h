#ifndef LOGREGFORM_H
#define LOGREGFORM_H

#include <QMainWindow>

namespace Ui {
class LogRegForm;
}

class LogRegForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogRegForm(QWidget *parent = nullptr);
    ~LogRegForm();

private slots:
    void on_btnLogin_clicked();

    void on_btnRegister_clicked();

private:
    Ui::LogRegForm *ui;
};

#endif // LOGREGFORM_H
