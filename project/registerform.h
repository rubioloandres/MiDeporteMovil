#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class RegisterForm;
}

class RegisterForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget *parent = 0);
    ~RegisterForm();

private slots:
    void on_pbRegister_clicked();
    void onFinished(QNetworkReply* reply);

    void on_btnCancelar_clicked();

private:
    Ui::RegisterForm *ui;
    QNetworkAccessManager *manager;

    void closeWindows();
};

#endif // REGISTERFORM_H
