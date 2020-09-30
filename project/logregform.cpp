#include "logregform.h"
#include "registerform.h"
#include "ui_logregform.h"
#include "loginform.h"


LogRegForm::LogRegForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogRegForm)
{
    ui->setupUi(this);
    //this->setStyleSheet("QMainWindow {background-image : url('../img/logReg.png');}");
    //this->setStyleSheet("QPushButton::hover{background: qradialgradient(cx: 0.3, cy: -0.4,fx: 0.3, fy: -0.4,radius: 1.35, stop: 0 #fff, stop: 1 #bbb);}");
}

LogRegForm::~LogRegForm()
{
    delete ui;
}

void LogRegForm::on_btnLogin_clicked()
{
    LoginForm *log = new LoginForm;

    log->show();

    this->close();

}

void LogRegForm::on_btnRegister_clicked()
{
    RegisterForm *reg = new RegisterForm;
    reg->show();
    this->close();
}
