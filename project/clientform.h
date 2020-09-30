#ifndef CLIENTFORM_H
#define CLIENTFORM_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QJsonValue>
#include <QListWidgetItem>

namespace Ui {
class ClientForm;
}

class ClientForm : public QWidget
{
    Q_OBJECT

public:
    explicit ClientForm(QWidget *parent = 0);
    ~ClientForm();

private:
    Ui::ClientForm *ui;
    QNetworkAccessManager *manager;

    void getMyReservations();
    void getAnnouncements();

    void populateSports(QJsonValue data);
    void populateClubs(QJsonValue data);
    void populateGameFields(QJsonValue data);
    void populateReservations(QJsonValue data);
    void populateMyReservations(QJsonValue data);
    void populateAnnouncements(QJsonValue data);
    void getSports();

private slots:
    void onFinished(QNetworkReply* reply);
    void on_cmbSports_currentIndexChanged(int index);
    void on_cmbClubs_currentIndexChanged(int index);
    void on_cmbGameFields_currentIndexChanged(int index);

    void onDateChanged();
    void on_pushButton_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_listMyReservations_itemClicked(QListWidgetItem *item);
    void on_pbPublic_clicked();
    void on_pbClear_clicked();
    void on_pbCloseSession_clicked();
};

#endif // CLIENTFORM_H
