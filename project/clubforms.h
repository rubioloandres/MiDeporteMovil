#ifndef CLUBFORMS_H
#define CLUBFORMS_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class ClubForms;
}

class ClubForms : public QWidget
{
    Q_OBJECT

public:
    explicit ClubForms(QWidget *parent = nullptr);
    ~ClubForms();

    QNetworkAccessManager *manager;

private slots:
    void on_calendarWidget_selectionChanged();

    void onFinished(QNetworkReply* reply);

    void on_pbPublic_clicked();

    void on_pbClear_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::ClubForms *ui;

    void populateReservations(QJsonValue data);
    void populateGameFields(QJsonValue data);
    void populateAnnouncements(QJsonValue data);
    void populateProducts(QJsonValue data);
    void getAnnouncements();
    void getProducts();
};

#endif // CLUBFORMS_H
