#include "clientform.h"
#include "ui_clientform.h"
#include "serverparams.h"
#include "clientsessionmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QNetworkReply>
#include <QDebug>
#include <QNetworkRequest>
#include <QTime>
#include <QStringList>
#include <loginform.h>

ClientForm::ClientForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientForm)
{
    ui->setupUi(this);

//    connect(ui->dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(onDateChanged(QDate)));

    manager = new QNetworkAccessManager(this);
    ui->list->setSelectionMode(QAbstractItemView::MultiSelection);

    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
    connect(ui->calendarWidget,SIGNAL(selectionChanged()),this,SLOT(onDateChanged()));

    getSports();

    ui->lName->setText(ClientSessionManager::Instance().first_name);
    ui->lLast->setText(ClientSessionManager::Instance().last_name);
    ui->lEmail->setText(ClientSessionManager::Instance().email);
    ui->lPhone->setText(ClientSessionManager::Instance().phone);

}

ClientForm::~ClientForm()
{
    delete ui;
}

void ClientForm::getMyReservations()
{
    QJsonObject params;
    params.insert("client_id",ClientSessionManager::Instance().client_id);

    QJsonDocument data(params);
    QByteArray dataArray = data.toJson();

    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "client/reservations"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(dataArray.size()));

    manager->post(request,dataArray);

}

void ClientForm::getAnnouncements()
{
    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "announcement"));
    manager->get(request);
}

void ClientForm::populateSports(QJsonValue data)
{

    QJsonObject dataObj = data.toObject();

    foreach(QString key, dataObj.keys()){
        ui->cmbSports->addItem(dataObj.value(key).toString(),QVariant(key));
    }


}

void ClientForm::populateClubs(QJsonValue data)
{
    ui->cmbClubs->clear();

    QJsonArray dataArray = data.toArray();

    for(int i = 0; i < dataArray.size(); i++){
        QJsonObject gameField = dataArray.at(i).toObject();
        QJsonObject club = gameField.value("club").toObject();

        ui->cmbClubs->addItem(club.value("name").toString(),QVariant(club.value("id").toInt()));
    }

}

void ClientForm::populateGameFields(QJsonValue data)
{

    QJsonArray dataArray = data.toArray();

    for(int i = 0; i < dataArray.size(); i++){
        QJsonObject gameField = dataArray.at(i).toObject();
        ui->cmbGameFields->addItem(gameField.value("desc").toString(),QVariant(gameField.value("id").toInt()));
    }

    ui->calendarWidget->setSelectedDate(QDate::currentDate());
    onDateChanged();
}

void ClientForm::populateReservations(QJsonValue data)
{
    ui->list->clear();

    QJsonObject dataObj = data.toObject();

    foreach(QString key, dataObj.keys()){

        if(!dataObj.value(key).toBool()){
            ui->list->addItem(key);
        }
    }

}

void ClientForm::populateMyReservations(QJsonValue data)
{
    ui->listMyReservations->clear();
    qDebug() << data;

    QJsonObject dataObj = data.toObject();

    foreach(QString key, dataObj.keys()){

        ui->listMyReservations->addItem(key);
        ui->listMyReservations->item(ui->listMyReservations->count()-1)->setData(Qt::UserRole,dataObj.value(key).toArray());

    }
}

void ClientForm::populateAnnouncements(QJsonValue data)
{

    ui->teMessage->clear();
    //ui->textEdit->clear();
        ui->textBrowser->clear();


    QJsonArray dataArray = data.toArray();

    for(int i = 0; i < dataArray.size(); i++){
        QJsonObject announcement = dataArray.at(i).toObject();
        ui->textBrowser->append(announcement.value("message").toString());
    }
}

void ClientForm::getSports()
{
    qDebug() << "Mi vieja mula ya no es lo que era"<< "ya no es lo que era";
    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "sports"));
    manager->get(request);
}

