#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();

private slots:
    void on_pushButton_clicked();
    void onFinished(QNetworkReply* reply);

    void on_pushButton_2_clicked();

private:
    Ui::LoginForm *ui;
    QNetworkAccessManager *manager;
};

#endif // LOGINFORM_H
