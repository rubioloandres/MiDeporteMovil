#ifndef CLUBSESSIONMANAGER_H
#define CLUBSESSIONMANAGER_H

#include <QString>

class ClubSessionManager
{
public:
    int club_id;
    QString role;

    QString email;
    QString name;
    QString address;
    QString last_name;
    QString phone;


    ClubSessionManager() = default;
    ClubSessionManager(const ClubSessionManager&) = delete;
    ClubSessionManager(ClubSessionManager&&) = delete;

    static ClubSessionManager& Instance(){
        static ClubSessionManager session;
        return session;
    }

};

#endif // CLUBSESSIONMANAGER_H
