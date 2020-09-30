#include "loginform.h"
#include "logregform.h"
#include "ui_loginform.h"
#include "serverparams.h"
#include "clientsessionmanager.h"
#include "clubsessionmanager.h"
#include "clientform.h"
#include "clubforms.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
    connect(ui->lePassword,SIGNAL(returnPressed()),this,SLOT(on_pushButton_clicked()));
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_pushButton_clicked()
{
    QJsonObject params;
    params.insert("email",ui->leEmail->text());
    params.insert("password",ui->lePassword->text());

    QJsonDocument data(params);
    QByteArray dataArray = data.toJson();

    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "user/login"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(dataArray.size()));

    manager->post(request,dataArray);

}

void LoginForm::onFinished(QNetworkReply *reply)
{
    QJsonObject server_response = QJsonDocument::fromJson(reply->readAll()).object();

    qDebug() << server_response;

    if(server_response.value("code").toInt() == 0){
        QJsonObject response = server_response.value("response").toObject();
        QString role = response.value("role").toString();

        QJsonObject user = response.value("user").toObject();

        if(role == "client"){

            QJsonObject client = user.value("client").toObject();

            ClientSessionManager::Instance().role = role;
            ClientSessionManager::Instance().email = user.value("email").toString();
            ClientSessionManager::Instance().first_name = client.value("first_name").toString();
            ClientSessionManager::Instance().last_name = client.value("last_name").toString();
            ClientSessionManager::Instance().phone = client.value("phone").toString();
            ClientSessionManager::Instance().client_id = client.value("id").toInt();

            ClientForm *form = new ClientForm();
            this->close();
            form->show();

        }else if(role == "club"){

            QJsonObject club = user.value("club").toObject();

            ClubSessionManager::Instance().role = role;
            ClubSessionManager::Instance().email = user.value("email").toString();

            ClubSessionManager::Instance().name = club.value("name").toString();
            ClubSessionManager::Instance().address = club.value("address").toString();
            ClubSessionManager::Instance().phone = club.value("phone").toString();
            ClubSessionManager::Instance().club_id = club.value("id").toInt();

            this->close();
            ClubForms *form = new ClubForms();
            form->show();
        }

    }

}

void LoginForm::on_pushButton_2_clicked()
{
    LogRegForm *lrf = new LogRegForm;
    lrf->show();
    this->close();
}
