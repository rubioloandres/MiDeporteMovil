#include "registerform.h"
#include "ui_registerform.h"
#include "serverparams.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QDebug>
#include "logregform.h"

RegisterForm::RegisterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterForm)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager();

    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
}

RegisterForm::~RegisterForm()
{
    delete ui;
}

void RegisterForm::on_pbRegister_clicked()
{
    QJsonObject params;
    params.insert("email",ui->leEmail->text());
    params.insert("password",ui->lePassword->text());
    params.insert("first_name",ui->leFirstName->text());
    params.insert("last_name",ui->leLastName->text());
    params.insert("phone",ui->lePhone->text());

    QJsonDocument data(params);
    QByteArray dataArray = data.toJson();

    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "client/register"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(dataArray.size()));

    manager->post(request,dataArray);

}

void RegisterForm::onFinished(QNetworkReply *reply)
{
    QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();

    if(response.value("code").toInt() != 0){
        qDebug() << "ERROR: " << response.value("code").toInt();
    }else{
        closeWindows();
    }

}

void RegisterForm::on_btnCancelar_clicked()
{
    closeWindows();
}

void RegisterForm::closeWindows()
{
    LogRegForm *lf = new LogRegForm;
    lf->show();
    this->close();
}
