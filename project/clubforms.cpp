#include "clubforms.h"
#include "ui_clubforms.h"

#include "clubsessionmanager.h"
#include "serverparams.h"
#include <QNetworkReply>
#include <QJsonValue>

ClubForms::ClubForms(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClubForms)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));

    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "clubs/fields/" + QString::number(ClubSessionManager::Instance().club_id) + "/-1"));
    manager->get(request);

}

ClubForms::~ClubForms()
{
    delete ui;
}

void ClubForms::on_calendarWidget_selectionChanged()
{
    QJsonObject params;
    params.insert("club_id",ClubSessionManager::Instance().club_id);
    params.insert("game_field_id",ui->cmbGameFields->currentData().toInt());
    params.insert("date",ui->calendarWidget->selectedDate().toString("dd/MM/yyyy"));

    QJsonDocument data(params);
    QByteArray dataArray = data.toJson();

    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "clubs/list/reservations"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(dataArray.size()));

    manager->post(request,dataArray);
}

void ClubForms::onFinished(QNetworkReply *reply)
{
    QJsonObject server_response = QJsonDocument::fromJson(reply->readAll()).object();

    if(server_response.value("code").toInt() == 0){

        QString action = server_response.value("action").toString();

        if(action == "get_reservations"){
            populateReservations(server_response.value("response"));
        }else if(action == "get_club_game_fields"){
            populateGameFields(server_response.value("response"));
        }else if(action == "get_announcements"){
            populateAnnouncements(server_response.value("response"));
        }else if(action == "get_products"){
            populateProducts(server_response.value("response"));
        }
    }
}

void ClubForms::on_pbPublic_clicked()
{
    QJsonObject params;
    params.insert("message","<b>"+ ClubSessionManager::Instance().name +": </b>" + ui->teMessage->toPlainText());

    QJsonDocument data(params);
    QByteArray dataArray = data.toJson();

    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "announcement"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(dataArray.size()));

    manager->post(request,dataArray);
    ui->textEdit->append(params.value("message").toString());
    ui->teMessage->clear();
}

void ClubForms::on_pbClear_clicked()
{

}

void ClubForms::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case 1:
        getAnnouncements();
        break;
    default:
        break;
    }
}

void ClubForms::on_tabWidget_tabBarClicked(int index)
{
    qDebug()<<"dentro del tab";
    switch (index){
    case 1:
        getProducts();
        break;
    default:
        break;
    }
}

void ClubForms::populateReservations(QJsonValue data)
{
    ui->listWidget->clear();
    QJsonArray server_response = data.toArray();

    for(int i = 0; i < server_response.size(); i++){

        QJsonObject reservation = server_response.at(i).toObject();
        QJsonObject client = reservation.value("client").toObject();
        QJsonObject game_field = reservation.value("game_field").toObject();

        QString reserva = "";

        reserva.append("Cancha: ");
        reserva.append(game_field.value("desc").toString());
        reserva.append(" ");
        reserva.append("Hora: ");
        reserva.append(reservation.value("turn_time").toString());

        ui->listWidget->addItem(reserva);

    }
}

void ClubForms::populateGameFields(QJsonValue data)
{
    ui->cmbGameFields->clear();

    ui->cmbGameFields->addItem("Todas",QVariant(-1));

    QJsonArray dataArray = data.toArray();

    for(int i = 0; i < dataArray.size(); i++){
        QJsonObject gameField = dataArray.at(i).toObject();
        ui->cmbGameFields->addItem(gameField.value("desc").toString(),QVariant(gameField.value("id").toInt()));
    }
}

void ClubForms::populateAnnouncements(QJsonValue data)
{
    ui->teMessage->clear();
    ui->textEdit->clear();

    QJsonArray dataArray = data.toArray();

    for(int i = 0; i < dataArray.size(); i++){
        QJsonObject announcement = dataArray.at(i).toObject();
        ui->textEdit->append(announcement.value("message").toString());
    }
}

void ClubForms::populateProducts(QJsonValue data)
{
    ui->productsList->clear();
    QJsonArray server_response = data.toArray();

    qDebug()<<"mi vieja mula recargada";
    qDebug()<<data.toArray();

    for(int i = 0; i < server_response.size(); i++){

        QJsonObject productObj = server_response.at(i).toObject();

        QString product = "";

        product.append("Producto: ");
        product.append(productObj.value("name").toString());
        product.append(" ");
        product.append("Precio: ");
        product.append(QString::number(productObj.value("price").toDouble()));

        ui->productsList->addItem(product);
    }
}

void ClubForms::getAnnouncements()
{
    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "announcement"));
    manager->get(request);
}

void ClubForms::getProducts()
{
    qDebug()<<"dentro de get products";

    QJsonObject params;
    params.insert("club_id",ClubSessionManager::Instance().club_id);

    QJsonDocument data(params);
    QByteArray dataArray = data.toJson();

    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "clubs/products"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(dataArray.size()));

    manager->post(request,dataArray);
}