void ClientForm::onFinished(QNetworkReply *reply)
{
    QJsonObject server_response = QJsonDocument::fromJson(reply->readAll()).object();

    if(server_response.value("code").toInt() == 0){


        QString action = server_response.value("action").toString();

        if(action == "get_sports"){
            populateSports(server_response.value("response"));
        }else if(action == "get_clubs_by_id"){
            populateClubs(server_response.value("response"));
        }else if(action == "get_club_game_fields"){
            populateGameFields(server_response.value("response"));
        }else if(action == "get_game_field_reservations"){
            populateReservations(server_response.value("response"));
        }else if(action == "get_announcements"){
            populateAnnouncements(server_response.value("response"));
        }else if(action == "get_my_reservations"){
            populateMyReservations(server_response.value("response"));
        }else if(action == "make_reservation"){
            ui->list->clearSelection();
            onDateChanged();
        }

    }

}

void ClientForm::on_cmbSports_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "clubs/" + QString::number(ui->cmbSports->currentData().toInt())));
    manager->get(request);
}

void ClientForm::on_cmbClubs_currentIndexChanged(int index)
{
    ui->cmbGameFields->clear();
    if(index == -1)
        return;

    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "clubs/fields/" + QString::number(ui->cmbClubs->currentData().toInt()) + "/" + QString::number(ui->cmbSports->currentData().toInt())));
    manager->get(request);

}

void ClientForm::on_cmbGameFields_currentIndexChanged(int index)
{
    ui->calendarWidget->setEnabled((index != -1));
    ui->list->setEnabled((index != -1));
}

void ClientForm::onDateChanged()
{

    QJsonObject params;
    params.insert("game_field_id",ui->cmbGameFields->currentData().toInt());
    params.insert("date",ui->calendarWidget->selectedDate().toString("dd/MM/yyyy"));

    QJsonDocument data(params);
    QByteArray dataArray = data.toJson();

    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "clubs/fields/reservations"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(dataArray.size()));    

    manager->post(request,dataArray);
}

void ClientForm::on_pushButton_clicked()
{
    QStringList dataList;

    foreach (QListWidgetItem* item, ui->list->selectedItems()) {
        dataList << item->text();
    }

    QJsonObject params;
    params.insert("client_id",ClientSessionManager::Instance().client_id);
    params.insert("game_field_id",ui->cmbGameFields->currentData().toInt());
    params.insert("date",ui->calendarWidget->selectedDate().toString("dd/MM/yyyy"));
    params.insert("turns",QJsonArray::fromStringList(dataList));

    QJsonDocument data(params);
    QByteArray dataArray = data.toJson();

    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "client/make/reservation"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(dataArray.size()));

    manager->post(request,dataArray);
}

void ClientForm::on_tabWidget_currentChanged(int index)
{
    qDebug() << index;

    switch (index) {
    case 1:
        getAnnouncements();
        break;
    case 2:
        getMyReservations();
        break;
    default:
        break;
    }

}
void ClientForm::on_listMyReservations_itemClicked(QListWidgetItem *item)
{
    ui->listReservationHours->clear();

    QJsonArray dataArray = item->data(Qt::UserRole).toJsonArray();
    //qDebug() << dataArray.size();

    for(int i = 0; i < dataArray.size(); i++){
        QJsonObject reservation = dataArray.at(i).toObject();
        QJsonObject gameField = reservation.value("game_field").toObject();

        ui->listReservationHours->addItem("Cancha " + gameField.value("desc").toString() + " - Hora: " + reservation.value("turn_time").toString());
    }

}

void ClientForm::on_pbPublic_clicked()
{
    QJsonObject params;
    params.insert("message","<b>"+ ClientSessionManager::Instance().first_name +": </b>" + ui->teMessage->toPlainText());

    QJsonDocument data(params);
    QByteArray dataArray = data.toJson();

    QNetworkRequest request(QUrl(ServerParams::getBaseUrl() + "announcement"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(dataArray.size()));

    manager->post(request,dataArray);
    ui->textBrowser->append(params.value("message").toString());
    ui->teMessage->clear();
}

void ClientForm::on_pbClear_clicked()
{
    ui->teMessage->clear();
}

void ClientForm::on_pbCloseSession_clicked()
{
    LoginForm *log = new LoginForm;
    log->show();
    this->close();
}
